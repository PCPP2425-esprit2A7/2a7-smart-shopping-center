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
                  "VALUES (:nom, :description, :statut, :cout, :frequence, TO_DATE(:dateDebut, 'DD-MM-YY'), TO_DATE(:dateFin, 'DD-MM-YY'), :id_espace)");

    query.bindValue(":nom", nom_service);
    query.bindValue(":description", description_service);
    query.bindValue(":statut", statut_service);
    query.bindValue(":cout", cout);
    query.bindValue(":frequence", frequence);
    query.bindValue(":dateDebut", date_debut_service);
    query.bindValue(":dateFin", date_fin_service);
    query.bindValue(":id_espace", id_espace);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du service:" << query.lastError().text();
        return false;
    }
    return true;
}
QSqlQueryModel* Service::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT S.id, S.nom, S.description, S.cout, S.frequence, S.statut, "
                    "TO_CHAR(S.date_deb, 'DD-MM-YYYY') AS date_deb, "
                    "TO_CHAR(S.date_fin, 'DD-MM-YYYY') AS date_fin, "
                    "S.id_espace, E.nom AS nom_espace "
                    "FROM SERVICE S "
                    "LEFT JOIN ESPACE E ON S.id_espace = E.id "
                    "ORDER BY S.id");

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Description");
    model->setHeaderData(3, Qt::Horizontal, "Coût");
    model->setHeaderData(4, Qt::Horizontal, "Fréquence");
    model->setHeaderData(5, Qt::Horizontal, "Statut");
    model->setHeaderData(6, Qt::Horizontal, "Date Début");
    model->setHeaderData(7, Qt::Horizontal, "Date Fin");
    model->setHeaderData(8, Qt::Horizontal, "ID Espace");
    model->setHeaderData(9, Qt::Horizontal, "Nom Espace");

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
    qDebug() << "Chargement du service avec ID :" << id;

    QSqlQuery query;
    query.prepare("SELECT nom, description, cout, frequence, statut, date_deb, date_fin, id_espace FROM SERVICE WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        setNom(query.value(0).toString());
        setDescription(query.value(1).toString());
        setCout(query.value(2).toDouble());
        setFrequence(query.value(3).toString());
        setStatut(query.value(4).toString());
        setDateDebut(query.value(5).toString());
        setDateFin(query.value(6).toString());
        setIdEspace(query.value(7).toInt()); // Ajout de l'ID de l'espace

        qDebug() << "Service chargé avec succès ! ID Espace:" << query.value(7).toInt();
        return true;
    } else {
        qDebug() << "Aucun service trouvé avec cet ID.";
        return false;
    }
}


bool Service::modifier(int id, const QString &nom, const QString &description, double cout,
                       const QString &frequence, const QString &statut, const QString &dateDebut,
                       const QString &dateFin, int id_espace)
{
    QSqlQuery query;
    query.prepare("UPDATE \"YOUSSEF\".\"SERVICE\" "
                  "SET \"NOM\" = :nom, \"DESCRIPTION\" = :description, \"COUT\" = :cout, "
                  "\"FREQUENCE\" = :frequence, \"STATUT\" = :statut, \"DATE_DEB\" = TO_DATE(:dateDebut, 'DD-MM-YYYY'), "
                  "\"DATE_FIN\" = TO_DATE(:dateFin, 'DD-MM-YYYY'), \"ID_ESPACE\" = :id_espace "
                  "WHERE \"ID\" = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);
    query.bindValue(":cout", cout);
    query.bindValue(":frequence", frequence);
    query.bindValue(":statut", statut);
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":id_espace", id_espace);

    return query.exec();
}
