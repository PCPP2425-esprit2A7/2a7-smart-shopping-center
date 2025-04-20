#include "locataire.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlQueryModel>
#include "locataire.h"
#include <QFile>


// Constructeur par défaut
Locataire::Locataire() {
    id = 0;
    nom = "";
    prenom = "";
    typeActivite = "";
    email = "";
    telephone = 0;
    dateDeb = QDate::currentDate();
    dateFin = QDate::currentDate();
    dureeContrat = "";
    paiementEffectue = "";
}

Locataire::Locataire(QString nom, QString prenom, QString typeActivite, QString email,
                     int telephone, QString dureeContrat, QDate dateDeb, QDate dateFin,
                     QString paiementEffectue,QByteArray contrat) {
    this->nom = nom;
    this->prenom = prenom;
    this->typeActivite = typeActivite;
    this->email = email;
    //this->telephone = telephone;
    this->dureeContrat = dureeContrat;
    this->dateDeb = dateDeb;
    this->dateFin = dateFin;
    this->paiementEffectue = paiementEffectue;
    this->contrat = contrat;

}

bool Locataire::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO LOCATAIRE (NOM, PRENOM, TYPE_ACTIVITE, EMAIL, TELEPHONE, DUREE_CONTRAT, DATE_DEB, DATE_FIN, PAIMENT_EFFECTUE, CONTRAT) "
                  "VALUES (:nom, :prenom, :typeActivite, :email, :telephone, :duree, :dateDeb, :dateFin, :paiement, :contrat)");


    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":typeActivite", typeActivite);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":duree", dureeContrat);
    query.bindValue(":dateDeb", dateDeb);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":paiement", paiementEffectue);  // Utilisez la variable membre de la classe
    query.bindValue(":contrat", contrat);  // Utilisez la variable membre de la classe

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return false;
    }

    return true;  // Assurez-vous de retourner true lorsque l'ajout réussit
}


bool Locataire::modifier(int id, const QString &nom, const QString &prenom, const QString &typeActivite,
                         const QString &email, const QString &telephone, const QString &duree,
                         const QString &dateDeb, const QString &dateFin, const QString &paiement,
                         const QByteArray &contrat) {
    // Vérification si l'ID est valide
    if (id <= 0) {
        qDebug() << "ID invalide";
        return false;
    }

    bool ok;
    int telephoneInt = telephone.toInt(&ok);

    if (!ok) {
        qDebug() << "Numéro de téléphone invalide : " << telephone;
        return false;
    }

    QSqlQuery query;
    QString formattedDateDeb = QDate::fromString(dateDeb, "yyyy-MM-dd").toString("yyyy-MM-dd");
    QString formattedDateFin = QDate::fromString(dateFin, "yyyy-MM-dd").toString("yyyy-MM-dd");

    query.prepare("UPDATE locataire SET nom = ?, prenom = ?, type_activite = ?, email = ?, telephone = ?, "
                  "duree_contrat = ?, date_deb = TO_DATE(?, 'YYYY-MM-DD'), date_fin = TO_DATE(?, 'YYYY-MM-DD'), "
                  "paiment_effectue = ?, contrat = ? WHERE id = ?");

    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(typeActivite);
    query.addBindValue(email);
    query.addBindValue(telephoneInt);
    query.addBindValue(duree);
    query.addBindValue(formattedDateDeb);
    query.addBindValue(formattedDateFin);
    query.addBindValue(paiement);
    query.addBindValue(contrat, QSql::In | QSql::Binary);  // Mettre à jour le contrat
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return false;
    }

    qDebug() << "Locataire modifié avec succès.";
    return true;
}

QSqlQueryModel* Locataire::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, PRENOM, TYPE_ACTIVITE, EMAIL, TELEPHONE, DUREE_CONTRAT, DATE_DEB, DATE_FIN, PAIMENT_EFFECTUE, CONTRAT FROM LOCATAIRE");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL lors de l'affichage des locataires:" << model->lastError().text();
    } else {
        qDebug() << "Requête exécutée avec succès.";
    }

    return model;
}

bool Locataire::existe(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM locataire WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;  // Retourne vrai si l'ID existe
    }

    return false;
}
// Supprimer un locataire par ID
bool Locataire::supprimer(int id) {
    // Vérification si le locataire existe avant la suppression
    if (!existe(id)) {
        qDebug() << "Le locataire avec l'ID " << id << " n'existe pas.";
        return false;
    }

    // Création de la requête pour supprimer le locataire
    QSqlQuery query;
    query.prepare("DELETE FROM locataire WHERE ID = :id");
    query.bindValue(":id", id);

    // Exécution de la requête de suppression
    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du locataire :" << query.lastError().text();
        return false;
    }

    qDebug() << "Le locataire avec l'ID " << id << " a été supprimé avec succès.";
    return true;
}


bool Locataire::loadById(int id) {
    qDebug() << "Chargement du locataire avec ID :" << id;

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, TYPE_ACTIVITE, EMAIL, TELEPHONE, "
                  "DUREE_CONTRAT, DATE_DEB, DATE_FIN, PAIMENT_EFFECTUE, CONTRAT "
                  "FROM LOCATAIRE WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        setNom(query.value(0).toString());
        setPrenom(query.value(1).toString());
        setTypeActivite(query.value(2).toString());
        setEmail(query.value(3).toString());
        setTelephone(query.value(4).toInt());
        setDureeContrat(query.value(5).toString());
        setDateDeb(query.value(6).toDate());
        setDateFin(query.value(7).toDate());
        setPaiementEffectue(query.value(8).toString());

        // Charger le contrat depuis la base de données
        setContrat(query.value(9).toByteArray());

        qDebug() << "Locataire chargé avec succès !";
        return true;
    } else {
        qDebug() << "Aucun locataire trouvé avec cet ID.";
        return false;
    }
}
