#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QSortFilterProxyModel>
#include <QRandomGenerator>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QSqlRecord>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QGraphicsOpacityEffect>
#include <QCheckBox>
#include <QGraphicsProxyWidget>  // Add this line to fix the issue
#include <QPropertyAnimation>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QPdfDocument>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression> // à mettre en haut de ton fichier
#include <QDesktopServices>


const QString CLARIFAI_API_KEY = "35e6d6a543214a109e32dbc5b2d604c9";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    arduino(new Arduino(this)),
    dernierIdEmploye(-1)
{
    ui->setupUi(this); // D'abord configurer l'UI

    // Connexion des signaux Arduino
    connect(arduino, &Arduino::empreinteEnregistree,
            this, &MainWindow::onEmpreinteEnregistree);

    connect(arduino, &Arduino::messageStatut,
            this, [this](const QString &msg){
                ui->statusbar->showMessage(msg, 3000); // attention à "statusbar" vs "statusBar"
            });

    // Connexion automatique
    if (!arduino->connecter()) {
        QTimer::singleShot(3000, this, [this]() {
            arduino->connecter();
        });
    }

    afficherEmployes();


    // Connexion des signaux
    connect(arduino, &Arduino::empreinteEnregistree, this, &MainWindow::onEmpreinteEnregistree);
    // Exemple d’init si arduino déjà instancié :
    bool ok = connect(arduino, &Arduino::empreinteReconnue,
                      this,  &MainWindow::slotEmpreinteReconnue);
    qDebug() << "[CONNECT empreinteReconnue/MainWindow] Connect status: " << ok;    connect(arduino, &Arduino::empreinteNonTrouvee, this, &MainWindow::slotEmpreinteNonTrouvee);
    // Tu peux aussi connecter messageStatut() ou erreurCapteur() à un QLabel ou statusBar




    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherEmployes);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::trierEmployes);
    connect(ui->statt, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    //connect(ui->pushButton_faceId, &QPushButton::clicked, this, &MainWindow::on_pushButton_faceId_clicked);
    connect(ui->pushButton_appliquerModif, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_appliquerModif_clicked);

    ui->tabWidget->tabBar()->hide();

    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        afficherEmployes();

        qDebug() << "Passage à l'onglet Liste";
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Ajout";
    });
    connect(ui->login, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(4);
        qDebug() << "Retour à l'onglet login";
    });
    connect(ui->pushButton_8, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(5);
        chargementTodoList(); // 👈 Appelle cette fonction ici
    });
    connect(ui->pushButton_ajouter, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(6);
    });

    connect(ui->stat, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        afficherStatistiques();
    });

    connect(ui->pushButton_modifier, &QPushButton::clicked, this, [=]() {
        QModelIndex index = ui->tableView->currentIndex();

        if (!index.isValid()) {
            QMessageBox::warning(this, "Attention", "Veuillez d'abord sélectionner un employé.");
            return;
        }

        int row = index.row();
        auto model = ui->tableView->model();

        Employe e;

        e.setId(model->index(row, 1).data().toInt());
        e.setNom(model->index(row, 2).data().toString());
        e.setPrenom(model->index(row, 3).data().toString());

        QDate d = model->index(row, 4).data().toDate();
        e.setDateEmbauche(d.isValid() ? d : QDate::fromString(model->index(row, 4).data().toString(), "yyyy-MM-dd"));

        e.setPoste(model->index(row, 5).data().toString());
        e.setSalaire(model->index(row, 6).data().toDouble());
        e.setEmail(model->index(row, 7).data().toString());
        e.setSexe(model->index(row, 8).data().toString());
        e.setTelephone(model->index(row, 9).data().toString());
        e.setStatut(model->index(row, 10).data().toString());

        QStandardItemModel* m = qobject_cast<QStandardItemModel*>(model);
        if (m) {
            QStandardItem* photoItem = m->item(row, 0);
            if (photoItem) {
                QVariantMap datas = photoItem->data(Qt::UserRole + 1).toMap();
                e.setPdp(datas.value("pdp").toByteArray());
            }
        }

        setEmploye(e);

        ui->tabWidget->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Modification pour l'employé ID:" << e.getId();
    });

    connect(ui->comboRecherchePar, &QComboBox::currentTextChanged, this, &MainWindow::changerIndication);
    connect(ui->btnRechercher, &QPushButton::clicked, this, &MainWindow::rechercherEmployes);

    ui->comboRecherchePar->addItems({
        "Recherche par ID",
        "Recherche par Nom",
        "Recherche par Date d'embauche"
    });
    ui->labelIndication->setText("ID à rechercher :");
    ui->champRecherche->setPlaceholderText("e.g. 1 / Karim / 2024-03-28");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficherEmployes()
{
    disconnect(ui->tableView, &QTableView::clicked, this, &MainWindow::onPhotoClicked);

    QSqlQuery query("SELECT pdp, id, nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, statut FROM employe");

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "Photo", "ID", "Nom", "Prénom", "Date embauche",
        "Poste", "Salaire", "Email", "Sexe", "Téléphone", "Statut"
    });

    while (query.next()) {
        QList<QStandardItem*> rowItems;

        QStandardItem *photoItem = new QStandardItem();
        QByteArray imageData = query.value("pdp").toByteArray();

        qDebug() << "🔄 Chargement image : Taille =" << imageData.size();

        QVariantMap empData;
        empData["pdp"] = imageData;
        empData["id"] = query.value("id");
        empData["nom"] = query.value("nom");
        empData["prenom"] = query.value("prenom");
        empData["date_embauche"] = query.value("date_embauche");
        empData["poste"] = query.value("poste");
        empData["salaire"] = query.value("salaire");
        empData["email"] = query.value("email");
        empData["sexe"] = query.value("sexe");
        empData["telephone"] = query.value("telephone");
        empData["statut"] = query.value("statut");

        photoItem->setData(empData, Qt::UserRole + 1);

        if (!imageData.isEmpty()) {
            QPixmap pix;
            if (pix.loadFromData(imageData)) {
                pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QPixmap rounded = createRoundedPixmap(pix, 48);
                photoItem->setIcon(QIcon(rounded));
            } else {
                qDebug() << "⚠ Erreur : Impossible de charger l'image.";
            }
        } else {
            QPixmap def(":/img/default-user.png");
            if (!def.isNull()) {
                QPixmap rounded = createRoundedPixmap(def, 48);
                photoItem->setIcon(QIcon(rounded));
            }
        }
        photoItem->setText("");
        rowItems << photoItem;

        for (int i = 1; i < query.record().count(); ++i) {
            rowItems << new QStandardItem(query.value(i).toString());
        }

        model->appendRow(rowItems);
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setIconSize(QSize(64, 64));

    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::onPhotoClicked);
}

