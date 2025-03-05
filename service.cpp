#include "service.h"
#include <QSqlError>
#include <QDebug>

Service::Service() {}

Service::Service(QString nom, QString description, double cout, QString frequence, QString statut, QString dateDebut, QString dateFin, int idEspace)
{

    this->nom_service = nom;
    this->description_service = description;
    this->cout = cout;
    this->frequence = frequence;
    this->statut_service = statut;
    this->date_debut_service = dateDebut;
    this->date_fin_service = dateFin;
    this->id_espace = idEspace;  // Initialisation de id_espace
}

bool Service::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO SERVICE (NOM, DESCRIPTION, STATUT, COUT, FREQUENCE, DATE_DEB, DATE_FIN, ID_ESPACE) "
                  "VALUES (:nom, :description, :statut, :cout, :frequence, TO_DATE(:dateDebut, 'DD-MM-YY'), TO_DATE(:dateFin, 'DD-MM-YY'), 1)");

    query.bindValue(":nom", nom_service);
    query.bindValue(":description", description_service);
    query.bindValue(":statut", statut_service);
    query.bindValue(":cout", cout);
    query.bindValue(":frequence", frequence);
    query.bindValue(":dateDebut", date_debut_service);
    query.bindValue(":dateFin", date_fin_service);
    //query.bindValue(":idEspace", id_espace);  // Lier l'id_espace

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du service:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Service::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SERVICE");
    return model;
}

bool Service::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SERVICE WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du service:" << query.lastError().text();
        return false;
    }
    return true;
}


bool Service::loadById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        m_id = id;
        m_nom = query.value("nom").toString();
        m_description = query.value("description").toString();
        m_cout = query.value("cout").toDouble();
        m_frequence = query.value("frequence").toString();
        m_statut = query.value("statut").toString();
        m_dateDebut = query.value("dateDebut").toString();
        m_dateFin = query.value("dateFin").toString();
        return true;
    }
    return false;
}

bool Service::modifier(int id, const QString &nom, const QString &description, double cout,
                       const QString &frequence, const QString &statut, const QString &dateDebut, const QString &dateFin)
{
    QSqlQuery query;
    query.prepare("UPDATE SERVICE SET nom = :nom, description = :description, cout = :cout, "
                  "frequence = :frequence, statut = :statut, date_deb = :dateDebut, date_Fin = :dateFin "
                  "WHERE id = :id");


    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);
    query.bindValue(":cout", cout);
    query.bindValue(":frequence", frequence);
    query.bindValue(":statut", statut);
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);

    return query.exec();
}
