#include "employe.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>

// Constructeur par défaut
Employe::Employe() {
    id = 0;
    nom = "";
    prenom = "";
    date_embauche = QDate::currentDate();
    poste = "";
    salaire = 0.0;
    email = "";
    sexe = "";
    telephone = "";
    pdp = "";
    statut = "";
}

// Constructeur paramétré
Employe::Employe(QString nom, QString prenom, QDate date_embauche, QString poste, double salaire,
                 QString email, QString sexe, QString telephone, QString pdp, QString statut) {
    this->nom = nom;
    this->prenom = prenom;
    this->date_embauche = date_embauche;
    this->poste = poste;
    this->salaire = salaire;
    this->email = email;
    this->sexe = sexe;
    this->telephone = telephone;
    this->pdp = pdp;
    this->statut = statut;
}

// Ajouter un employé
bool Employe::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO employe (NOM, PRENOM, DATE_EMBAUCHE, POSTE, SALAIRE, EMAIL, SEXE, TELEPHONE, PDP, STATUT) "
                  "VALUES (:nom, :prenom, :date_embauche, :poste, :salaire, :email, :sexe, :telephone, :pdp, :statut)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":telephone", telephone);
    query.bindValue(":pdp", pdp);
    query.bindValue(":statut", statut);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'employé :" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employe");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL lors de l'affichage des employés:" << model->lastError().text();
    }

    return model;
}
// Supprimer un employé par ID
bool Employe::supprimer(int id) {
    // Vérification si l'employé existe avant la suppression
    if (!existe(id)) {
        qDebug() << "L'employé avec l'ID " << id << " n'existe pas.";
        return false;
    }

    // Création de la requête pour supprimer l'employé
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    // Exécution de la requête de suppression
    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de l'employé :" << query.lastError().text();
        return false;
    }

    qDebug() << "L'employé avec l'ID " << id << " a été supprimé avec succès.";
    return true;
}



// Vérifier si un employé existe
bool Employe::existe(int id) {
    QSqlQuery query;
    query.prepare("SELECT ID FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification de l'existence de l'employé :" << query.lastError().text();
        return false;
    }
    return query.next();
}

// Récupérer le nombre total d'employés
int Employe::compterEmployes() {
    QSqlQuery query("SELECT COUNT(*) FROM employe");

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors du comptage des employés :" << query.lastError().text();
        return -1;
    }
}
// employe.cpp

