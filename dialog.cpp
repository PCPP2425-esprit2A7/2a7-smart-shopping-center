#include "dialog.h"
#include "locataire.h"  // Inclure ta classe Locataire
#include "ui_dialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>


Dialog::Dialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),  // Assure-toi que "Ui::Dialog" est bien défini
    locataireId(id)  // Stocker l'ID du locataire pour l'utiliser dans le dialogue
{
    ui->setupUi(this);  // Lier l'interface graphique
    loadLocataireDetails();  // Charger les informations du locataire à l'ouverture du dialogue
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::loadLocataireDetails()
{
    Locataire locataire;  // Crée une instance de Locataire
    if (locataire.loadById(locataireId)) {  // Charger les détails du locataire sélectionné
        // Remplir les champs de l'UI avec les données récupérées
        ui->nom->setText(locataire.getNom());
        ui->prenom->setText(locataire.getPrenom());
        ui->typeac->setText(locataire.getTypeActivite());
        ui->email->setText(locataire.getEmail());
        ui->tel->setText(locataire.getTelephone());  // Now it's a QString
        ui->duree->setText(locataire.getDureeContrat());
        ui->datedeb->setDate(QDate::fromString(locataire.getDateDeb(), "yyyy-MM-dd"));  // Now the date is a QString
        ui->datefin->setDate(QDate::fromString(locataire.getDateFin(), "yyyy-MM-dd"));
        ui->paiement->setText(locataire.getPaiementEffectue());
        // Si le contrat est stocké dans un QByteArray
        ui->contrat->setText(QString::fromUtf8(locataire.getContrat()));  // Si c'est en UTF-8

    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les détails du locataire.");
    }
}

// Code de votre méthode on_saveButton_clicked()
void Dialog::on_saveButton_clicked()
{
    Locataire locataire;

    // Récupérer les valeurs des champs
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString typeact = ui->typeac->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephoneStr = ui->tel->text().trimmed();
    bool ok;
    int telephone = telephoneStr.toInt(&ok);

    qDebug() << "Nom: " << nom;
    qDebug() << "Prénom: " << prenom;
    qDebug() << "Type Activité: " << typeact;
    qDebug() << "Email: " << email;
    qDebug() << "Téléphone: " << telephone;

    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone est invalide !");
        return;
    }

    QDate datedeb = ui->datedeb->date();
    QDate datefin = ui->datefin->date();
    QString duree = ui->duree->text().trimmed();
    QString paiement = ui->paiement->text().trimmed();

    // Lire le contrat (fichier image ou document) en QByteArray
    QByteArray contrat;
    QString cheminContrat = ui->contrat->text();  // Assurez-vous que ce champ contient le chemin du fichier
    if (!cheminContrat.isEmpty()) {
        QFile file(cheminContrat);
        if (file.open(QIODevice::ReadOnly)) {
            contrat = file.readAll();
            file.close();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de lire le fichier du contrat.");
            return;
        }
    }

    qDebug() << "Date Début: " << datedeb.toString("yyyy-MM-dd");
    qDebug() << "Date Fin: " << datefin.toString("yyyy-MM-dd");

    // Vérification des champs nécessaires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    if (!datedeb.isValid() || !datefin.isValid()) {
        QMessageBox::warning(this, "Erreur", "Les dates doivent être valides !");
        return;
    }

    if (datedeb > datefin) {
        QMessageBox::warning(this, "Erreur", "La date de début ne peut pas être après la date de fin !");
        return;
    }

    // Mettre à jour les informations du locataire
    locataire.setId(locataireId);
    locataire.setNom(nom);
    locataire.setPrenom(prenom);
    locataire.setTypeActivite(typeact);
    locataire.setEmail(email);
    locataire.setTelephone(telephone);
    locataire.setDateDeb(datedeb);
    locataire.setDateFin(datefin);
    locataire.setDureeContrat(duree);
    locataire.setPaiementEffectue(paiement);

    // Appel de la méthode modifier avec le QByteArray du contrat
    if (locataire.modifier(locataireId, nom, prenom, typeact, email, QString::number(telephone),
                           duree, datedeb.toString("yyyy-MM-dd"), datefin.toString("yyyy-MM-dd"), paiement, contrat)) {
        QMessageBox::information(this, "Succès", "Les informations du locataire ont été mises à jour avec succès.");
        this->accept();  // Fermer le dialogue si la mise à jour a réussi
    } else {
        QMessageBox::warning(this, "Erreur", "La mise à jour des informations a échoué.");
    }
}
