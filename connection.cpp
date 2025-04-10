#include "connection.h"
#include <QFile>
#include <QTextStream>
#include <QDir>

Connection::Connection()
{
    lastError.clear();
}

bool Connection::loadConnectionSettings(QString& dbName, QString& userName, QString& password)
{
    // Par défaut, on utilise les valeurs actuelles
    dbName = "SMM";
    userName = "youssef";
    password = "esprit18";
    
    // TODO: Implémenter la lecture depuis un fichier de configuration
    // QFile configFile("config.ini");
    // if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&configFile);
    //     // Lire les paramètres de configuration
    //     configFile.close();
    //     return true;
    // }
    
    return true;
}

bool Connection::createconnect()
{
    bool test = false;
    lastError.clear();

    // Charger les paramètres de connexion
    QString dbName, userName, password;
    if (!loadConnectionSettings(dbName, userName, password)) {
        lastError = "Impossible de charger les paramètres de connexion";
        return false;
    }

    // Configurer la connexion
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);

    // Tenter la connexion
    if (db.open()) {
        test = true;
    } else {
        lastError = db.lastError().text();
    }

    return test;
}

QString Connection::getLastError() const
{
    return lastError;
}

void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}
