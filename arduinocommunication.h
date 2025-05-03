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
