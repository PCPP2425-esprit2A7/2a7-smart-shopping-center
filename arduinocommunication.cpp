
/*#include "ArduinoCommunication.h"
#include <QSerialPortInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "connection.h"
ArduinoCommunication::ArduinoCommunication(QObject *parent) : QObject(parent) {
    connect(&serial, &QSerialPort::readyRead, this, &ArduinoCommunication::handleReadyRead);

    Connection c;

    bool test = c.createconnect();
    if (test) {
        qDebug() << "sahit";

    } else {
        qDebug() << "Erreur ";

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
        QString eventInfo = getNextEventInfo();
        serial.write(eventInfo.toUtf8());
    }
}

QString ArduinoCommunication::getNextEventInfo() {
    QSqlQuery query(R"(
        SELECT TITRE,
               TO_CHAR(DATE_DEB, 'YYYY-MM-DD') AS DATE_DEB,
               TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN,
               PRIX
        FROM (
            SELECT * FROM EVENEMENT
            WHERE DATE_DEB >= SYSDATE
            ORDER BY DATE_DEB ASC
        )
        WHERE ROWNUM = 1
    )");

    if (query.next()) {
        QString titre = query.value(0).toString();
        QString dateDeb = query.value(1).toString();
        QString dateFin = query.value(2).toString();
        QString prix = query.value(3).toString();

        // Retourner les informations sous forme de chaîne séparée par des "|"
        return titre + "|" + dateDeb + "|" + dateFin + "|" + prix;
    }

    return "Aucun|--|--|--";  // Retourne "Aucun événement" si aucune donnée trouvée
}
*/