void MainWindow::on_pushButton_analysePDP_clicked() {
    static QPixmap originalImage;
    static QString fileName;

    // Récupérer l'image du bouton pushButton_pdp
    QIcon icon = ui->pushButton_pdp->icon();
    QPixmap pixmap = icon.pixmap(200, 200);

    if (pixmap.isNull()) {
        // Si aucune image n'est trouvée sur le bouton, ouvrir l'explorateur de fichiers
        fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une photo"), "", tr("Images (*.png *.jpg *.jpeg)"));
        if (fileName.isEmpty()) return;
        originalImage = QPixmap(fileName);
    } else {
        originalImage = pixmap;
    }

    if (originalImage.isNull()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image"));
        return;
    }

    QPixmap scaledImage = originalImage.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pushButton_pdp->setIcon(scaledImage);
    ui->pushButton_pdp2->setIcon(scaledImage);

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    originalImage.save(&buffer, "PNG");
    QString base64Image = imageData.toBase64();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://api.clarifai.com/v2/models/face-detection/outputs"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Key " + CLARIFAI_API_KEY.toUtf8());

    QJsonObject inputs;
    inputs["data"] = QJsonObject{{"image", QJsonObject{{"base64", base64Image}}}};
    QJsonObject requestBody;
    requestBody["inputs"] = QJsonArray{inputs};

    QJsonDocument doc(requestBody);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->post(request, data);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Analyse de l'image");
    dialog->setMinimumWidth(600);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *imageLabel = new QLabel(dialog);
    imageLabel->setPixmap(originalImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout->addWidget(imageLabel);

    QLabel *analysisLabel = new QLabel("Analyse en cours...", dialog);
    layout->addWidget(analysisLabel);

    QPushButton *selectButton = new QPushButton("Utiliser cette image", dialog);
    QPushButton *chooseAnotherButton = new QPushButton("Choisir une autre image", dialog);
    connect(selectButton, &QPushButton::clicked, [=]() {
        ui->pushButton_pdp->setIcon(scaledImage);
        ui->pushButton_pdp2->setIcon(scaledImage);
        dialog->accept();
    });
    connect(chooseAnotherButton, &QPushButton::clicked, [=]() {
        dialog->reject();
        QString newFileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une photo"), "", tr("Images (*.png *.jpg *.jpeg)"));
        if (!newFileName.isEmpty()) {
            fileName = newFileName;
            // Mettre à jour le bouton avec la nouvelle image avant de relancer l'analyse
            originalImage = QPixmap(fileName);
            QPixmap scaledImage = originalImage.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->pushButton_pdp->setIcon(scaledImage);
            ui->pushButton_pdp2->setIcon(scaledImage);
            on_pushButton_analysePDP_clicked(); // Relancer l'analyse
        }
    });
    layout->addWidget(selectButton);
    layout->addWidget(chooseAnotherButton);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            QJsonArray outputs = response.object()["outputs"].toArray();
            if (!outputs.isEmpty()) {
                QJsonArray regions = outputs[0].toObject()["data"].toObject()["regions"].toArray();
                int faceCount = regions.size();

                if (faceCount > 0) {
                    QPixmap tempImage = originalImage;
                    QPainter painter(&tempImage);
                    painter.setPen(QPen(Qt::red, 3));

                    for (const auto &region : regions) {
                        QJsonObject box = region.toObject()["region_info"].toObject()["bounding_box"].toObject();
                        double top = box["top_row"].toDouble() * tempImage.height();
                        double left = box["left_col"].toDouble() * tempImage.width();
                        double bottom = box["bottom_row"].toDouble() * tempImage.height();
                        double right = box["right_col"].toDouble() * tempImage.width();
                        QRect rect(QPoint(left, top), QPoint(right, bottom));
                        painter.drawRect(rect);
                    }
                    painter.end();
                    imageLabel->setPixmap(tempImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }

                if (faceCount == 1) {
                    analysisLabel->setText("✅ Un seul visage détecté. Photo validée.");
                } else if (faceCount > 1) {
                    analysisLabel->setText("⚠️ Plusieurs visages détectés. Photo non validée.");
                } else {
                    analysisLabel->setText("❌ Aucun visage détecté. Photo non validée.");
                }
            } else {
                analysisLabel->setText("❌ Erreur d'analyse Clarifai.");
            }
        } else {
            analysisLabel->setText("❌ Erreur réseau : " + reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });

    dialog->exec();
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    qDebug() << "Tentative d'ajout d'un employé...";

    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QDate date_embauche = ui->date_embauche->date();
    QString poste = ui->poste->currentText();
    QString email = ui->email->text();
    QString sexe = ui->sexe->currentText();
    QString telephone = ui->telephone->text();
    QString statut = ui->statut->text();

    // Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    bool ok;
    int salaire = ui->salaire->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide.");
        return;
    }

    QByteArray imageData;
    if (!cheminImagePDP.isEmpty()) {
        QPixmap pixmap(cheminImagePDP);
        if (!pixmap.isNull()) {
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            if (pixmap.save(&buffer, "PNG")) {
                qDebug() << "✅ Image convertie en QByteArray avec succès (" << imageData.size() << " octets)";
            } else {
                qDebug() << "❌ Échec de la conversion de l'image.";
            }
        } else {
            qDebug() << "❌ Chemin de l'image invalide.";
        }
    }

    // Si c'est un chef technicien, vérifier le FaceID
    if (poste == "chef technicien" && faceIdTemp.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "L'enregistrement du FaceID est obligatoire pour un chef technicien.");
        return;
    }

    // Création de l'objet employé avec face_id et empreinte
    Employe employe(nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, imageData, statut, faceIdTemp, empreinteTemp);

    // Tentative d'ajout de l'employé
    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");

        // Clear tous les champs
        ui->nom->clear();
        ui->prenom->clear();
        ui->date_embauche->setDate(QDate::currentDate());
        ui->poste->setCurrentIndex(0);
        ui->email->clear();
        ui->sexe->setCurrentIndex(0);
        ui->telephone->clear();
        ui->statut->clear();
        ui->salaire->clear();

        // Réinitialiser les boutons de photo de profil
        ui->pushButton_pdp->setIcon(QIcon());  // Enlever l'icône
        ui->pushButton_pdp2->setIcon(QIcon()); // Enlever l'icône
        ui->pushButton_pdp->setStyleSheet("");  // Réinitialiser le style
        ui->pushButton_pdp2->setStyleSheet(""); // Réinitialiser le style

        // Réinitialiser le chemin de l'image
        cheminImagePDP.clear();

        // Réinitialiser le Face ID temporaire
        faceIdTemp.clear();

        // Rafraîchir le tableau des employés
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
    }
}
void MainWindow::on_pushButton_choisirImage_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_pdp->setIcon(QIcon(pixmap));
            ui->pushButton_pdp->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2->setIconSize(QSize(60, 60));
            cheminImagePDP = filePath;
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
        }
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int id = selection.first().siblingAtColumn(1).data(Qt::DisplayRole).toInt();
    qDebug() << "ID sélectionné :" << id;

    if (id <= 0) {
        QMessageBox::critical(this, "Erreur", "ID invalide (" + QString::number(id) + ") !");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Êtes-vous sûr de vouloir supprimer cet employé ?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    Employe employe;
    if (employe.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'employé !");
    }
}

