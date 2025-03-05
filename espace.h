#ifndef ESPACE_H
#define ESPACE_H

#include <QString>
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
    QString Date_Debut;  // Nom correct de la variable
    QString Date_Fin;  // Vous avez mentionné Date_fin, mais cela doit être cohérent
    int ID_Locataire; // Clé étrangère (ID du locataire)

public:
    // Constructeur
    Espace(const QString& nom = "", const QString& type = "", double superficie = 0,
           const QString& emplacement = "", const QString& statut = "", double loyer = 0,
           const QString& debut = "", const QString& fin = "", int id_locataire = 0)
        : Nom(nom), Type(type), Superficie(superficie), Emplacement(emplacement),
        Statut(statut), Loyer_Mensuel(loyer), Date_Debut(debut), Date_Fin(fin),ID_Locataire() {}

    // Getters
    int getID_Espace() const { return ID; }
    QString getNom_Espace() const { return Nom; }
    QString getType_Espace() const { return Type; }
    double getSuperficie() const { return Superficie; }
    QString getEmplacement() const { return Emplacement; }
    QString getStatut() const { return Statut; }
    double getLoyer_Mensuel() const { return Loyer_Mensuel; }
    QString getDate_Debut() const { return Date_Debut; }
    QString getDate_Fin() const { return Date_Fin; }
    int getId_locataire() const { return ID_Locataire ; }

    // Setters
    void setNom(const QString& nom) { Nom = nom; }
    void setType(const QString& type) { Type = type; }
    void setSuperficie(double superficie) { Superficie = superficie; }
    void setEmplacement(const QString& emplacement) { Emplacement = emplacement; }
    void setStatut(const QString& statut) { Statut = statut; }
    void setLoyer_Mensuel(double loyer) { Loyer_Mensuel = loyer; }
    void setDate_Debut(const QString& debut) { Date_Debut = debut; }
    void setDate_Fin(const QString& fin) { Date_Fin = fin; }

    // CRUD Operations (Assumed to be implemented in .cpp file)
    bool ajouter();
    bool supprimer(int id);
 bool modifier(const QString &nom, const QString &type, int superficie, const QString &emplacement, const QString &statut, double loyer, const QString &debut, const QString &fin);
    QSqlQueryModel* afficher();
    bool charger(int id);
};

#endif // ESPACE_H
