#include "facelogindialog.h"
#include "ui_facelogindialog.h"
#include "connection.h"
#include <QDir>
#include <QStandardPaths>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMediaDevices>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimedia/QImageCapture>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QBuffer>



FaceLoginDialog::FaceLoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FaceLoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("Authentification par reconnaissance faciale");

    // Initialisation de la base de données
    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données");
        return;
    }

    // Configuration de la caméra
    camera = new QCamera(QMediaDevices::defaultVideoInput(), this);
    viewfinder = new QVideoWidget(ui->cameraFrame);
    captureSession = new QMediaCaptureSession(this);
    imageCapture = new QImageCapture(this);

    // Ajuster la taille du viewfinder pour remplir le cadre
    viewfinder->setGeometry(ui->cameraFrame->rect());

    captureSession->setCamera(camera);
    captureSession->setVideoOutput(viewfinder);
    captureSession->setImageCapture(imageCapture);

    // Connexions
    connect(ui->captureButton, &QPushButton::clicked, this, &FaceLoginDialog::onCaptureClicked);
    connect(imageCapture, &QImageCapture::imageCaptured, this, &FaceLoginDialog::onImageCaptured);
    connect(imageCapture, &QImageCapture::errorOccurred, this, [this](int /*id*/, QImageCapture::Error error, const QString &errorString) {
        qDebug() << "Erreur de capture:" << error << errorString;
        ui->statusLabel->setText("Erreur de capture. Veuillez réessayer.");
        ui->captureButton->setEnabled(true);
    });

    // Créer un QLabel transparent pour le cadre de guidage
    frameLabel = new QLabel(ui->cameraFrame);
    frameLabel->setGeometry(ui->cameraFrame->rect());
    frameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    frameLabel->raise();

    // Créer et dessiner l'image du cadre
    QImage frameImage(frameLabel->size(), QImage::Format_ARGB32_Premultiplied);
    frameImage.fill(Qt::transparent);

    QPainter painter(&frameImage);
    painter.setPen(QPen(Qt::green, 2));

    // Dessiner le rectangle guide au centre
    QRect guideRect = frameLabel->rect();
    guideRect.setSize(guideRect.size() * 0.7);
    guideRect.moveCenter(frameLabel->rect().center());
    painter.drawRect(guideRect);

    // Appliquer l'image au label
    frameLabel->setPixmap(QPixmap::fromImage(frameImage));
    frameLabel->show();

    // Démarrer la caméra
    camera->start();

    // Message initial
    ui->statusLabel->setText("Positionnez votre visage dans le cadre vert et cliquez sur Capturer");
}

FaceLoginDialog::~FaceLoginDialog()
{
    // Arrêter la caméra
    if (camera) {
        camera->stop();
    }

    // Nettoyer les ressources
    delete imageCapture;
    delete captureSession;
    delete viewfinder;
    delete camera;
    delete frameLabel;
    delete ui;
}

void FaceLoginDialog::onCaptureClicked()
{
    if (!camera->isActive()) {
        ui->statusLabel->setText("La caméra n'est pas active");
        return;
    }

    ui->captureButton->setEnabled(false);
    ui->statusLabel->setText("Capture en cours...");

    // Capture avec un délai pour laisser le temps à la caméra de se stabiliser
    QTimer::singleShot(500, this, [this]() {
        imageCapture->capture();
    });
}

void FaceLoginDialog::onImageCaptured(int /*id*/, const QImage &preview)
{
    capturedImage = preview;

    // Détecter le visage
    QImage processedImage = detectAndDrawFacialLandmarks(capturedImage);

    if (processedImage.isNull()) {
        ui->statusLabel->setText("❌ Aucun visage détecté. Veuillez réessayer.");
        ui->captureButton->setEnabled(true);
        return;
    }

    // Afficher l'image traitée dans une nouvelle fenêtre
    QDialog *resultDialog = new QDialog(this);
    resultDialog->setWindowTitle("Résultat de la détection");

    QVBoxLayout *layout = new QVBoxLayout(resultDialog);
    QLabel *imageLabel = new QLabel(resultDialog);

    // Charger l'image traitée
    QPixmap pixmap = QPixmap::fromImage(processedImage);
    if (!pixmap.isNull()) {
        // Redimensionner l'image si elle est trop grande
        if (pixmap.width() > 800) {
            pixmap = pixmap.scaledToWidth(800, Qt::SmoothTransformation);
        }
        imageLabel->setPixmap(pixmap);
    }

    layout->addWidget(imageLabel);

    // Ajouter des boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *acceptButton = new QPushButton("Accepter", resultDialog);
    QPushButton *retryButton = new QPushButton("Réessayer", resultDialog);

    connect(acceptButton, &QPushButton::clicked, [this, resultDialog, processedImage]() {
        // Mode Enregistrement
        if (isRegistrationMode) {
            if (saveFaceIDToDatabase(currentEmployeeId, processedImage)) {
                ui->statusLabel->setText("✅ FaceID enregistré avec succès");
                resultDialog->accept();
                QTimer::singleShot(1000, this, &QDialog::accept);
            } else {
                ui->statusLabel->setText("❌ Erreur lors de l'enregistrement");
                ui->captureButton->setEnabled(true);
                resultDialog->reject();
            }
        }
        // Mode Authentification
        else {
            QDir tempDir = QDir::temp();
            tempImagePath = tempDir.filePath("face_temp.png");
            processedImage.save(tempImagePath);
            resultDialog->accept();
            processFaceRecognition();
        }
    });

    connect(retryButton, &QPushButton::clicked, [this, resultDialog]() {
        ui->captureButton->setEnabled(true);
        resultDialog->reject();
    });

    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(retryButton);
    layout->addLayout(buttonLayout);

    resultDialog->setModal(true);
    resultDialog->exec();
}

