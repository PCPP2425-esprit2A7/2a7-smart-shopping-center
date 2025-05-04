#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include "ui_mainwindow.h"


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
    QString analyse;

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
    void setAnalyse(const QString &a) { analyse = a; }

    // CRUD
    bool ajouter();
    static QMap<QString, QString> getEmployeById(int id);
    static bool employeeExiste(int id);
    static bool espaceExiste(int id);
    QSqlTableModel* getModel();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    void afficherDetails(int id, Ui::MainWindow *ui);
    bool update(int id, QString type, QString description, double cout, QString statut, QString date_resolution, int id_employe, int id_espace) ;

    //recherche
    QSqlQueryModel* rechercher(QString id, QString id_espace, QString id_employe, QString type, QString statut, QString date, QString nom_espace, QString nom_employee, QString prenom_employee);

    //stat
    QMap<QString, double> getCoutsParTypeEtStatut();

    //analyse
    static bool updateEmployeStatut(int id, const QString& statut);
    static QMap<QString, QString> getEmployeInactifByPoste(const QString& poste);




};

#endif // MAINTENANCE_H
