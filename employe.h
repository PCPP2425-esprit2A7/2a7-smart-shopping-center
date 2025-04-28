#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QPixmap> // Pour manipuler les images
#include <QByteArray> // Pour stocker la photo de profil en binaire
#include <QImage> // Pour convertir QByteArray en image

class Employe
{
private:
    int id;
    QString nom, prenom, poste, email, sexe, telephone, statut;
    QDate date_embauche;
    double salaire;
    QByteArray pdp; // Stockage de l'image en binaire
    QByteArray face_id;
    QByteArray empreinte;



public:
    QString cheminImagePDP;

    // ✅ Constructeurs
    Employe();
    Employe(QString nom, QString prenom, QDate date_embauche, QString poste, double salaire,
            QString email, QString sexe, QString telephone, QByteArray pdp, QString statut, QByteArray face_id,QByteArray empreinte);

    // ✅ CRUD Operations
    bool ajouter();
    bool charger(int id);
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool existe(int id);
    int compterEmployes();
    static Employe rechercher(int id);

    // ✅ Méthodes supplémentaires
    bool chargerImage(int id) const;
    bool enregistrerImage(const QPixmap &pixmap, int id);
    QImage getImageById(int id) const;
    bool enregistrerFaceId(const QByteArray &faceData);

    // ✅ Getters et Setters
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

    QString getStatut() const { return statut; }
    void setStatut(const QString &statut) { this->statut = statut; }

    QByteArray getPdp() const { return pdp; }
    void setPdp(const QByteArray &pdp) { this->pdp = pdp; }
    QByteArray getFaceId() const { return face_id; }
    void setFaceId(const QByteArray &value) { face_id = value; }
    QByteArray getEmpreinte() const { return empreinte; }
    void setEmpreinte(const QByteArray &value) { empreinte = value; }


};

#endif // EMPLOYE_H