QString FaceLoginDialog::findMatchingEmployee(const QString &tempImage)
{
    QSqlQuery query;
    query.prepare("SELECT id, nom, face_id FROM employe WHERE face_id IS NOT NULL");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche des employés:" << query.lastError().text();
        return QString();
    }

    while (query.next()) {
        QByteArray faceData = query.value("face_id").toByteArray();
        QString employeeId = query.value("id").toString();
        QString employeeName = query.value("nom").toString();
        QString storedImagePath = QDir::tempPath() + "/stored_face_" + employeeId + ".png";

        // Vérification si les données du visage existent
        if (faceData.isEmpty()) {
            qDebug() << "Aucune donnée de visage pour l'employé:" << employeeName;
            continue;
        }

        // Écrire le fichier temporaire pour l'image du visage stockée
        QFile file(storedImagePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(faceData);
            file.close();
            qDebug() << "Fichier temporaire créé pour l'employé:" << employeeName;

            // Comparer les visages
            if (compareFaces(tempImage, storedImagePath)) {
                file.remove(); // Nettoyer le fichier temporaire après la comparaison
                authenticatedEmployeeId = employeeId; // Stocker l'ID de l'employé authentifié
                return employeeName; // Renvoi du nom de l'employé
            }

            file.remove(); // Nettoyer le fichier temporaire si la comparaison échoue
        } else {
            qDebug() << "Erreur lors de l'ouverture du fichier temporaire pour l'employé:" << employeeName;
        }
    }

    return QString(); // Si aucun employé n'a été trouvé
}
bool FaceLoginDialog::compareFaces(const QString &tempImage, const QString &storedImage)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    QString pythonExe = "C:/Users/Yasmine/AppData/Local/Programs/Python/Python311/python.exe";
    QString pythonScript = "C:/Users/Yasmine/Desktop/testtttfinal/compare_faces.py"; // Use source path, not build dir!

    // Verify paths
    if (!QFile::exists(pythonScript)) {
        qDebug() << "Python script missing:" << pythonScript;
        return false;
    }

    if (!QFile::exists(tempImage) || !QFile::exists(storedImage)) {
        qDebug() << "Image files missing";
        return false;
    }

    // Run process
    process.start(pythonExe, {pythonScript, tempImage, storedImage});

    if (!process.waitForFinished(10000)) {
        qDebug() << "Python timeout:" << process.errorString();
        return false;
    }

    // Get output
    QString output = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    QString error = QString::fromUtf8(process.readAllStandardError()).trimmed();

    qDebug() << "Raw Python output:" << output;
    qDebug() << "Python errors:" << error;

    return output == "OK";
}
void FaceLoginDialog::processFaceRecognition()
{
    QString employeeName = findMatchingEmployee(tempImagePath);

    if (!employeeName.isEmpty()) {
        ui->statusLabel->setText(QString("✅ Bienvenue %1").arg(employeeName)); // Affichage du message de bienvenue avec le nom
        qDebug() << "Visage reconnu, bienvenue " << employeeName;
        accept(); // Ferme la boîte de dialogue immédiatement
    } else {
        ui->statusLabel->setText("❌ Visage non reconnu");
        authenticatedEmployeeId.clear();
    }
}
bool FaceLoginDialog::registerFaceID(QWidget* parent, const QString& employeeId)
{
    FaceLoginDialog dialog(parent);
    dialog.isRegistrationMode = true;
    dialog.currentEmployeeId = employeeId;
    dialog.setWindowTitle("Enregistrement du FaceID");
    dialog.ui->statusLabel->setText("Positionnez votre visage dans le cadre");
    dialog.ui->captureButton->setText("Enregistrer le FaceID");

    return (dialog.exec() == QDialog::Accepted);
}