void MainWindow::on_pushButton_exporterPDF_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "liste_employes.pdf", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(120);
    writer.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    QAbstractItemModel *model = ui->tableView->model();
    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    int y = 100;
    int x = 50;
    const int rowHeight = 200;
    const int photoSize = 100;

    // 🔤 Normalisation des en-têtes
    auto normalize = [](const QString &text) -> QString {
        QRegularExpression re("[\\x{0300}-\\x{036F}]");
        QString cleaned = text.normalized(QString::NormalizationForm_D);
        return cleaned.remove(re).toLower().simplified();
    };

    // 📦 Détection des colonnes
    QMap<QString, int> cols;
    for (int c = 0; c < colCount; ++c) {
        QString head = normalize(model->headerData(c, Qt::Horizontal).toString());
        if (head.contains("photo") || head.contains("pdp")) cols["photo"] = c;
        else if (head.contains("id")) cols["id"] = c;
        else if (head == "nom") cols["nom"] = c;
        else if (head == "prenom") cols["prenom"] = c;
        else if (head.contains("date")) cols["date_embauche"] = c;
        else if (head.contains("poste")) cols["poste"] = c;
        else if (head.contains("salaire")) cols["salaire"] = c;
        else if (head.contains("email")) cols["email"] = c;
        else if (head.contains("sexe")) cols["sexe"] = c;
        else if (head.contains("telephone")) cols["telephone"] = c;
        else if (head.contains("statut")) cols["statut"] = c;
    }

    if (cols.size() < 10) {
        painter.end();
        QMessageBox::critical(this, "Erreur", "Certaines colonnes sont introuvables !");
        return;
    }

    // 📝 Titre stylisé du document
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(220, 230, 250));  // Fond doux pour le titre
    painter.drawRoundedRect(QRect(30, 20, writer.width() - 60, 60), 10, 10);

    // Texte centré
    painter.setFont(QFont("Georgia", 20, QFont::Bold));
    painter.setPen(QColor(25, 50, 120));
    painter.drawText(QRect(30, 20, writer.width() - 60, 60), Qt::AlignCenter, "📋 Liste des Employés");
    painter.setFont(QFont("Arial", 11));


    // 📄 Parcours des employés
    for (int row = 0; row < rowCount; ++row) {
        int posX = x;

        // 🎨 Fond alterné
        QColor bg = (row % 2 == 0) ? QColor(250, 250, 255) : QColor(240, 245, 255);
        painter.fillRect(QRect(30, y - 20, writer.width() - 60, rowHeight + 20), bg);

        // 🖼️ Photo
        QVariant cellData = model->data(model->index(row, cols["photo"]), Qt::UserRole + 1);
        QPixmap pixmap;
        if (cellData.canConvert<QVariantMap>()) {
            QByteArray imgData = cellData.toMap()["pdp"].toByteArray();
            pixmap.loadFromData(imgData);
        }

        if (!pixmap.isNull()) {
            QPixmap circle(photoSize, photoSize);
            circle.fill(Qt::transparent);
            QPainter circlePainter(&circle);
            circlePainter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, photoSize, photoSize);
            circlePainter.setClipPath(path);
            circlePainter.drawPixmap(0, 0, pixmap.scaled(photoSize, photoSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            painter.drawPixmap(posX, y, circle);
        }

        posX += photoSize + 30;

        // 📋 Données avec icônes
        QString rawDate = model->data(model->index(row, cols["date_embauche"])).toString();
        QDate date = QDate::fromString(rawDate.left(10), "yyyy-MM-dd");
        QString dateStr = date.isValid() ? date.toString("yyyy-MM.dd") : rawDate;

        QStringList lignes;
        lignes << QString("🆔 ID : %1").arg(model->data(model->index(row, cols["id"])).toString());
        lignes << QString("👤 Nom : %1 %2")
                      .arg(model->data(model->index(row, cols["prenom"])).toString())
                      .arg(model->data(model->index(row, cols["nom"])).toString());
        lignes << QString("📅 Embauche : %1").arg(dateStr);
        lignes << QString("💼 Poste : %1").arg(model->data(model->index(row, cols["poste"])).toString());
        lignes << QString("💰 Salaire : %1 DT")
                      .arg(QString::number(model->data(model->index(row, cols["salaire"])).toDouble(), 'f', 2));
        lignes << QString("📧 Email : %1").arg(model->data(model->index(row, cols["email"])).toString());
        lignes << QString("📞 Téléphone : %1").arg(model->data(model->index(row, cols["telephone"])).toString());
        lignes << QString("🔘 Sexe : %1").arg(model->data(model->index(row, cols["sexe"])).toString());
        lignes << QString("📌 Statut : %1").arg(model->data(model->index(row, cols["statut"])).toString());

        painter.setPen(Qt::black);
        QFont lineFont("Arial", 10);
        painter.setFont(lineFont);
        for (int i = 0; i < lignes.size(); ++i) {
            painter.drawText(posX, y + 20 + i * 16, lignes[i]);
        }

        y += rowHeight;

        // Nouvelle page
        if (y > writer.height() - 180) {
            writer.newPage();
            y = 100;
        }
    }

    // 🦶 Pied de page
    painter.setPen(QColor(80, 80, 80));
    painter.setFont(QFont("Arial", 9));
    QString footer = QString("📄 Généré le %1  |  Employés : %2")
                         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
                         .arg(rowCount);
    painter.drawText(QRect(0, writer.height() - 40, writer.width(), 20), Qt::AlignCenter, footer);

    painter.end();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    QMessageBox::information(this, "PDF généré", "🎉 La liste des employés a été exportée avec succès !");
}
void MainWindow::trierEmployes() {
    int columnIndex = ui->tableView->currentIndex().column();

    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return;
    }

    // Récupérer le modèle source (StandardItemModel) même si on a un proxy model
    QStandardItemModel *sourceModel = nullptr;
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

    if (proxyModel) {
        sourceModel = qobject_cast<QStandardItemModel*>(proxyModel->sourceModel());
    } else {
        sourceModel = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    }

    if (!sourceModel) {
        QMessageBox::warning(this, "Erreur", "Impossible de trier ce type de modèle");
        return;
    }

    // Si on n'a pas de proxy model, en créer un nouveau
    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(sourceModel);
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    }

    // Configurer le proxy model pour le tri numérique
    proxyModel->setSortRole(Qt::UserRole);

    // Pour chaque ligne, définir la valeur numérique dans le rôle UserRole
    for (int row = 0; row < sourceModel->rowCount(); ++row) {
        QStandardItem *item = sourceModel->item(row, columnIndex);
        if (item) {
            QString text = item->text();
            bool ok;
            double numericValue = text.toDouble(&ok);
            if (ok) {
                item->setData(numericValue, Qt::UserRole);
            } else {
                item->setData(text, Qt::UserRole);
            }
        }
    }

    // Appliquer le tri
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    ui->tableView->setModel(proxyModel);
}
void MainWindow::afficherStatistiques() {
    // Création de la scène
    QGraphicsScene *scene = new QGraphicsScene(this);
    int chartWidth = 900;  // Augmenter la largeur totale

    // Grand titre
    QLabel *titleLabel = new QLabel("📊 Statistiques des employés");
    titleLabel->setStyleSheet("font-size: 28pt; font-weight: bold; color: #2c3e50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedWidth(chartWidth);
    QGraphicsProxyWidget *titleProxy = scene->addWidget(titleLabel);
    titleProxy->setPos(0, 0);

    // =================== Haute Section : Répartition par Poste ===================
    QSqlQuery query1("SELECT poste, COUNT(*) AS effectif FROM employe GROUP BY poste");
    QPieSeries *posteSeries = new QPieSeries();
    int totalEmployes = 0;

    while (query1.next()) {
        int count = query1.value(1).toInt();
        totalEmployes += count;
        posteSeries->append(query1.value(0).toString(), count);
    }

    for (QPieSlice *slice : posteSeries->slices()) {
        slice->setExploded(true);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *posteChart = new QChart();
    posteChart->addSeries(posteSeries);
    posteChart->setTitle("📋 Répartition par poste");
    posteChart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    posteChart->setAnimationOptions(QChart::SeriesAnimations);
    posteChart->legend()->setAlignment(Qt::AlignBottom);
    posteChart->legend()->setBackgroundVisible(false);
    posteChart->setBackgroundBrush(QColor("#ecf0f1"));

    QChartView *posteChartView = new QChartView(posteChart);
    posteChartView->setRenderHint(QPainter::Antialiasing);
    posteChartView->setFixedSize(450, 350); // Ajustement de taille

    // Position pour le graphique par poste
    QGraphicsProxyWidget *posteProxy = scene->addWidget(posteChartView);
    posteProxy->setPos(0, 100); // Position en haut à gauche

    // =================== Basse Section : Répartition par Sexe ===================
    QSqlQuery query2("SELECT sexe FROM employe");
    int hommes = 0, femmes = 0;

    while (query2.next()) {
        QString sexe = query2.value(0).toString().toLower();
        if (sexe.contains("homme")) {
            hommes++;
        } else if (sexe.contains("femme")) {
            femmes++;
        }
    }

    QPieSeries *sexeSeries = new QPieSeries();
    sexeSeries->append("👨 Hommes", hommes);
    sexeSeries->append("👩 Femmes", femmes);

    for (QPieSlice *slice : sexeSeries->slices()) {
        slice->setExploded(true);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *sexeChart = new QChart();
    sexeChart->addSeries(sexeSeries);
    sexeChart->setTitle("👥 Répartition par sexe");
    sexeChart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    sexeChart->setAnimationOptions(QChart::SeriesAnimations);
    sexeChart->legend()->setAlignment(Qt::AlignBottom);
    sexeChart->legend()->setBackgroundVisible(false);
    sexeChart->setBackgroundBrush(QColor("#ecf0f1"));

    QChartView *sexeChartView = new QChartView(sexeChart);
    sexeChartView->setRenderHint(QPainter::Antialiasing);
    sexeChartView->setFixedSize(450, 350);

    // Position pour le graphique par sexe
    QGraphicsProxyWidget *sexeProxy = scene->addWidget(sexeChartView);
    sexeProxy->setPos(460, 100); // Position à droite sous le titre

    // Appliquer la scène au QGraphicsView
    scene->setSceneRect(0, 0, chartWidth + 20, 500);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setSceneRect(scene->sceneRect());
    // Animation d'apparition : ajouter une animation de fondu
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->graphicsView);
    ui->graphicsView->setGraphicsEffect(effect);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(400);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
void MainWindow::changerIndication()
{
    QString critere = ui->comboRecherchePar->currentText();

    if (critere == "Recherche par ID") {
        ui->labelIndication->setText("ID à rechercher :");
    } else if (critere == "Recherche par Nom") {
        ui->labelIndication->setText("Nom à rechercher :");
    } else if (critere == "Recherche par Date d'embauche") {
        ui->labelIndication->setText("Date (format YYYY-MM-DD) :");
    }
}
void MainWindow::rechercherEmployes()
{
    QString critere = ui->comboRecherchePar->currentText();
    QString valeur = ui->champRecherche->text().trimmed();

    if (valeur.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez saisir une valeur !");
        return;
    }

    QString requete = "SELECT pdp, id, nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, statut FROM employe WHERE ";
    QVariant bindValue;

    if (critere == "Recherche par ID") {
        requete += "id = ?";
        bindValue = valeur;
    } else if (critere == "Recherche par Nom") {
        requete += "LOWER(nom) LIKE LOWER(?)";
        bindValue = "%" + valeur + "%";
    } else if (critere == "Recherche par Date d'embauche") {
        QDate date = QDate::fromString(valeur, "yyyy-MM-dd");
        if (!date.isValid()) {
            QMessageBox::warning(this, "Format incorrect", "La date doit être au format YYYY-MM-DD");
            return;
        }
        requete += "date_embauche = ?";
        bindValue = date;
    }

    QSqlQuery query;
    query.prepare(requete);
    query.bindValue(0, bindValue);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "Photo", "ID", "Nom", "Prénom", "Date embauche",
        "Poste", "Salaire", "Email", "Sexe", "Téléphone", "Statut"
    });

    while (query.next()) {
        QList<QStandardItem*> rowItems;
        QStandardItem *photoItem = new QStandardItem();

        QByteArray imageData = query.value("pdp").toByteArray();

        QVariantMap empData;
        empData["pdp"] = imageData;
        empData["id"] = query.value("id");
        empData["nom"] = query.value("nom");
        empData["prenom"] = query.value("prenom");
        empData["date_embauche"] = query.value("date_embauche");
        empData["poste"] = query.value("poste");
        empData["salaire"] = query.value("salaire");
        empData["email"] = query.value("email");
        empData["sexe"] = query.value("sexe");
        empData["telephone"] = query.value("telephone");
        empData["statut"] = query.value("statut");

        photoItem->setData(empData, Qt::UserRole + 1);

        if (!imageData.isEmpty()) {
            QPixmap pix;
            if (pix.loadFromData(imageData)) {
                pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QPixmap rounded = createRoundedPixmap(pix, 48);  // Assure-toi que createRoundedPixmap existe
                photoItem->setIcon(QIcon(rounded));
            }
        } else {
            QPixmap def(":/img/default-user.png");
            QPixmap rounded = createRoundedPixmap(def, 48);
            photoItem->setIcon(QIcon(rounded));
        }

        photoItem->setText("");
        rowItems << photoItem;

        for (int i = 1; i < query.record().count(); ++i) {
            rowItems << new QStandardItem(query.value(i).toString());
        }

        model->appendRow(rowItems);
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setIconSize(QSize(64, 64));

    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::onPhotoClicked);
}
void MainWindow::setEmploye(const Employe &employe)
{
    this->employe = employe;

    ui->nom_2->setText(employe.getNom());
    ui->prenom_2->setText(employe.getPrenom());
    ui->date_embauche_2->setDate(employe.getDateEmbauche());
    ui->poste_2->setText(employe.getPoste());
    ui->salaire_2->setText(QString::number(employe.getSalaire()));
    ui->email_2->setText(employe.getEmail());
    ui->sexe_2->setCurrentText(employe.getSexe());
    ui->telephone_2->setText(employe.getTelephone());
    ui->statut_2->setText(employe.getStatut());

    if (!employe.getPdp().isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(employe.getPdp());

        if (!pixmap.isNull()) {
            ui->pushButton_pdp_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp_2->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2_2->setIconSize(QSize(50, 50));
        } else {
            qDebug() << "❌ Erreur : Impossible de charger l'image depuis les données binaires.";
        }
    } else {
        qDebug() << "⚠️ Aucune donnée d'image disponible.";
    }

    ui->tabWidget->setCurrentIndex(3);
}

Employe MainWindow::getEmployeModifie() const
{
    Employe employe;

    employe.setNom(ui->nom_2->text());
    employe.setPrenom(ui->prenom_2->text());
    employe.setDateEmbauche(ui->date_embauche_2->date());
    employe.setPoste(ui->poste_2->text());
    employe.setSalaire(ui->salaire_2->text().toDouble());
    employe.setEmail(ui->email_2->text());
    employe.setSexe(ui->sexe_2->currentText());
    employe.setTelephone(ui->telephone_2->text());
    employe.setStatut(ui->statut_2->text());

    employe.setPdp(this->employe.getPdp());
    employe.setId(this->employe.getId());

    return employe;
}

void MainWindow::on_pushButton_modifierPhoto_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_pdp_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp_2->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2_2->setIconSize(QSize(50, 50));
            ui->pushButton_pdp2_2->setStyleSheet("border-radius: 25px; border: none;");

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");
            employe.setPdp(byteArray);
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image sélectionnée.");
        }
    }
}

