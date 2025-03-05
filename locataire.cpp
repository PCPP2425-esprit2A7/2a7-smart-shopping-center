#include "locataire.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlQueryModel>
#include "locataire.h"


// Constructeur par défaut
Locataire::Locataire() {
    id = 0;
    nom = "";
    prenom = "";
    typeActivite = "";
    email = "";
    telephone = "";
    dateDeb = QDate::currentDate();
    dateFin = QDate::currentDate();
    dureeContrat = "";
    paiementEffectue = "";
}

Locataire::Locataire(QString nom, QString prenom, QString typeActivite, QString email,
                     int telephone, QString dureeContrat, QDate dateDeb, QDate dateFin,
                     QString paiementEffectue) {
    this->nom = nom;
    this->prenom = prenom;
    this->typeActivite = typeActivite;
    this->email = email;
    //this->telephone = telephone;
    this->dureeContrat = dureeContrat;
    this->dateDeb = dateDeb;
    this->dateFin = dateFin;
    this->paiementEffectue = paiementEffectue;
}

// Ajouter un locataire (Correction de la requête SQL)
bool Locataire::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO locataire (nom, prenom, typeActivite, email, telephone, dureeContrat, dateDeb, dateFin, paiementEffectue) "
                  "VALUES (:nom, :prenom, :typeActivite, :email, :telephone, :dureeContrat, :dateDeb, :dateFin, :paiementEffectue)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":typeActivite", typeActivite);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":dureeContrat", dureeContrat);
    query.bindValue(":dateDeb", dateDeb);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":paiementEffectue", paiementEffectue);

    qDebug() << "Requête SQL exécutée :" << query.executedQuery();

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du locataire :" << query.lastError().text();
        return false;
    }
    return true;
}
