#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Evenement
{
private:
    int id;
    QString titre;
    QString type;
    int capacite;
    double prix;
    QString affiche;
    QString description;
    QDate date_debut;
    QDate date_fin;
    QString categorie;
    QString statut;
    QString organisateur;
    int id_espace; // Jointure avec espace

public:
    // Constructeurs
    Evenement() {}

    Evenement( QString titre, QString type, int capacite, double prix, QString affiche,
              QString description, QDate date_debut, QDate date_fin, QString categorie,
              QString statut, QString organisateur, int id_espace)
    {

        this->titre = titre;
        this->type = type;
        this->capacite = capacite;
        this->prix = prix;
        this->affiche = affiche;
        this->description = description;
        this->date_debut = date_debut;
        this->date_fin = date_fin;
        this->categorie = categorie;
        this->statut = statut;
        this->organisateur = organisateur;
        this->id_espace = id_espace;
    }

    ~Evenement() {}

    // Getters
    int getId() const { return id; }
    QString getTitre() const { return titre; }
    QString getType() const { return type; }
    int getCapacite() const { return capacite; }
    double getPrix() const { return prix; }
    QString getAffiche() const { return affiche; }
    QString getDescription() const { return description; }
    QDate getDateDebut() const { return date_debut; }
    QDate getDateFin() const { return date_fin; }
    QString getCategorie() const { return categorie; }
    QString getStatut() const { return statut; }
    QString getOrganisateur() const { return organisateur; }
    int getIdEspace() const { return id_espace; }

    // Setters
    void setId(int id) { this->id = id; }
    void setTitre(const QString &titre) { this->titre = titre; }
    void setType(const QString &type) { this->type = type; }
    void setCapacite(int capacite) { this->capacite = capacite; }
    void setPrix(double prix) { this->prix = prix; }
    void setAffiche(const QString &affiche) { this->affiche = affiche; }
    void setDescription(const QString &description) { this->description = description; }
    void setDateDebut(const QDate &date_debut) { this->date_debut = date_debut; }
    void setDateFin(const QDate &date_fin) { this->date_fin = date_fin; }
    void setCategorie(const QString &categorie) { this->categorie = categorie; }
    void setStatut(const QString &statut) { this->statut = statut; }
    void setOrganisateur(const QString &organisateur) { this->organisateur = organisateur; }
    void setIdEspace(int id_espace) { this->id_espace = id_espace; }

    // CRUD
    bool ajouter();
    QSqlQueryModel *afficher();
    bool modifier(int id);
    bool supprimer(int id);
};

#endif // EVENEMENT_H

