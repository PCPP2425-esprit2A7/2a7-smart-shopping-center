#include "maintenance.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QVariant>
#include <QDate>


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
    query.prepare("INSERT INTO maintenance (TYPE, DESCRIPTION, COUT, STATUT, DATE_RESOLUTION, ID_EMPLOYE, ID_ESPACE, ANALYSE) "
                  "VALUES (:type, :description, :cout, :statut, :date_resolution, :id_employe, :id_espace, :analyse)");

    query.bindValue(":type", type);
    query.bindValue(":description", description);
    query.bindValue(":cout", cout);
    query.bindValue(":statut", statut);
    query.bindValue(":date_resolution", date_resolution);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_espace", id_espace);
    query.bindValue(":analyse", analyse);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de la maintenance :" << query.lastError().text();
        return false;
    }
}

QMap<QString, QString> Maintenance::getEmployeById(int id)
{
    QMap<QString, QString> employeData;

    QSqlQuery query;
    query.prepare("SELECT id, nom, prenom, email, telephone FROM employe WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        employeData["id"] = query.value("id").toString();
        employeData["nom"] = query.value("nom").toString();
        employeData["prenom"] = query.value("prenom").toString();
        employeData["email"] = query.value("email").toString();
        employeData["telephone"] = query.value("telephone").toString();
    } else {
        qDebug() << "❌ getEmployeById: employé introuvable ou erreur SQL:" << query.lastError().text();
    }

    return employeData;
}


bool Maintenance::employeeExiste(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM employe WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool Maintenance::espaceExiste(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM espace WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
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

    model->setQuery("SELECT id, type, SUBSTR(description, 1, 14) || '...' as description, cout, statut, date_resolution, id_employe, id_espace, analyse FROM maintenance");

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

bool Maintenance::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM maintenance WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();  // Retourne vrai si la suppression a réussi
}

void Maintenance::afficherDetails(int id, Ui::MainWindow *ui) {
    QSqlQuery query;
    query.prepare("SELECT type, description, cout, statut, id_employe, id_espace, date_resolution FROM maintenance WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        ui->up_id->setText(QString::number(id));
        ui->up_type->setCurrentText(query.value(0).toString());
        ui->up_description->setText(query.value(1).toString());
        ui->up_cout->setText(query.value(2).toString());
        ui->up_statut_maintenance->setCurrentText(query.value(3).toString());
        ui->up_id_employe_maintenance->setText(query.value(4).toString());
        ui->up_id_espace_maintenance->setText(query.value(5).toString());
        ui->up_date_resolution->setDate(query.value(6).toDate());

        ui->tabWidget_3->setCurrentIndex(2);
    } else {
        qDebug() << "Erreur lors du chargement des détails de maintenance :" << query.lastError().text();
    }
}

bool Maintenance::update(int id, QString type, QString description, double cout, QString statut, QString date_resolution, int id_employe, int id_espace) {
    QSqlQuery query;
    query.prepare("UPDATE maintenance SET type = :type, description = :description, cout = :cout, statut = :statut, date_resolution = :date_resolution, id_employe = :id_employe, id_espace = :id_espace WHERE id = :id");

    query.bindValue(":type", type);
    query.bindValue(":description", description);
    query.bindValue(":cout", cout);
    query.bindValue(":statut", statut);
    query.bindValue(":date_resolution", date_resolution);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_espace", id_espace);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Mise à jour réussie pour l'ID" << id;
        return true;
    } else {
        qDebug() << "Échec de la mise à jour de l'ID" << id << ":" << query.lastError().text();
        return false;
    }
}

//recherche

