#ifndef LOCATAIRE_H
#define LOCATAIRE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Locataire {
private:
    int id;  // Ajout de l'ID comme membre privé
    QString nom, prenom, typeActivite, email, telephone, dureeContrat, paiementEffectue;
    QDate dateDeb, dateFin;

public:
    // Constructeurs
    Locataire();  // Constructeur par défaut
    Locataire(QString nom, QString prenom, QString typeActivite, QString email, int telephone,
              QString dureeContrat, QDate dateDeb, QDate dateFin, QString paiementEffectue);
    Locataire(int id, QString nom, QString prenom, QString typeActivite, QString email, int telephone,
              QString dureeContrat, QDate dateDeb, QDate dateFin, QString paiementEffectue);

    // Getters et Setters
    int getId() const { return id; }
    void setId(int id) { this->id = id; }
    QString getNom() const { return nom; }
    void setNom(const QString &nom) { this->nom = nom; }
    QString getPrenom() const { return prenom; }
    void setPrenom(const QString &prenom) { this->prenom = prenom; }
    QString getTypeActivite() const { return typeActivite; }
    void setTypeActivite(const QString &typeActivite) { this->typeActivite = typeActivite; }
    QString getEmail() const { return email; }
    void setEmail(const QString &email) { this->email = email; }
    //int getTelephone() const { return telephone; }
    void setTelephone(const QString &telephone) { this->telephone = telephone; }
    QString getDureeContrat() const { return dureeContrat; }
    void setDureeContrat(const QString &dureeContrat) { this->dureeContrat = dureeContrat; }
    QDate getDateDeb() const { return dateDeb; }
    void setDateDeb(const QDate &dateDeb) { this->dateDeb = dateDeb; }
    QDate getDateFin() const { return dateFin; }
    void setDateFin(const QDate &dateFin) { this->dateFin = dateFin; }
    QString getPaiementEffectue() const { return paiementEffectue; }
    void setPaiementEffectue(const QString &paiementEffectue) { this->paiementEffectue = paiementEffectue; }

    // CRUD Operations
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool existe(int id);
};

#endif // LOCATAIRE_H