void MainWindow::on_pushButton_enregistrer_clicked()
{
    if (ui->nom_2->text().isEmpty() || ui->prenom_2->text().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis !");
        return;
    }

    Employe e = getEmployeModifie();

    if (e.modifier(e.getId())) {
        QMessageBox::information(this, "Succès", "✅ Modification enregistrée !");
        afficherEmployes();
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "❌ La modification a échoué.");
    }
}

void MainWindow::on_pushButton_appliquerModif_clicked()
{
    if (ui->nom_2->text().isEmpty() || ui->prenom_2->text().isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    Employe updated;
    updated.setId(employe.getId());
    updated.setNom(ui->nom_2->text());
    updated.setPrenom(ui->prenom_2->text());
    updated.setDateEmbauche(ui->date_embauche_2->date());
    updated.setPoste(ui->poste_2->text());
    updated.setSalaire(ui->salaire_2->text().toDouble());
    updated.setEmail(ui->email_2->text());
    updated.setSexe(ui->sexe_2->currentText());
    updated.setTelephone(ui->telephone_2->text());
    updated.setStatut(ui->statut_2->text());

    updated.setPdp(employe.getPdp());

    if (updated.modifier(updated.getId())) {
        QMessageBox::information(this, "Succès", "✅ Employé modifié avec succès !");
        afficherEmployes();
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "❌ La mise à jour a échoué !");
    }
}

