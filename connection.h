#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
public:
    Connection();           // Constructeur
    bool createconnect();   // Connexion à la base Oracle via QODBC
    void closeconnexion();  // Ferme la connexion (optionnel)

private:
    QSqlDatabase db;        // Connexion persistante
};

#endif // CONNECTION_H
