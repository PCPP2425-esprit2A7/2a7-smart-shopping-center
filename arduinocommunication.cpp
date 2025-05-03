/*#include "ArduinoCommunication.h"
#include <QSerialPortInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "connexion.h"

#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
        QString eventInfo = getNextEventInfo(currentEventIndex + 1); // Passe l'index actuel pour obtenir l'événement suivant
        serial.write(eventInfo.toUtf8());
        afficherImageDepuisBDD();  // Affiche l'image de l'événement suivant
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