QPixmap createRoundedPixmap(const QPixmap &src, int size)
{
    QPixmap dest(size, size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, size, size, src);
    return dest;
}

void MainWindow::afficherDetailsEmploye(const QModelIndex &index)
{
    if (index.column() != 0) return;

    QVariant empData = index.data(Qt::UserRole + 1);
    if (!empData.isValid()) {
        qDebug() << "❌ Aucune donnée employé trouvée";
        return;
    }

    QVariantMap employee = empData.toMap();
    qDebug() << "📸 Affichage des détails de l'employé:" << employee;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Détails de l'employé");
    dialog->setFixedSize(350, 500);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // 📷 Affichage de la photo
    QLabel *photoLabel = new QLabel();
    QPixmap photo;
    QByteArray photoData = employee["pdp"].toByteArray();

    qDebug() << "Taille des données photo:" << photoData.size();

    if (!photoData.isEmpty() && photo.loadFromData(photoData)) {
        photo = photo.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        photo.load(":/img/default-user.png");
        photo = photo.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    photoLabel->setPixmap(photo);
    photoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(photoLabel);

    // 🔹 Infos employé
    QFrame *infoFrame = new QFrame();
    QFormLayout *formLayout = new QFormLayout(infoFrame);
    formLayout->setSpacing(5);

    // Correction des clés pour correspondre aux données stockées
    QStringList fields = {"ID", "Nom", "Prénom", "Poste", "Salaire", "Email", "Téléphone"};
    QStringList keys = {"id", "nom", "prenom", "poste", "salaire", "email", "telephone"};

    for (int i = 0; i < fields.size(); ++i) {
        QLabel *label = new QLabel(employee[keys[i]].toString());
        label->setStyleSheet("font: 10pt; padding: 2px;");

        QLabel *title = new QLabel(fields[i] + ":");
        title->setStyleSheet("font-weight: bold; font: 10pt; color: #2c3e50;");

        formLayout->addRow(title, label);
    }

    infoFrame->setStyleSheet("background: #f8f9fa; border-radius: 10px; padding: 10px;");
    layout->addWidget(infoFrame);

    // 📄 Ajout d'un bouton pour générer le PDF
    QPushButton *pdfButton = new QPushButton("Exporter en PDF");
    layout->addWidget(pdfButton);

    connect(pdfButton, &QPushButton::clicked, this, [this, employee]() {
        genererPDF(employee);
    });

    dialog->exec();
    delete dialog;
}

void MainWindow::genererPDF(const QVariantMap &employee)
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Enregistrer PDF", "details_employe.pdf", "Fichiers PDF (*.pdf)");
    if (filename.isEmpty()) return;

    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);
    writer.setPageMargins(QMarginsF(40, 40, 40, 40));

    QPainter painter;
    if (!painter.begin(&writer)) return;

    int pageWidth = writer.width();
    int pageHeight = writer.height();
    int margin = 100;

    // Arrière-plan
    painter.fillRect(0, 0, pageWidth, pageHeight, QColor(252, 252, 252));
    painter.fillRect(0, 0, pageWidth, 350, QColor(245, 245, 245));

    // PHOTO À GAUCHE
    int photoX = margin;
    int photoY = 50;
    int photoSize = 280;

    QByteArray photoData = employee["pdp"].toByteArray();
    if (!photoData.isEmpty()) {
        QPixmap photo;
        if (photo.loadFromData(photoData)) {
            QPixmap rounded(photoSize, photoSize);
            rounded.fill(Qt::transparent);

            QPainter roundPainter(&rounded);
            roundPainter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, photoSize, photoSize);
            roundPainter.setClipPath(path);

            QPixmap scaled = photo.scaled(photoSize, photoSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            roundPainter.drawPixmap(0, 0, scaled);
            roundPainter.end();

            painter.drawPixmap(photoX, photoY, rounded);
        }
    }

    // Titre à droite
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(QRect(photoX + photoSize + 50, 140, pageWidth - photoSize - 200, 80), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("Détails de l'employé"));

    // Carte Informations
    int infoStartY = 370;
    int cardWidth = pageWidth - (2 * margin);
    int cardHeight = 1000;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(220, 220, 220, 100));
    painter.drawRoundedRect(margin + 5, infoStartY + 5, cardWidth, cardHeight, 15, 15);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(margin, infoStartY, cardWidth, cardHeight, 15, 15);

    // Détails
    int x = margin + 80;
    int y = infoStartY + 80;
    int lineHeight = 135; // ✅ Espacement augmenté ici

    QStringList fields = {
        QStringLiteral("ID"), QStringLiteral("Nom"), QStringLiteral("Prénom"),
        QStringLiteral("Poste"), QStringLiteral("Salaire"), QStringLiteral("Email"),
        QStringLiteral("Tél")
    };
    QStringList keys = {"id", "nom", "prenom", "poste", "salaire", "email", "telephone"};

    for (int i = 0; i < fields.size(); ++i) {
        if (employee.contains(keys[i])) {
            // Label
            painter.setPen(QColor(50, 50, 50));
            painter.setFont(QFont("Arial", 18, QFont::Bold));
            QRect labelRect(x, y, 300, 60);
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, fields[i] + " :");

            // Valeur
            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 18));
            QString value = employee[keys[i]].toString();
            if (fields[i] == QStringLiteral("Salaire"))
                value += QStringLiteral(" DT");

            QRect valueRect(x + 320, y, cardWidth - 400, 60);
            painter.drawText(valueRect, Qt::AlignLeft | Qt::AlignVCenter, value);

            // Ligne de séparation
            if (i < fields.size() - 1) {
                painter.setPen(QPen(QColor(200, 200, 200), 2));
                painter.drawLine(x - 30, y + lineHeight - 35, x + cardWidth - 100, y + lineHeight - 35);
            }

            y += lineHeight;
        }
    }

    // Pied de page
    painter.setPen(QColor(90, 90, 90));
    painter.setFont(QFont("Arial", 14));
    QString dateStr = "Document généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm");

    // ... pied de page ...
    painter.drawText(QRect(margin, pageHeight - 60, cardWidth, 40), Qt::AlignCenter, dateStr);

    // 🔚 Fin du dessin
    painter.end();

    // 🚀 Ouvre automatiquement le fichier PDF généré
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

    QMessageBox::information(nullptr, "Succès", "✅ Le PDF a été généré avec succès !");

}


