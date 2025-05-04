#include "arduino.h"
#include <QDebug>
#include <QMessageBox>

Arduino::Arduino(QObject *parent) : QObject(parent), m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::readyRead, this, &Arduino::lireDonnees);
}

Arduino::~Arduino()
{
    deconnecter();
}

bool Arduino::connecter()
{
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
        // Windows: généralement "COM3", Linux: "/dev/ttyACM0"
        if (portInfo.portName().contains("COM3")) { // À adapter selon votre port
            m_serial->setPort(portInfo);
            break;
        }
    }

    if (!m_serial->isOpen()) {
        m_serial->setBaudRate(QSerialPort::Baud9600);
        m_serial->setDataBits(QSerialPort::Data8);
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setStopBits(QSerialPort::OneStop);

        if (m_serial->open(QIODevice::ReadWrite)) {
            emit connexionReussie();
            emit messageStatut("Connecté à l'Arduino");
            return true;
        } else {
            emit connexionEchouee(m_serial->errorString());
            return false;
        }
    }
    return true;
}

void Arduino::deconnecter()
{
    if (m_serial->isOpen()) {
        m_serial->close();
        emit messageStatut("Déconnecté de l'Arduino");
    }
}

bool Arduino::estConnecte() const
{
    return m_serial->isOpen();
}

void Arduino::demarrerEnregistrement(int idEmploye)
{
    if (m_serial->isOpen()) {
        QString commande = QString("ENROLL:%1\n").arg(idEmploye);
        qDebug() << "Commande envoyée à Arduino:" << commande;
        m_serial->write(commande.toUtf8());
        emit messageStatut("Démarrage enregistrement empreinte...");
    }
}



void Arduino::demarrerVerification()
{
    if (m_serial->isOpen()) {
        m_serial->write("VERIFY\n");
        emit messageStatut("Démarrage vérification empreinte...");
    }
}

void Arduino::lireDonnees()
{
    m_buffer += m_serial->readAll();

    // Traitement ligne par ligne
    while (m_buffer.contains("\n")) {
        int lineEnd = m_buffer.indexOf("\n");
        QString ligne = m_buffer.left(lineEnd).trimmed();
        m_buffer.remove(0, lineEnd + 1);

        traiterLigne(ligne);
    }
}

void Arduino::traiterLigne(const QString &ligne)
{
    qDebug() << "[DEBUG] Arduino::traiterLigne - Reçu:" << ligne;

    if (ligne.startsWith("ENROLLED:")) {
        const QStringList parts = ligne.split(':');
        if (parts.size() == 2) {
            bool ok = false;
            int id = parts[1].toInt(&ok);
            if (ok) {
                emit empreinteEnregistree(id);
                emit messageStatut(QString("Empreinte enregistrée pour ID %1").arg(id));
            } else {
                emit erreurCapteur("Format ID invalide après ENROLLED:");
            }
        } else {
            emit erreurCapteur("Format de message invalide pour ENROLLED:");
        }
    }
    else if (ligne.startsWith("MATCH:")) {
        const QStringList parts = ligne.split(':');
        if (parts.size() == 2) {
            bool ok = false;
            int id = parts[1].toInt(&ok);
            if (ok) {
                emit empreinteReconnue(id);
                emit messageStatut(QString("Empreinte reconnue: ID %1").arg(id));
                if (m_serial && m_serial->isOpen())
                    m_serial->write("OPEN_DOOR\n");  // Ouvrir la porte (si besoin)
            } else {
                emit erreurCapteur("Format ID invalide après MATCH:");
            }
        } else {
            emit erreurCapteur("Format de message invalide pour MATCH:");
        }
    }
    else if (ligne == "NO_MATCH") {
        emit empreinteNonTrouvee();
        emit messageStatut("Empreinte non trouvée");
    }
    else if (ligne.startsWith("ERROR:")) {
        const QString erreur = ligne.mid(QString("ERROR:").size());
        emit erreurCapteur(erreur);
        emit messageStatut("Erreur: " + erreur);
    }
    else if (ligne.startsWith("STATUS:")) {
        const QString statut = ligne.mid(QString("STATUS:").size());
        emit messageStatut(statut);

        if (statut == "PLACE_FINGER") {
            emit messageStatut("Placez votre doigt sur le capteur.");
        } else if (statut == "REMOVE_FINGER") {
            emit messageStatut("Retirez votre doigt du capteur.");
        }
    }
    else {
        emit messageStatut("Message inconnu reçu: " + ligne);
        qWarning() << "[WARN] Arduino::traiterLigne - Message non traité:" << ligne;
    }
}

