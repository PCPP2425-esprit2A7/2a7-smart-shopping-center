#include "event.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Evenement::ajouter()
{
    QSqlQuery query;

    // Préparation de la requête SQL
    query.prepare("INSERT INTO evenement (TITRE, DESCRIPTION, DATE_DEB, DATE_FIN, ID_ESPACE, CAPACITE, STATUT, CATEGORIE, TYPE, PRIX, ORGANISATEUR) "
                  "VALUES (:titre, :description, :date_debut, :date_fin, 1, :capacite, :statut, :categorie, :type, :prix, :organisateur)");

    // Binding des valeurs aux paramètres
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":id_espace", id_espace);  // lieu (c'est un espace dans le mall)
    query.bindValue(":capacite", capacite);
    query.bindValue(":statut", statut);
    query.bindValue(":categorie", categorie);
    query.bindValue(":type", type);
    query.bindValue(":prix", prix);
    query.bindValue(":organisateur", organisateur);

    // Exécution de la requête
    if (query.exec()) {
        qDebug() << "Evenement ajouté avec succès !";
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'événement :" << query.lastError().text();
        return false;
    }
}
QSqlQueryModel* Evenement::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EVENEMENT");
    return model;
}
bool Evenement::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Événement supprimé avec succès.";
        return true;
    } else {
        qDebug() << "Erreur lors de la suppression :" << query.lastError().text();
        return false;
    }
}