void MainWindow::saveImageToDatabase(const QPixmap &pixmap) {
    if (pixmap.isNull()) {
        qDebug() << "❌ Image invalide";
        return;
    }

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    if (!pixmap.save(&buffer, "PNG")) {
        qDebug() << "❌ Échec de la conversion de l'image";
        return;
    }
    buffer.close();

    QSqlQuery query;
    query.prepare("UPDATE employe SET pdp = :imageData WHERE id = :id");
    query.bindValue(":imageData", imageData);
    query.bindValue(":id", employe.getId());

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la mise à jour de l'image:" << query.lastError().text();
    } else {
        qDebug() << "✅ Image mise à jour avec succès";
    }
}

void MainWindow::onPhotoClicked(const QModelIndex &index) {
    if (index.column() != 0) return;

    QVariant empData = index.data(Qt::UserRole + 1);
    if (!empData.isValid()) {
        qDebug() << "❌ Aucune donnée employé trouvée";
        return;
    }

    QVariantMap employee = empData.toMap();
    qDebug() << "📸 Affichage des détails de l'employé:" << employee;

    afficherDetailsEmploye(index);
}
void MainWindow::chargementTodoList()
{
    if (ui->tableTodo->model())
        delete ui->tableTodo->model();

    QSqlQuery query(R"(
        SELECT
            m.id,
            m.type,
            m.description,
            m.statut,
            m.date_resolution AS date_resolution
        FROM maintenance m
    )");

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ "✔", "🔧 Type", "📝 Description", "📌 Statut", "📅 Résolution" });

    while (query.next()) {
        QList<QStandardItem *> row;

        // Case à cocher (colonne 0)
        QStandardItem *checkItem = new QStandardItem();
        checkItem->setCheckable(true);
        QString statut = query.value("statut").toString();
        checkItem->setCheckState(statut.toLower() == "traitée" ? Qt::Checked : Qt::Unchecked);
        checkItem->setData(query.value("id").toInt()); // pour update
        row << checkItem;

        // Type, description
        row << new QStandardItem(query.value("type").toString());
        row << new QStandardItem(query.value("description").toString());

        // Statut (badge coloré)
        QStandardItem *statutItem = new QStandardItem(statut);
        QString couleur = "#ffffff";
        if (statut.toLower() == "non traitée") couleur = "#ffeaa7";
        else if (statut.toLower() == "en cours") couleur = "#d6eaff";
        else if (statut.toLower() == "traitée") couleur = "#d4edda";
        statutItem->setBackground(QColor(couleur));
        row << statutItem;

        // Date résolution
        row << new QStandardItem(query.value("date_resolution").toString());

        model->appendRow(row);
    }

    ui->tableTodo->setModel(model);

    for (int i = 0; i < model->columnCount(); ++i)
        ui->tableTodo->setColumnWidth(i, 160);

    // Style
    ui->tableTodo->setStyleSheet(R"(
        QTableView {
            background-color: #f4f6f8;
            font-size: 14px;
            alternate-background-color: #ffffff;
            selection-background-color: #aee1f9;
            border: 1px solid #ccc;
            border-radius: 8px;
        }
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            font-weight: bold;
            font-size: 14px;
            padding: 6px;
        }
        QTableView::item {
            padding: 5px;
        }
        QTableView::item:hover {
            background-color: #eaf4fb;
        }
    )");

    ui->tableTodo->horizontalHeader()->setStretchLastSection(true);
    ui->tableTodo->setAlternatingRowColors(true);
    ui->tableTodo->verticalHeader()->setVisible(false);
    ui->tableTodo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTodo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Animation à l'apparition
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->tableTodo);
    ui->tableTodo->setGraphicsEffect(effect);
    QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity");
    fade->setDuration(500);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    // Mise à jour du statut en BDD quand on coche
    connect(model, &QStandardItemModel::itemChanged, this, [=](QStandardItem *item) {
        if (item->column() == 0) {
            int id = item->data().toInt();
            bool isChecked = item->checkState() == Qt::Checked;
            QString newStatus = isChecked ? "traitée" : "en cours";

            QSqlQuery update;
            update.prepare("UPDATE maintenance SET statut = :statut WHERE id = :id");
            update.bindValue(":statut", newStatus);
            update.bindValue(":id", id);

            if (!update.exec()) {
                qDebug() << "❌ Erreur update maintenance :" << update.lastError().text();
            } else {
                model->setData(model->index(item->row(), 3), newStatus);
                // Optionnel : changer la couleur du badge
                QColor color = (newStatus == "traitée") ? QColor("#d4edda") : QColor("#d6eaff");
                model->item(item->row(), 3)->setBackground(color);
            }
        }
    });

    qDebug() << "✅ To-Do list chargée avec badge et checkbox.";
}

