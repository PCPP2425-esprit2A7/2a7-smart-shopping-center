#ifndef ESPACE_H
#define ESPACE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>

class Espace {
private:
    int ID;  // ID auto-généré dans la base de données
    QString Nom;
    QString Type;
    double Superficie;
    QString Emplacement;
    QString Statut;
    double Loyer_Mensuel;
    QDate Date_Debut;  // Stocké en tant que QDate
    QDate Date_Fin;    // Stocké en tant que QDate
    int ID_Locataire; // Clé étrangère (ID du locataire)
    QString m_lastError;
public:
    // Constructeur
    Espace(const QString& nom = "", const QString& type = "", double superficie = 0,
           const QString& emplacement = "", const QString& statut = "", double loyer = 0,
           const QDate& debut = QDate(), const QDate& fin = QDate(), int id_locataire = 0)
        : Nom(nom), Type(type), Superficie(superficie), Emplacement(emplacement),
        Statut(statut), Loyer_Mensuel(loyer), Date_Debut(debut), Date_Fin(fin), ID_Locataire(id_locataire) {}

    // Getters
    int getID_Espace() const { return ID; }
    QString getNom_Espace() const { return Nom; }
    QString getType_Espace() const { return Type; }
    double getSuperficie() const { return Superficie; }
    QString getEmplacement() const { return Emplacement; }
    QString getStatut() const { return Statut; }
    double getLoyer_Mensuel() const { return Loyer_Mensuel; }
    QDate getDate_Debut() const { return Date_Debut; }
    QDate getDate_Fin() const { return Date_Fin; }
    int getId_locataire() const { return ID_Locataire; }

    // Setters
    void setNom(const QString& nom) { Nom = nom; }
    void setType(const QString& type) { Type = type; }
    void setSuperficie(double superficie) { Superficie = superficie; }
    void setEmplacement(const QString& emplacement) { Emplacement = emplacement; }
    void setStatut(const QString& statut) { Statut = statut; }
    void setLoyer_Mensuel(double loyer) { Loyer_Mensuel = loyer; }
    void setDate_Debut(const QDate& debut) { Date_Debut = debut; }
    void setDate_Fin(const QDate& fin) { Date_Fin = fin; }
    void setId_locataire(int id) { ID_Locataire = id; }

    // Static method to get existing space names for autocomplete
    static QStringList getExistingSpaceNames();

    // CRUD Operations (Implémentées dans .cpp)
    bool ajouter();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool charger(int id);
    bool modifier(int id, const QString &nom, const QString &type, double superficie,
                  const QString &emplacement, const QString &statut, double loyerM,
                  const QDate &dateDebutContrat, const QDate &dateFin,
                  int idLocataire);
    QString lastError() const;

};

#endif // ESPACE_H
