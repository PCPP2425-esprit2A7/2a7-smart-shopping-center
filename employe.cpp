#include "employe.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
// Constructeur par défaut
Employe::Employe()
{
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
    statut = "";  // Initialisation du statut
}

// Constructeur paramétré
Employe::Employe(QString nom, QString prenom, QDate date_embauche, QString poste, double salaire,
                 QString email, QString sexe, QString telephone, QString pdp,  QString statut)
{
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
bool Employe::ajouter()
{
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

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'employé :" << query.lastError().text();
        return false;
    }
}
QSqlTableModel* Employe::getModel()
{
    QSqlTableModel* model = new QSqlTableModel();
    model->setTable("employe");
    model->select();

    // Personnalisation des entêtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Embauche"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("PDP"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
