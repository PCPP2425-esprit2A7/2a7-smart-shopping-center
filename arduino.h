#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    // Méthodes de connexion
    bool connecter();
    void deconnecter();
    bool estConnecte() const;

    // Commandes pour le capteur d'empreintes
    void demarrerEnregistrement(int idEmploye);
    void demarrerVerification();
    void enregistrerEmpreinte(int id);


signals:
    void connexionReussie();
    void connexionEchouee(const QString &erreur);
    void empreinteEnregistree(int idEmploye);
    void empreinteReconnue(int idEmploye);
    void erreurCapteur(const QString &message);
    void messageStatut(const QString &message);
    void erreurArduino(const QString &message); // Signal pour transmettre des erreurs

    void empreinteNonTrouvee();            // ← NO_MATCH




private slots:
    void lireDonnees();

private:

    QSerialPort *m_serial;
    QString m_buffer;
    void traiterLigne(const QString &ligne);
};
/*#ifndef ARDUINOCOMMUNICATION_H
#define ARDUINOCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSqlDatabase>

class ArduinoCommunication : public QObject {
    Q_OBJECT

public:
    explicit ArduinoCommunication(QObject *parent = nullptr);
    bool openSerialPort(const QString &portName);
    void closeSerialPort();
    void afficherImageDepuisBDD() ;
    void afficherEvenementSuivant();


private slots:
    void handleReadyRead();

private:
    QSerialPort serial;
    QSqlDatabase db;
    bool sonDejaJoue = false;

    QString getNextEventInfo(int offset);
    bool isImageDisplayed = false; // Déclaration de la variable
    int currentEventIndex = 0;  // Déclare currentEventIndex ici


};

#endif // ARDUINOCOMMUNICATION_H*/

#endif // ARDUINO_H
