#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class Connection
{
public:
    Connection();
    bool createconnect();
    QString getLastError() const;
    void closeConnection();

private:
    QSqlDatabase db;
    QString lastError;
    bool loadConnectionSettings(QString& dbName, QString& userName, QString& password);
};

#endif // CONNECTION_H
