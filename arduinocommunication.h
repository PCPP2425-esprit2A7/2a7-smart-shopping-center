
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

private slots:
    void handleReadyRead();

private:
    QSerialPort serial;
    QSqlDatabase db;
    QString getNextEventInfo();
};

#endif // ARDUINOCOMMUNICATION_H
*/
