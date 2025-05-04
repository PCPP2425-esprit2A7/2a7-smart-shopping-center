#include "employe.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QBuffer>    // ✅ Corriger ici
#include <QIODevice>  // ✅ Pour QIODevice::WriteOnly


// ✅ Constructeur par défaut
Employe::Employe() {
    id = 0;
    nom = "";
    prenom = "";
    date_embauche = QDate::currentDate();
    poste = "";
    salaire = 0.0;
    email = "";
    sexe = "";
    telephone = "";
    pdp = QByteArray(); // Correction du type
    statut = "";
    face_id = QByteArray(); // Correction du type
    empreinte = QByteArray(); // Correction du type


}

// ✅ Constructeur paramétré
Employe::Employe(QString nom, QString prenom, QDate date_embauche, QString poste, double salaire,
                 QString email, QString sexe, QString telephone, QByteArray pdp, QString statut, QByteArray face_id, QByteArray empreinte) {
    this->nom = nom;
    this->prenom = prenom;
    this->date_embauche = date_embauche;
    this->poste = poste;
    this->salaire = salaire;
    this->email = email;
    this->sexe = sexe;
    this->telephone = telephone;
    this->pdp = pdp;
    this->statut = statut;
    this->face_id= face_id;
    this->empreinte= empreinte;

}

bool Employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employe (nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, pdp, statut, face_id, empreinte) "
                  "VALUES (:nom, :prenom, :date_embauche, :poste, :salaire, :email, :sexe, :telephone, :pdp, :statut, :face_id, :empreinte)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":telephone", telephone);
    query.bindValue(":statut", statut);

    // Gestion de la photo de profil
    if (pdp.isEmpty()) {
        qDebug() << "⚠️ Pas d'image fournie.";
        query.bindValue(":pdp", QVariant()); // 🔥 Utilisation de QVariant() pour NULL
    } else {
        query.bindValue(":pdp", pdp);
    }

    // Gestion du Face ID
    if (face_id.isEmpty()) {
        qDebug() << "⚠️ Pas de Face ID fourni.";
        query.bindValue(":face_id", QVariant()); // NULL pour face_id
    } else {
        query.bindValue(":face_id", face_id);
    }
    // Gestion de l'empreinte digitale
    if (empreinte.isEmpty()) {
        qDebug() << "⚠️ Pas d'empreinte digitale fournie.";
        query.bindValue(":empreinte", QVariant()); // NULL pour empreinte
    } else {
        query.bindValue(":empreinte", empreinte);
    }

    // 🔥 Debug de la requête SQL
    qDebug() << "Tentative d'insertion de l'employé :"
             << "Nom:" << nom
             << "Prénom:" << prenom
             << "Date Embauche:" << date_embauche
             << "Poste:" << poste
             << "Salaire:" << salaire
             << "Email:" << email
             << "Sexe:" << sexe
             << "Téléphone:" << telephone
             << "Statut:" << statut
             << "Image (taille en octets):" << pdp.size()
             << "Face ID (taille en octets):" << face_id.size()
             << "empreinte (taille en octets):" << empreinte.size();


    // Exécuter la requête
    if (query.exec()) {
        qDebug() << "✅ Insertion réussie !";
        return true;
    } else {
        qDebug() << "❌ Erreur d'insertion : " << query.lastError().text();
        return false;
    }
}
// ✅ Afficher les employés
QSqlQueryModel* Employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employe");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL lors de l'affichage des employés:" << model->lastError().text();
    }

    return model;
}

// ✅ Supprimer un employé par ID
bool Employe::supprimer(int id) {
    if (!existe(id)) {
        qDebug() << "L'employé avec l'ID " << id << " n'existe pas.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de l'employé :" << query.lastError().text();
        return false;
    }

    qDebug() << "L'employé avec l'ID " << id << " a été supprimé avec succès.";
    return true;
}

// ✅ Vérifier si un employé existe
bool Employe::existe(int id) {
    QSqlQuery query;
    query.prepare("SELECT ID FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification de l'existence de l'employé :" << query.lastError().text();
        return false;
    }
    return query.next();
}

// ✅ Compter les employés
int Employe::compterEmployes() {
    QSqlQuery query("SELECT COUNT(*) FROM employe");

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors du comptage des employés :" << query.lastError().text();
        return -1;
    }
}

// ✅ Rechercher un employé par ID
Employe Employe::rechercher(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        Employe employe;
        employe.setNom(query.value("NOM").toString());
        employe.setPrenom(query.value("PRENOM").toString());
        employe.setDateEmbauche(query.value("DATE_EMBAUCHE").toDate());
        employe.setPoste(query.value("POSTE").toString());
        employe.setSalaire(query.value("SALAIRE").toDouble());
        employe.setEmail(query.value("EMAIL").toString());
        employe.setSexe(query.value("SEXE").toString());
        employe.setTelephone(query.value("TELEPHONE").toString());
        employe.setPdp(query.value("PDP").toByteArray()); // ✅ Correction ici
        employe.setStatut(query.value("STATUT").toString());
        employe.setFaceId(query.value("FACE_ID").toByteArray()); // ✅ Correction ici
        employe.setFaceId(query.value("empreinte").toByteArray()); // ✅ Correction ici



        return employe;
    } else {
        qDebug() << "Erreur lors de la recherche de l'employé :" << query.lastError().text();
        return Employe(); // Retourne un employé vide en cas d'erreur
    }
}

// ✅ Modifier un employé
bool Employe::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE employe SET NOM = :nom, PRENOM = :prenom, DATE_EMBAUCHE = :date_embauche, POSTE = :poste, SALAIRE = :salaire, EMAIL = :email, SEXE = :sexe, TELEPHONE = :telephone, PDP = :pdp, FACE_ID = :face_id, EMPREINTE = :empreinte, STATUT = :statut WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":telephone", telephone);
    query.bindValue(":pdp", pdp);
    query.bindValue(":statut", statut);
    query.bindValue(":face_id", face_id);
    query.bindValue(":empreinte", face_id);



    return query.exec();
}

// ✅ Charger une image par ID
bool Employe::chargerImage(int id) const
{
    QSqlQuery query;
    query.prepare("SELECT PDP FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        return !imageData.isEmpty(); // ✅ Vérification de la non-nullité
    }

    return false;
}

// ✅ Enregistrer une image par ID
/*bool Employe::enregistrerImage(const QPixmap &pixmap, int id)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QSqlQuery query;
    query.prepare("UPDATE employe SET PDP = :pdp WHERE ID = :id");
    query.bindValue(":pdp", imageData);
    query.bindValue(":id", id);

    return query.exec();
}*/

QImage Employe::getImageById(int id) const
{
    QImage image;
    if (chargerImage(id)) {
        image.loadFromData(pdp);
    }
    return image;
}
bool Employe::enregistrerFaceId(const QByteArray &faceData)
{
    QSqlQuery query;
    query.prepare("UPDATE employe SET face_id = :face_id WHERE id = :id");
    query.bindValue(":face_id", faceData);
    query.bindValue(":id", this->id);

    if (query.exec()) {
        this->face_id = faceData;
        qDebug() << "Face ID enregistré avec succès";
        return true;
    } else {
        qDebug() << "Erreur lors de l'enregistrement du Face ID:" << query.lastError().text();
        return false;
    }
}