QSqlQueryModel* Maintenance::rechercher(QString id, QString id_espace, QString id_employe, QString type, QString statut, QString date, QString nom_espace, QString nom_employee, QString prenom_employee)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString = R"(
        SELECT
            m.id,
            m.type,
            m.description,
            m.cout,
            m.statut,
            m.date_resolution,
            m.id_employe,
            m.id_espace
        FROM maintenance m
        JOIN espace e ON m.id_espace = e.id
        JOIN employe emp ON m.id_employe = emp.id
        WHERE 1=1
    )";

    if (!id.isEmpty()) queryString += " AND m.id = :id";
    if (!id_espace.isEmpty()) queryString += " AND m.id_espace = :id_espace";
    if (!id_employe.isEmpty()) queryString += " AND m.id_employe = :id_employe";
    if (!type.isEmpty()) queryString += " AND m.type = :type";
    if (!statut.isEmpty()) queryString += " AND m.statut = :statut";
    if (!date.isEmpty()) queryString += " AND m.date_resolution = :date";
    if (!nom_espace.isEmpty()) queryString += " AND e.nom LIKE :nom_espace";
    if (!nom_employee.isEmpty()) queryString += " AND emp.nom LIKE :nom_employee";
    if (!prenom_employee.isEmpty()) queryString += " AND emp.prenom LIKE :prenom_employee";

    query.prepare(queryString);

    if (!id.isEmpty()) query.bindValue(":id", id);
    if (!id_espace.isEmpty()) query.bindValue(":id_espace", id_espace);
    if (!id_employe.isEmpty()) query.bindValue(":id_employe", id_employe);
    if (!type.isEmpty()) query.bindValue(":type", type);
    if (!statut.isEmpty()) query.bindValue(":statut", statut);
    if (!date.isEmpty()) query.bindValue(":date", date);
    if (!nom_espace.isEmpty()) query.bindValue(":nom_espace", "%" + nom_espace + "%");
    if (!nom_employee.isEmpty()) query.bindValue(":nom_employee", "%" + nom_employee + "%");
    if (!prenom_employee.isEmpty()) query.bindValue(":prenom_employee", "%" + prenom_employee + "%");

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
    } else {
        qDebug() << "Requête exécutée:" << queryString;
    }

    model->setQuery(std::move(query));

    return model;
}


//stat

QMap<QString, double> Maintenance::getCoutsParTypeEtStatut() {
    QMap<QString, double> result = {
        {"Instantané Traité", 0},
        {"Instantané Non Traité", 0},
        {"Prédictive Traité", 0},
        {"Prédictive Non Traité", 0}
    };

    QSqlQuery query;
    if (query.exec("SELECT statut, type, SUM(cout) FROM maintenance GROUP BY statut, type")) {
        while (query.next()) {
            QString statut = query.value(0).toString().toLower();
            QString type = query.value(1).toString().toLower();
            double cout = query.value(2).toDouble();

            if (type == "instantané" && statut == "traité") {
                result["Instantané Traité"] += cout;
            } else if (type == "instantané" && statut == "non traité") {
                result["Instantané Non Traité"] += cout;
            } else if (type == "prédictive" && statut == "traité") {
                result["Prédictive Traité"] += cout;
            } else if (type == "prédictive" && statut == "non traité") {
                result["Prédictive Non Traité"] += cout;
            }
        }
    } else {
        qDebug() << "Erreur requête SQL dans getCoutsParTypeEtStatut:" << query.lastError().text();
    }

    return result;
}

//analyse

bool Maintenance::updateEmployeStatut(int id, const QString& statut)
{
    QSqlQuery query;
    query.prepare("UPDATE employe SET statut = :statut WHERE id = :id");
    query.bindValue(":statut", statut);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✔️ Statut de l'employé" << id << "mis à jour en:" << statut;
        return true;
    } else {
        qDebug() << "❌ Erreur mise à jour statut employé:" << query.lastError().text();
        return false;
    }
}

QMap<QString, QString> Maintenance::getEmployeInactifByPoste(const QString& poste)
{
    QMap<QString, QString> employeData;  // contient : id, nom, prenom, email, telephone

    QSqlQuery query;
    query.prepare("SELECT id, nom, prenom, email, telephone FROM employe WHERE lower(poste) LIKE :poste AND lower(statut) = 'inactif'");
    query.bindValue(":poste", poste.toLower().trimmed());
    query.bindValue(":poste", poste.trimmed().toLower());

    if (query.exec() && query.next()) {
        employeData["id"] = query.value("id").toString();
        employeData["nom"] = query.value("nom").toString();
        employeData["prenom"] = query.value("prenom").toString();
        employeData["email"] = query.value("email").toString();
        employeData["telephone"] = query.value("telephone").toString();
        qDebug() << "✔️ Employé inactif trouvé :" << employeData;
        qDebug() << "RECHERCHE EMPLOYE SQL = "
                 << query.lastQuery() << " [poste]=" << poste;
    } else {
        qDebug() << "❌ Aucun employé inactif trouvé avec le poste:" << poste;
    }

    return employeData;
}






