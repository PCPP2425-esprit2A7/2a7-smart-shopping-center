#include "espace.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Ajout d'un espace dans la base de données
bool Espace::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO espace (NOM, TYPE, SUPERFICIE, EMPLACEMENT, STATUT, LOYER_M, DATE_DEB_CONTRAT, DATE_FIN, ID_Locataire) "
                  "VALUES (:nom, :type, :superficie, :emplacement, :statut, :loyer, TO_DATE(:debut, 'YYYY-MM-DD'), TO_DATE(:fin, 'YYYY-MM-DD'), :id_locataire)");

    query.bindValue(":nom", Nom);
    query.bindValue(":type", Type);
    query.bindValue(":superficie", Superficie);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":statut", Statut);
    query.bindValue(":loyer", Loyer_Mensuel);
    query.bindValue(":debut", Date_Debut.toString("yyyy-MM-dd"));  // Conversion correcte en string
    query.bindValue(":fin", Date_Fin.toString("yyyy-MM-dd"));      // Conversion correcte en string
    query.bindValue(":id_locataire", ID_Locataire);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'espace : " << query.lastError().text();
        return false;
    }
}

// Affichage des espaces
QSqlQueryModel* Espace::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM espace");
    return model;
}

// Suppression d'un espace
bool Espace::supprimer(int id) {
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM espace WHERE ID = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QSqlQuery query;
        query.prepare("DELETE FROM espace WHERE ID = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            return true;
        } else {
            qDebug() << "Erreur de suppression: " << query.lastError().text();
        }
    } else {
        qDebug() << "ID non trouvé: " << id;
    }
    return false;
}
bool Espace::modifier(int id, const QString &nom, const QString &type, double superficie,
                       const QString &emplacement, const QString &statut, double loyerM,
                       const QDate &dateDebutContrat, const QDate &dateFin)
{
    QSqlQuery query;

    // Convert QDate to QString in 'DD-MM-YY' format
    QString strDateDebutContrat = dateDebutContrat.toString("dd-MM-yy");
    QString strDateFin = dateFin.toString("dd-MM-yy");

    query.prepare("UPDATE \"YOUSSEF\".\"ESPACE\" SET \"NOM\" = :nom, \"TYPE\" = :type, \"SUPERFICIE\" = :superficie, "
                  "\"EMPLACEMENT\" = :emplacement, \"STATUT\" = :statut, \"LOYER_M\" = :loyerM, "
                  "\"DATE_DEB_CONTRAT\" = TO_DATE(:dateDebutContrat, 'DD-MM-YY'), "
                  "\"DATE_FIN\" = TO_DATE(:dateFin, 'DD-MM-YY') WHERE \"ID\" = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":superficie", superficie);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":statut", statut);
    query.bindValue(":loyerM", loyerM);
    query.bindValue(":dateDebutContrat", strDateDebutContrat);
    query.bindValue(":dateFin", strDateFin);

    return query.exec();
}

// Get AI-powered space name suggestions
QStringList Espace::getExistingSpaceNames() {
    QStringList suggestions;
    
    // Common space types with variations
    QMap<QString, QStringList> spacePatterns = {
        {"Bureau", {
            "Bureau individuel", "Bureau partagé", "Bureau avec vue",
            "Bureau moderne", "Bureau classique", "Bureau exécutif",
            "Bureau d'équipe", "Bureau de direction", "Bureau de réception"
        }},
        {"Magasin", {
            "Magasin de détail", "Magasin d'exposition", "Magasin de stockage",
            "Magasin de proximité", "Magasin spécialisé", "Magasin de luxe",
            "Magasin de vêtements", "Magasin d'électronique", "Magasin de meubles"
        }},
        {"Salle", {
            "Salle de réunion", "Salle d'exposition", "Salle de conférence",
            "Salle de sport", "Salle polyvalente", "Salle de formation",
            "Salle de spectacle", "Salle de banquet", "Salle de réception"
        }},
        {"Local", {
            "Local commercial", "Local artisanal", "Local industriel",
            "Local de stockage", "Local de service", "Local technique",
            "Local administratif", "Local de production", "Local de vente"
        }},
        {"Zone", {
            "Zone de production", "Zone de service", "Zone de vente",
            "Zone administrative", "Zone technique", "Zone de stockage",
            "Zone de réception", "Zone de livraison", "Zone de parking"
        }},
        {"Espace", {
            "Espace de coworking", "Espace de stockage", "Espace de vente",
            "Espace de réception", "Espace de détente", "Espace de travail",
            "Espace de création", "Espace de formation", "Espace de production"
        }}
    };
    
    // Add all variations to suggestions
    for (const auto &pattern : spacePatterns) {
        suggestions.append(pattern);
    }
    
    // Add some smart combinations
    QStringList adjectives = {"Premium", "Elite", "Standard", "Basic", "Luxe", "Moderne", "Classique", "Industriel", "Commercial"};
    QStringList locations = {"Centre-ville", "Périphérie", "Zone industrielle", "Zone commerciale", "Zone résidentielle"};
    
    for (const QString &type : spacePatterns.keys()) {
        for (const QString &adj : adjectives) {
            suggestions << adj + " " + type;
        }
        for (const QString &loc : locations) {
            suggestions << type + " " + loc;
        }
    }
    
    return suggestions;
}