QImage FaceLoginDialog::detectAndDrawFacialLandmarks(const QImage &image)
{
    QProcess process;
    QString appDir = QCoreApplication::applicationDirPath();
    QString pythonScript = appDir + "/face_recognition.py";
    QString cascadeFile = appDir + "/haarcascade_frontalface_default.xml";

    qDebug() << "Chemin du script Python:" << pythonScript;
    qDebug() << "Chemin du fichier cascade:" << cascadeFile;

    // Vérifier si les fichiers existent
    if (!QFile::exists(pythonScript)) {
        qDebug() << "ERREUR: Le script Python n'existe pas:" << pythonScript;
        return QImage();
    }

    if (!QFile::exists(cascadeFile)) {
        qDebug() << "ERREUR: Le fichier cascade n'existe pas:" << cascadeFile;
        return QImage();
    }

    // Copier le fichier cascade dans le dossier temporaire
    QString tempCascade = QDir::temp().filePath("haarcascade_frontalface_default.xml");
    if (QFile::exists(tempCascade)) {
        QFile::remove(tempCascade);
    }
    if (!QFile::copy(cascadeFile, tempCascade)) {
        qDebug() << "ERREUR: Impossible de copier le fichier cascade";
        return QImage();
    }

    // Sauvegarder l'image temporairement
    QString tempInput = QDir::temp().filePath("input_temp.png");
    if (!image.save(tempInput)) {
        qDebug() << "ERREUR: Impossible de sauvegarder l'image temporaire";
        return QImage();
    }

    // Changer le répertoire de travail vers le dossier temporaire
    QString currentPath = QDir::currentPath();
    QDir::setCurrent(QDir::temp().path());

    // Appeler le script Python qui détecte et dessine les repères
    QString pythonExe = "C:/Users/Yasmine/AppData/Local/Programs/Python/Python311/python.exe";
    process.start(pythonExe, QStringList() << pythonScript << tempInput);

    if (!process.waitForFinished(60000)) { // 60 secondes timeout
        qDebug() << "Erreur: Le script Python n'a pas terminé dans le délai imparti";
        return QImage();
    }

    // Lire la sortie et l'erreur après que le processus soit terminé
    QString output = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();

    qDebug() << "Sortie Python:" << output;
    qDebug() << "Erreurs Python:" << errorOutput;

    // Restaurer le répertoire de travail
    QDir::setCurrent(currentPath);

    if (output.contains("NO_FACE") || output.isEmpty()) {
        // Nettoyer les fichiers temporaires
        QFile::remove(tempInput);
        QFile::remove(tempCascade);
        return QImage();
    }

    // Le script Python sauvegarde l'image avec les repères
    QString outputPath = QDir::temp().filePath("output_temp.png");
    QImage result(outputPath);

    // Nettoyer les fichiers temporaires
    QFile::remove(tempInput);
    QFile::remove(outputPath);
    QFile::remove(tempCascade);

    return result;
}

bool FaceLoginDialog::saveFaceIDToDatabase(const QString &employeeId, const QImage &faceImage)
{
    // Vérifier si l'image est valide
    if (faceImage.isNull()) {
        qDebug() << "L'image de FaceID est invalide.";
        return false;
    }

    // Log de la taille de l'image avant la conversion
    qDebug() << "Image FaceID valide, taille:" << faceImage.size();

    // Convertir l'image en format binaire (QByteArray)
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);


    // Sauvegarder l'image dans le buffer (format PNG)
    if (!faceImage.save(&buffer, "PNG")) {
        qDebug() << "Échec de la conversion de l'image en PNG.";
        return false;
    }

    // Log de la taille du QByteArray après la conversion
    qDebug() << "Image convertie en QByteArray avec succès (" << imageData.size() << " octets)";

    // Créer une requête pour mettre à jour le FaceID dans la base de données
    QSqlQuery query;
    query.prepare("UPDATE employe SET face_id = :face_id WHERE id = :id");
    query.bindValue(":face_id", imageData);
    query.bindValue(":id", employeeId);

    // Vérifier l'exécution de la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'enregistrement du Face ID:" << query.lastError().text();
        return false;
    }

    qDebug() << "FaceID enregistré avec succès pour l'employé : " << employeeId;
    return true;
}
