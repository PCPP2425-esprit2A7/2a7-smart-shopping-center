#include "event.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include <QBuffer>

bool Evenement::ajouter()
{
    QSqlQuery query;

    // Préparation de la requête SQL avec l'ajout de LIEU
    query.prepare("INSERT INTO evenement (TITRE, DESCRIPTION, DATE_DEB, DATE_FIN, CAPACITE, STATUT, CATEGORIE, TYPE, PRIX, ORGANISATEUR, ID_ESPACE) "
                  "VALUES (:titre, :description, :date_debut, :date_fin, :capacite, :statut, :categorie, :type, :prix, :organisateur, :id_espace)");

    // Binding des valeurs aux paramètres
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":capacite", capacite);
    query.bindValue(":statut", statut);
    query.bindValue(":categorie", categorie);
    query.bindValue(":type", type);
    query.bindValue(":prix", prix);
    query.bindValue(":organisateur", organisateur);
    query.bindValue(":id_espace", id_espace); // Ajout du champ lieu

    // Exécution de la requête
    if (query.exec()) {
        qDebug() << "Événement ajouté avec succès !";
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

bool Evenement::loadById(int id)
{
    qDebug() << "Chargement de l'événement avec ID :" << id;

    QSqlQuery query;
    query.prepare("SELECT TITRE, TYPE, CAPACITE, PRIX, DESCRIPTION, DATE_DEB, DATE_FIN, CATEGORIE, STATUT, ORGANISATEUR, ID_ESPACE "
                  "FROM YOUSSEF.EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        // Remplissage des attributs de l'objet Evenement
        setTitre(query.value("TITRE").toString());
        setType(query.value("TYPE").toString());
        setCapacite(query.value("CAPACITE").toInt());
        setPrix(query.value("PRIX").toDouble());
        setDescription(query.value("DESCRIPTION").toString());
        setDateDebut(query.value("DATE_DEB").toDate());
        setDateFin(query.value("DATE_FIN").toDate());
        setCategorie(query.value("CATEGORIE").toString());
        setStatut(query.value("STATUT").toString());
        setOrganisateur(query.value("ORGANISATEUR").toString());
        setid_espace(query.value("ID_ESPACE").toInt()); // Ajout du champ lieu

        qDebug() << "Événement chargé avec succès !";
        return true;
    } else {
        qDebug() << "Aucun événement trouvé avec cet ID.";
        return false;
    }
}

bool Evenement::modifier(int id, const QString &titre, const QString &type, int capacite, double prix,
                         const QString &description, const QString &dateDebut,
                         const QString &dateFin, const QString &categorie, const QString &statut,
                         const QString &organisateur, const int &id_espace) // Ajout de lieu en paramètre
{
    QSqlQuery query;

    // Préparer la requête SQL pour mettre à jour les données de l'événement
    query.prepare("UPDATE \"YOUSSEF\".\"EVENEMENT\" "
                  "SET \"TITRE\" = :titre, "
                  "\"TYPE\" = :type, "
                  "\"CAPACITE\" = :capacite, "
                  "\"PRIX\" = :prix, "
                  "\"DESCRIPTION\" = :description, "
                  "\"DATE_DEB\" = TO_DATE(:dateDebut, 'YYYY-MM-DD'), "
                  "\"DATE_FIN\" = TO_DATE(:dateFin, 'YYYY-MM-DD'), "
                  "\"CATEGORIE\" = :categorie, "
                  "\"STATUT\" = :statut, "
                  "\"ORGANISATEUR\" = :organisateur, "
                  "\"ID_ESPACE\" = :id_espace " // Supprimer l'espace
                   // Ajout du champ lieu
                  "WHERE \"ID\" = :id");

    // Lier les valeurs aux paramètres
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":type", type);
    query.bindValue(":capacite", capacite);
    query.bindValue(":prix", prix);
    query.bindValue(":description", description);
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":categorie", categorie);
    query.bindValue(":statut", statut);
    query.bindValue(":organisateur", organisateur);
    query.bindValue(":id_espace", id_espace); // Ajout du bindValue pour lieu

    // Exécuter la requête et vérifier le résultat
    if (!query.exec()) {
        // Afficher une erreur si la requête échoue
        qDebug() << "Erreur SQL lors de la modification de l'événement:" << query.lastError().text();
        return false;
    }

    // Si la modification a réussi
    qDebug() << "Événement modifié avec succès !";
    return true;
}

