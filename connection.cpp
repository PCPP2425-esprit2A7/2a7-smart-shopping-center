#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("SMM");       // Ton DSN
    db.setUserName("youssef");
    db.setPassword("esprit18");
}

bool Connection::createconnect()
{
    if (!db.open()) {
        qDebug() << "❌ Connexion échouée :" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connexion réussie à Oracle.";
    return true;
}

void Connection::closeconnexion()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "🔒 Connexion fermée.";
    }
}