void MainWindow::toutMarquerCommeTermine()
{
    QSqlQuery update("UPDATE maintenance SET statut = 'traitée' WHERE statut <> 'traitée' OR statut IS NULL");
    if (!update.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de tout marquer comme terminé !");
    } else {
        chargementTodoList();
    }
}

void MainWindow::on_btnAccederTodo_clicked()
{
    FaceLoginDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->tabWidget->setCurrentIndex(5);  // Index de l'onglet Todo List
        chargementTodoList();
    }
}
void MainWindow::on_pushButton_faceId_clicked()
{
    // Récupérer le dernier ID inséré dans la table employe
    QSqlQuery query;
    query.prepare("SELECT MAX(id) FROM employe");

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer l'ID de l'employé");
        return;
    }

    QString employeeId = query.value(0).toString();
    if (employeeId.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord ajouter l'employé");
        return;
    }

    // Appeler la fonction d'enregistrement du FaceID
    if (FaceLoginDialog::registerFaceID(this, employeeId)) {
        QMessageBox::information(this, "Succès", "FaceID enregistré avec succès");
        // Mettre à jour l'icône du bouton pour indiquer que le FaceID est enregistré
        ui->pushButton_faceId->setStyleSheet(ui->pushButton_faceId->styleSheet() + "\nbackground-color: #4CAF50;");
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'enregistrement du FaceID");
    }
}
void MainWindow::on_btnVerifier_clicked()
{
    QString nomSaisi = ui->lineEdit_nom->text().trimmed();
    QSqlQuery query;
    query.prepare("SELECT id, nom, face_id FROM employes WHERE nom = :nom");
    query.bindValue(":nom", nomSaisi);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

}
void MainWindow::on_cvButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir un CV", "", "Fichiers texte (*.txt)");
    if (filePath.isEmpty()) return;

    QString extractedText;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        extractedText = in.readAll();
        file.close();
    }

    if (extractedText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le fichier est vide ou non lisible.");
        return;
    }

    // === Extraction des infos ===
    QString nom, prenom, email, tel, sexe, poste;

    // 🔍 Email & téléphone
    QRegularExpression reEmail(R"(([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");
    QRegularExpression rePhone(R"((\+?\d[\d\s\-]{6,}\d))");

    QRegularExpressionMatch matchEmail = reEmail.match(extractedText);
    if (matchEmail.hasMatch()) email = matchEmail.captured(1);

    QRegularExpressionMatch matchPhone = rePhone.match(extractedText);
    if (matchPhone.hasMatch()) tel = matchPhone.captured(1);

    // 🔤 Nom / prénom (simple heuristique)
    QStringList lines = extractedText.split("\n", Qt::SkipEmptyParts);
    if (!lines.isEmpty()) {
        QStringList names = lines.first().split(" ", Qt::SkipEmptyParts);
        if (names.size() >= 2) {
            prenom = names.first();
            nom = names.last();
        }
    }

    // 🔮 Détection sexe
    QString lowerText = extractedText.toLower();
    if (lowerText.contains("née le") || lowerText.contains("madame") || lowerText.contains("elle")) {
        sexe = "Femme";
    } else if (lowerText.contains("né le") || lowerText.contains("monsieur") || lowerText.contains("il")) {
        sexe = "Homme";
    }

    // 🎯 Détection poste (keywords)
    QStringList postes = {
        "développeur", "developpeur", "ingénieur", "ingenieur",
        "technicien", "chef de projet", "designer", "analyste",
        "data scientist", "administrateur", "manager"
    };

    for (const QString &mot : postes) {
        if (lowerText.contains(mot)) {
            poste = mot.left(1).toUpper() + mot.mid(1);  // Mettre en maj la première lettre
            break;
        }
    }

    // ✅ Remplir les champs
    ui->nom->setText(nom);
    ui->prenom->setText(prenom);
    ui->email->setText(email);
    ui->telephone->setText(tel);
    ui->sexe->setCurrentText(sexe);
    ui->poste->setCurrentText(poste);


    // ✅ Message OK
    QMessageBox::information(this, "Extraction réussie", "✅ Données extraites avec succès !");
}


//empreinte

