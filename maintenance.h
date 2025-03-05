#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlTableModel>

class Maintenance {
private:
    int id;
    QString type;
    QString description;
    double cout;
    QString statut;
    QString date_resolution;
    int id_employe;
    int id_espace;

public:
    // Constructeurs
    Maintenance();
    Maintenance(const QString &type, const QString &description,
                double cout, const QString &statut, const QString &date_resolution,
                int id_employe, int id_espace);

    // Getters et Setters
    int getId() const;
    QString getType() const;
    QString getDescription() const;
    double getCout() const;
    QString getStatut() const;
    QString getDateResolution() const;
    int getIdEmploye() const;
    int getIdEspace() const;

    void setId(int newId);
    void setType(const QString &newType);
    void setDescription(const QString &newDescription);
    void setCout(double newCout);
    void setStatut(const QString &newStatut);
    void setDateResolution(const QString &newDateResolution);
    void setIdEmploye(int newIdEmploye);
    void setIdEspace(int newIdEspace);

    // CRUD
    bool ajouter();
    QSqlTableModel* getModel();
    QSqlQueryModel* afficher();
};

#endif // MAINTENANCE_H
