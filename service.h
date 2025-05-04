#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlQueryModel>
#include <QSqlError>

class Service
{
private:
    int id_service;
    QString nom_service;
    QString description_service;
    double cout;
    QString frequence;
    QString statut_service;
    QString date_debut_service;
    QString date_fin_service;
    int id_espace;



public:
    Service();
    Service( QString, QString, double, QString, QString, QString, QString, int);

    // CRUD Methods
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool loadById(int id);
    bool modifier(int id, const QString &nom, const QString &description, double cout,
                  const QString &frequence, const QString &statut, const QString &dateDebut, const QString &dateFin,int id_espace);


    // Getters & Setters
    int getId() { return id_service; }
    QString getNom() { return nom_service; }
    QString getDescription() { return description_service; }
    double getCout() { return cout; }
    QString getFrequence() { return frequence; }
    QString getStatut() { return statut_service; }
    QString getDateDebut() { return date_debut_service; }
    QString getDateFin() { return date_fin_service; }
    int getIdEspace() { return id_espace; }  // Getter pour id_espace

    void setId(int id) { id_service = id; }
    void setNom(QString nom) { nom_service = nom; }
    void setDescription(QString desc) { description_service = desc; }
    void setCout(double c) { cout = c; }
    void setFrequence(QString f) { frequence = f; }
    void setStatut(QString s) { statut_service = s; }
    void setDateDebut(QString d) { date_debut_service = d; }
    void setDateFin(QString d) { date_fin_service = d; }
    void setIdEspace(int id) { id_espace = id; }  // Setter pour id_espace
};

#endif // SERVICE_H