void MainWindow::on_btnAjouterEmpreinte_clicked()
{
    // 1. Récupérer le dernier ID employé inséré
    QSqlQuery query;
    if (!query.exec("SELECT id FROM employe ORDER BY id DESC LIMIT 1")) {
        QString errorMsg = "Impossible de récupérer le dernier ID employé:\n" + query.lastError().text();

        // Solution alternative pour les drivers qui ne supportent pas LIMIT
        if (!query.exec("SELECT MAX(id) FROM employe")) {
            QMessageBox::critical(this, "Erreur SQL", errorMsg);
            return;
        }
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Avertissement", "Aucun employé trouvé dans la base de données");
        return;
    }

    int dernierId = query.value(0).toInt();
    if (dernierId <= 0) {
        QMessageBox::warning(this, "Avertissement", "ID employé invalide");
        return;
    }

    // 2. Vérifier la connexion Arduino
    if (!arduino->estConnecte()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Arduino non connecté",
            "Le capteur d'empreintes n'est pas connecté. Voulez-vous essayer de vous reconnecter ?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes && !arduino->connecter()) {
            QMessageBox::critical(this, "Erreur", "Échec de la connexion au capteur");
            return;
        }
        else if (reply == QMessageBox::No) {
            return;
        }
    }

    // 3. Préparer l'interface utilisateur
    ui->btnAjouterEmpreinte->setEnabled(false);
    ui->btnAjouterEmpreinte->setText("Enregistrement en cours...");
    QApplication::processEvents(); // Forcer la mise à jour de l'UI

    // 4. Afficher les instructions
    QMessageBox::information(this,
                             "Enregistrement d'empreinte",
                             QString("Prêt à enregistrer l'empreinte pour l'employé ID: %1\n\n"
                                     "Veuillez placer votre doigt sur le capteur.\n"
                                     "Vous devrez le placer deux fois pour une bonne capture.").arg(dernierId));

    // 5. Envoyer la commande à l'Arduino
    arduino->demarrerEnregistrement(dernierId); // Utiliser la méthode correcte

    // Le reste est géré par:
    // - onEmpreinteEnregistree() en cas de succès
    // - onErreurArduino() en cas d'échec
}
void MainWindow::onEmpreinteEnregistree(int idEmploye)
{
    // 1. Vérifier que l'ID correspond au dernier ID dans la base de données
    QSqlQuery verifQuery;
    verifQuery.prepare("SELECT MAX(id) FROM employe");

    if (!verifQuery.exec()) {
        QMessageBox::critical(this, "Erreur BD",
                              QString("Erreur lors de l'exécution de la requête SELECT MAX(id): %1")
                                  .arg(verifQuery.lastError().text()));
        return;
    }

    if (!verifQuery.next()) {
        QMessageBox::critical(this, "Erreur BD",
                              "Impossible de lire la réponse de la base de données.");
        return;
    }

    QVariant maxIdVariant = verifQuery.value(0);
    if (maxIdVariant.isNull()) {
        QMessageBox::warning(this, "BD vide", "Aucun employé trouvé dans la base.");
        return;
    }

    int currentMaxId = maxIdVariant.toInt();
    if (idEmploye != currentMaxId) {
        QMessageBox::warning(this, "Avertissement",
                             QString("L'ID de l'empreinte (%1) ne correspond pas au dernier employé (%2)")
                                 .arg(idEmploye).arg(currentMaxId));
        return; // Arrêter l'exécution si l'ID est incorrect
    }

    // 2. Mettre à jour l'attribut 'empreinte' ET 'id_empreinte_capteur'
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE employe SET empreinte = ?, id_empreinte_capteur = ? WHERE id = ?");

    QByteArray dummyData = QByteArray::fromHex("00"); // Donnée factice pour test, à remplacer par la vraie empreinte si dispo

    // Supposons pour l'exemple qu'on veut utiliser idEmploye comme id_empreinte_capteur :
    int idEmpreinteCapteur = idEmploye; // à adapter selon ton vrai ID du capteur, si tu l'as dans ta logique

    updateQuery.addBindValue(dummyData);           // Ajouter la valeur BLOB
    updateQuery.addBindValue(idEmpreinteCapteur);  // Ajouter l'ID depuis le capteur
    updateQuery.addBindValue(idEmploye);           // Ajouter l'ID

    if (updateQuery.exec()) {
        QMessageBox::information(this, "Succès",
                                 QString("Empreinte enregistrée avec succès pour l'employé #%1").arg(idEmploye));

        // Mettre à jour l'affichage si besoin
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur BD",
                              QString("Erreur lors de la mise à jour: %1").arg(updateQuery.lastError().text()));
    }

    // 3. Réinitialiser l'interface
    ui->btnAjouterEmpreinte->setEnabled(true);
    ui->btnAjouterEmpreinte->setText("Ajout avec succés");
}
void MainWindow::onErreurArduino(const QString &message)
{
    // 1. Afficher le message d'erreur dans la barre de statut
    ui->statusbar->showMessage("Erreur Arduino: " + message, 10000); // 10 secondes

    // 2. Journalisation de l'erreur
    qCritical() << "Erreur Arduino:" << message;

    // 3. Feedback visuel sur le bouton
    QString originalStyle = ui->btnAjouterEmpreinte->styleSheet();
    QString originalText = ui->btnAjouterEmpreinte->text();

    // Changement visuel temporaire
    ui->btnAjouterEmpreinte->setStyleSheet("background-color: #ffcccc; color: #cc0000;");
    ui->btnAjouterEmpreinte->setText("Erreur - Cliquez pour réessayer");
    ui->btnAjouterEmpreinte->setEnabled(true);

    // 4. Gestion des erreurs spécifiques
    if(message.contains("capteur", Qt::CaseInsensitive)) {
        QMessageBox::critical(this, "Erreur Capteur",
                              "Problème de communication avec le capteur d'empreintes.\n"
                              "Veuillez vérifier:\n"
                              "1. Que le capteur est bien branché\n"
                              "2. Qu'aucun autre programme n'utilise le port série\n"
                              "3. Que le câble USB est en bon état");
    }
    else if(message.contains("timeout", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Timeout",
                             "Temps d'attente dépassé. Veuillez réessayer.");
    }
    else {
        QMessageBox::warning(this, "Erreur", message);
    }

    // 5. Réinitialisation après 5 secondes
    QTimer::singleShot(5000, this, [this, originalStyle, originalText]() {
        ui->btnAjouterEmpreinte->setStyleSheet(originalStyle);
        ui->btnAjouterEmpreinte->setText(originalText);
        ui->statusbar->clearMessage();
    });

    // 6. Tentative automatique de reconnexion pour certaines erreurs
    if(message.contains("connexion", Qt::CaseInsensitive)) {
        QTimer::singleShot(3000, this, [this]() {
            if(!arduino->estConnecte()) {
                ui->statusbar->showMessage("Tentative de reconnexion...", 3000);
                arduino->connecter();
            }
        });
    }
}
void MainWindow::on_btnCheckEmpreinte_clicked()
{
    if (!arduino->estConnecte()) {
        QMessageBox::warning(this,"Erreur","Arduino non connecté.");
        return;
    }
    arduino->demarrerVerification(); // Envoie "VERIFY" à Arduino, qui fait la détection/matching
}
void MainWindow::slotEmpreinteReconnue(int idCapteur)
{
    // 1 → Rechercher l'employé ayant id_empreinte_capteur = idCapteur
    QSqlQuery q;
    q.prepare("SELECT id, nom, prenom FROM employe WHERE id_empreinte_capteur = :cid"); // adapte le champ si besoin
    q.bindValue(":cid", idCapteur);
    if (q.exec() && q.next()) {
        int idEmploye = q.value(0).toInt(); // l’id réel de la table ‘employe’ (clé primaire)
        QString nom = q.value(1).toString();
        QString prenom = q.value(2).toString();
        // 2 → Affiche la notification (id “humain” employé !)
        QMessageBox::information(this, "Succès",
                                 QString("Empreinte reconnue pour %1 %2 (id: %3)").arg(prenom, nom).arg(idEmploye));
        ui->tabWidget->setCurrentIndex(5);
        chargementTodoList(); // On charge la todoList de cet employé, via la vraie id BDD !
    } else {
        QMessageBox::critical(this, "Erreur", "Employé non trouvé pour cette empreinte capteur !");
    }
}
void MainWindow::slotEmpreinteNonTrouvee()
{
    QMessageBox::critical(this, "Accès refusé", "Empreinte inconnue, accès impossible.");
    // Tu peux aussi logger, bloquer, etc.
}
