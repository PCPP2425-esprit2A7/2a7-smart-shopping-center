#include "espace.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QRegularExpression>

bool Espace::ajouter() {

    QSqlQuery query;
    query.prepare("INSERT INTO espace (NOM, TYPE, SUPERFICIE, EMPLACEMENT, STATUT, LOYER_M, DATE_DEB_CONTRAT, DATE_FIN,ID_Locataire) "
                  "VALUES (:nom, :type, :superficie, :emplacement, :statut, :loyer, :debut, :fin,1)");

    query.bindValue(":nom", Nom);
    query.bindValue(":type", Type);
    query.bindValue(":superficie", Superficie);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":statut", Statut);
    query.bindValue(":loyer", Loyer_Mensuel);
    query.bindValue(":debut", Date_Debut);
    query.bindValue(":fin", Date_Fin);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'espace : " << query.lastError().text();
        return false;
    }
}


QSqlQueryModel* Espace::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM espace");
    return model;
}
bool Espace::supprimer(int id)
{
    // Vérifier si l'ID existe dans la table espace
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM espace WHERE ID = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        // Si l'ID existe, on prépare la suppression
        QSqlQuery query;
        query.prepare("DELETE FROM espace WHERE ID = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            return true;
        } else {
            qDebug() << "Erreur de suppression: " << query.lastError().text();
        }
    } else {
        qDebug() << "ID non trouvé: " << id;
    }
    return false;
}
bool Espace::charger(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM espace WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Populate the object's fields with the data from the database
        Nom = query.value("NOM").toString();
        Type = query.value("TYPE").toString();
        Superficie = query.value("SUPERFICIE").toInt();
        Emplacement = query.value("EMPLACEMENT").toString();
        Statut = query.value("STATUT").toString();
        Loyer_Mensuel = query.value("LOYER_M").toDouble();
        Date_Debut = query.value("DATE_DEB_CONTRAT").toString();
        Date_Fin = query.value("DATE_FIN").toString();
        return true;
    }
    return false;
}

bool Espace::modifier(const QString &nom, const QString &type, int superficie, const QString &emplacement, const QString &statut, double loyer, const QString &debut, const QString &fin)
{
    QSqlQuery query;
    query.prepare("UPDATE espace SET NOM = :nom, TYPE = :type, SUPERFICIE = :superficie, EMPLACEMENT = :emplacement, STATUT = :statut, LOYER_M = :loyer, DATE_DEB_CONTRAT = :debut, DATE_FIN = :fin WHERE ID = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":superficie", superficie);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":statut", statut);
    query.bindValue(":loyer", loyer);
    query.bindValue(":debut", debut);
    query.bindValue(":fin", fin);
    query.bindValue(":id", ID);  // Use the member variable 'ID' to identify the record

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de la modification de l'espace : " << query.lastError().text();
        return false;
    }
}



