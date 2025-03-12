#ifndef LOCATAIRE_H
#define LOCATAIRE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Locataire {
private:
    int id, telephone;
    QString nom, prenom, typeActivite, email, dureeContrat, paiementEffectue;
    QDate dateDeb, dateFin;

public:
    // Constructors
    Locataire();
    Locataire(QString nom, QString prenom, QString typeActivite, QString email, int telephone,
              QString dureeContrat, QDate dateDeb, QDate dateFin, QString paiementEffectue);
    Locataire(int id, QString nom, QString prenom, QString typeActivite, QString email, int telephone,
              QString dureeContrat, QDate dateDeb, QDate dateFin, QString paiementEffectue);

    // Getters and Setters
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
    QString getTelephone() const { return QString::number(telephone); }  // Convert to QString
    void setTelephone(int telephone) { this->telephone = telephone; }
    QString getDureeContrat() const { return dureeContrat; }
    void setDureeContrat(const QString &dureeContrat) { this->dureeContrat = dureeContrat; }
    QString getDateDeb() const { return dateDeb.toString("yyyy-MM-dd"); }  // Return as QString
    void setDateDeb(const QDate &dateDeb) { this->dateDeb = dateDeb; }
    QString getDateFin() const { return dateFin.toString("yyyy-MM-dd"); }  // Return as QString
    void setDateFin(const QDate &dateFin) { this->dateFin = dateFin; }
    QString getPaiementEffectue() const { return paiementEffectue; }
    void setPaiementEffectue(const QString &paiementEffectue) { this->paiementEffectue = paiementEffectue; }

    // CRUD Operations
    bool ajouter();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool existe(int id);
    bool loadById(int id);
    bool modifier(int id, const QString &nom, const QString &prenom, const QString &typeActivite,
                  const QString &email, const QString &telephone, const QString &duree,
                  const QString &dateDeb, const QString &dateFin, const QString &paiement);


};

#endif // LOCATAIRE_H
