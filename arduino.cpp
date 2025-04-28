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