void Arduino::enregistrerEmpreinte(int id)
{
    if (estConnecte()) {
        QByteArray commande = QByteArray::number(id) + "#ENREG";
        m_serial->write(commande);
    }
}


/*#include "ArduinoCommunication.h"
#include "connection.h"
#include <QSerialPortInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

ArduinoCommunication::ArduinoCommunication(QObject *parent) : QObject(parent) {
    connect(&serial, &QSerialPort::readyRead, this, &ArduinoCommunication::handleReadyRead);

    Connection c;
    if (c.createconnect()) {
        qDebug() << "Connexion réussie.";
    } else {
        qDebug() << "Erreur de connexion.";
    }
}

bool ArduinoCommunication::openSerialPort(const QString &portName) {
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    return serial.open(QIODevice::ReadWrite);
}

void ArduinoCommunication::closeSerialPort() {
    if (serial.isOpen()) {
        serial.close();
    }
}



void ArduinoCommunication::handleReadyRead() {
    QByteArray data = serial.readAll();

    if (data.contains("1")) {
        // Partie toujours exécutée
        QString eventInfo = getNextEventInfo(currentEventIndex + 1);
        serial.write(eventInfo.toUtf8());
        afficherImageDepuisBDD();
        qDebug() << "Obstacle détecté !";

        // Partie jouée UNE seule fois
        if (!sonDejaJoue) {
            sonDejaJoue = true;
            qDebug() << "Lecture du son bref.mp3";

            QMediaPlayer* player = new QMediaPlayer(this);
            QAudioOutput* audioOutput = new QAudioOutput(this);
            player->setAudioOutput(audioOutput);
            audioOutput->setVolume(1.0);
            player->setSource(QUrl::fromLocalFile("C:/Users/21698/Desktop/projet c++/bref.mp3"));
            player->play();

            connect(player, &QMediaPlayer::mediaStatusChanged, player, [player](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
                    player->deleteLater();
                }
            });
        }
    }

    // Réinitialisation possible (optionnel)
    if (data.contains("reset")) {
        sonDejaJoue = false;
    }
}



QString ArduinoCommunication::getNextEventInfo(int offset) {
    QSqlQuery query;
    query.prepare(R"(
        SELECT TITRE,
               TO_CHAR(DATE_DEB, 'YYYY-MM-DD') AS DATE_DEB,
               TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN,
               PRIX
        FROM (
            SELECT TITRE, DATE_DEB, DATE_FIN, PRIX, ROWNUM AS rnum
            FROM (
                SELECT * FROM EVENEMENT
                WHERE DATE_DEB >= SYSDATE
                ORDER BY DATE_DEB ASC
            )
        )
        WHERE rnum = :offset
    )");

    query.bindValue(":offset", offset);

    if (query.exec() && query.next()) {
        QString titre = query.value(0).toString();
        QString dateDeb = query.value(1).toString();
        QString dateFin = query.value(2).toString();
        QString prix = query.value(3).toString();

        return titre + "|" + dateDeb + "|" + dateFin + "|" + prix;
    }

    return "Aucun|--|--|--";  // S'il n'y a plus d'événements
}


// Variables globales pour l'affichage
QDialog *dialog = nullptr;
int currentEventIndex = 0;
bool isImageDisplayed = false;

void ArduinoCommunication::afficherImageDepuisBDD() {
    if (isImageDisplayed) {
        return;
    }
    isImageDisplayed = true;

    QSqlQuery query;
    QString queryString = QString(R"(
        SELECT * FROM (
            SELECT AFFICHE, TITRE, DATE_DEB, DESCRIPTION, ID_ESPACE, ORGANISATEUR, ROWNUM AS rnum
            FROM (
                SELECT * FROM EVENEMENT
                WHERE DATE_DEB >= SYSDATE
                ORDER BY DATE_DEB ASC
            )
        )
        WHERE rnum = %1
    )").arg(currentEventIndex + 1);  // ROWNUM commence à 1

    if (query.exec(queryString)) {
        if (query.next()) {
            QByteArray blobData = query.value(0).toByteArray();
            QString nomEvenement = query.value(1).toString();
            QString dateDebut = query.value(2).toString();
            QString description = query.value(3).toString();
            QString lieu = query.value(4).toString();
            QString organisateur = query.value(5).toString();

            QPixmap pixmap;
            if (pixmap.loadFromData(blobData)) {
                if (dialog) {
                    delete dialog;
                }

                dialog = new QDialog();
                dialog->setWindowTitle("Détails de l'Événement");

                QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

                // Image
                QLabel *labelImage = new QLabel();
                labelImage->setPixmap(pixmap.scaled(500, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                labelImage->setAlignment(Qt::AlignCenter);
                labelImage->setStyleSheet("border: 2px solid #555; margin: 10px;");

                // Informations
                QWidget *infoWidget = new QWidget();
                QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

                auto createInfoLabel = [](const QString& title, const QString& value) {
                    QLabel *label = new QLabel(QString("<b style='color:#2E86C1;'>%1 :</b> %2").arg(title, value));
                    label->setWordWrap(true);
                    label->setStyleSheet("font-size: 14px; margin: 5px;");
                    return label;
                };

                infoLayout->addWidget(createInfoLabel("Nom de l'événement", nomEvenement));
                infoLayout->addWidget(createInfoLabel("Date de début", dateDebut));
                infoLayout->addWidget(createInfoLabel("Description", description));
                infoLayout->addWidget(createInfoLabel("Lieu", lieu));
                infoLayout->addWidget(createInfoLabel("Organisateur", organisateur));

                infoWidget->setLayout(infoLayout);

                mainLayout->addWidget(labelImage);
                mainLayout->addWidget(infoWidget);

                // Boutons "Suivant" et "Fermer"
                QPushButton *nextButton = new QPushButton("Suivant");
                QPushButton *closeButton = new QPushButton("Fermer");

                nextButton->setFixedWidth(120);
                closeButton->setFixedWidth(120);

                nextButton->setStyleSheet("padding: 8px; font-size: 14px; background-color: #2ECC71; color: white; border-radius: 5px;");
                closeButton->setStyleSheet("padding: 8px; font-size: 14px; background-color: #E74C3C; color: white; border-radius: 5px;");

                QHBoxLayout *buttonLayout = new QHBoxLayout();
                buttonLayout->addStretch();
                buttonLayout->addWidget(nextButton);
                buttonLayout->addWidget(closeButton);
                buttonLayout->addStretch();

                mainLayout->addLayout(buttonLayout);

                // Connecter les boutons
                connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
                connect(nextButton, &QPushButton::clicked, this, &ArduinoCommunication::afficherEvenementSuivant);

                dialog->setLayout(mainLayout);
                dialog->resize(600, 700);
                dialog->exec();

                // Envoyer infos sur LCD via Arduino (port série)
                QString lcdData = nomEvenement + "|" + dateDebut + "|" + description + "|" + lieu + "|" + organisateur;
                serial.write(lcdData.toUtf8());
            } else {
                qDebug() << "Erreur chargement de l'image.";
            }
        } else {
            qDebug() << "Aucun événement trouvé.";
        }
    } else {
        qDebug() << "Erreur requête Oracle:" << query.lastError().text();
    }

    isImageDisplayed = false;
}

void ArduinoCommunication::afficherEvenementSuivant() {
    currentEventIndex++; // Passer à l'événement suivant
    if (dialog) {
        dialog->accept(); // Fermer le précédent
    }
    afficherImageDepuisBDD(); // Afficher dans une nouvelle fenêtre


    QString eventInfo = getNextEventInfo(currentEventIndex + 1);
    serial.write(eventInfo.toUtf8());
}*/

