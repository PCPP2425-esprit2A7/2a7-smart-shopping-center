#include "maintenance.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>


// Constructeur par défaut
Maintenance::Maintenance()
    : id(0), type(""), description(""), cout(0.0), statut(""),
    date_resolution(""), id_employe(0), id_espace(0) {}

// Constructeur avec paramètres
Maintenance::Maintenance(const QString &type, const QString &description,
                         double cout, const QString &statut, const QString &date_resolution,
                         int id_employe, int id_espace)
    : type(type), description(description), cout(cout), statut(statut),
    date_resolution(date_resolution), id_employe(id_employe), id_espace(id_espace) {}

// Getters
int Maintenance::getId() const { return id; }
QString Maintenance::getType() const { return type; }
QString Maintenance::getDescription() const { return description; }
double Maintenance::getCout() const { return cout; }
QString Maintenance::getStatut() const { return statut; }
QString Maintenance::getDateResolution() const { return date_resolution; }
int Maintenance::getIdEmploye() const { return id_employe; }
int Maintenance::getIdEspace() const { return id_espace; }

// Setters
void Maintenance::setId(int newId) { id = newId; }
void Maintenance::setType(const QString &newType) { type = newType; }
void Maintenance::setDescription(const QString &newDescription) { description = newDescription; }
void Maintenance::setCout(double newCout) { cout = newCout; }
void Maintenance::setStatut(const QString &newStatut) { statut = newStatut; }
void Maintenance::setDateResolution(const QString &newDateResolution) { date_resolution = newDateResolution; }
void Maintenance::setIdEmploye(int newIdEmploye) { id_employe = newIdEmploye; }
void Maintenance::setIdEspace(int newIdEspace) { id_espace = newIdEspace; }


// Ajouter une maintenance
bool Maintenance::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO maintenance (TYPE, DESCRIPTION, COUT, STATUT, DATE_RESOLUTION, ID_EMPLOYE, ID_ESPACE) "
                  "VALUES (:type, :description, :cout, :statut, :date_resolution, :id_employe, :id_espace)");

    query.bindValue(":type", type);
    query.bindValue(":description", description);
    query.bindValue(":cout", cout);
    query.bindValue(":statut", statut);
    query.bindValue(":date_resolution", date_resolution);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_espace", id_espace);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de la maintenance :" << query.lastError().text();
        return false;
    }
}

QSqlTableModel* Maintenance::getModel()
{
    QSqlTableModel* model = new QSqlTableModel();
    model->setTable("maintenance");
    model->select();

    // Personnalisation des en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Coût"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de résolution"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Espace"));

    return model;
}

QSqlQueryModel* Maintenance::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM maintenance");

    // Personnalisation des en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Coût"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de résolution"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Espace"));

    return model;
}
