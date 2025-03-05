#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>

class Employe
{
private:
    int id;
    QString nom, prenom, poste, email, sexe, telephone, pdp, statut;
    QDate date_embauche;
    double salaire;

public:
    // Constructeurs
    Employe();
    Employe(QString nom, QString prenom, QDate date_embauche, QString poste, double salaire,
            QString email, QString sexe, QString telephone, QString pdp, QString statut);

    // Getters et Setters
    int getId() const { return id; }
    void setId(int id) { this->id = id; }
    QString getNom() const { return nom; }
    void setNom(const QString &nom) { this->nom = nom; }
    QString getPrenom() const { return prenom; }
    void setPrenom(const QString &prenom) { this->prenom = prenom; }
    QDate getDateEmbauche() const { return date_embauche; }
    void setDateEmbauche(const QDate &date_embauche) { this->date_embauche = date_embauche; }
    QString getPoste() const { return poste; }
    void setPoste(const QString &poste) { this->poste = poste; }
    double getSalaire() const { return salaire; }
    void setSalaire(double salaire) { this->salaire = salaire; }
    QString getEmail() const { return email; }
    void setEmail(const QString &email) { this->email = email; }
    QString getSexe() const { return sexe; }
    void setSexe(const QString &sexe) { this->sexe = sexe; }
    QString getTelephone() const { return telephone; }
    void setTelephone(const QString &telephone) { this->telephone = telephone; }
    QString getPdp() const { return pdp; }
    void setPdp(const QString &pdp) { this->pdp = pdp; }
    QString getStatut() const { return statut; }
    void setStatut(const QString &statut) { this->statut = statut; }

    // CRUD Operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlTableModel* getModel();  // Nouveau pour QTableView
};

#endif // EMPLOYE_H
