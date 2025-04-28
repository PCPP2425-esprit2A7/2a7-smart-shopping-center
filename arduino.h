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

#endif // ARDUINO_H
