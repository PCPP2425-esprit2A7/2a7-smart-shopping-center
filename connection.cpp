#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("SMM");  // insérer le nom de la source de données
    db.setUserName("youssef");   // insérer le nom de l'utilisateur
    db.setPassword("esprit18");  // insérer le mot de passe de cet utilisateur

    if (db.open())
        test = true;

    return test;
}
