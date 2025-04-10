#include "dialog.h"
#include "ui_dialog.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include <QBuffer>

Dialog::Dialog(int id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    eventId(id)
      // Initialisation de l'UI
{
    ui->setupUi(this);  // Charge le fichier .ui dans la fenêtre
    loadEventDetails();
    connect(ui->enregistrer, &QPushButton::clicked, this, &Dialog::on_enregistrer_clicked);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::loadEventDetails()
{
    Evenement event;
    if (event.loadById(eventId)) {  // Charger les détails de l'événement sélectionné
        ui->titrel->setText(event.getTitre());
        ui->typl->setText(event.getType());
        ui->capl->setText(QString::number(event.getCapacite()));
        ui->prixl->setValue(event.getPrix());
        ui->desl->setPlainText(event.getDescription());
        ui->ddl->setDate(event.getDateDebut());
        ui->dfl->setDate(event.getDateFin());
        ui->catl->setText(event.getCategorie());
        ui->statusl->setCurrentText(event.getStatut());
        ui->orgl->setText(event.getOrganisateur());
        ui->lieul->setText(QString::number(event.getid_espace()));

        // ✅ Vérification et chargement de l'affiche
        QString affichePath = event.getAffiche(); // Récupérer l'affiche en QString

        if (!affichePath.isEmpty()) {
            QPixmap pixmap;

            // Vérifier si affichePath est un chemin valide vers une image
            if (pixmap.load(affichePath)) {
                ui->pushButton_affiche->setIcon(QIcon(pixmap));
                ui->pushButton_affiche->setIconSize(QSize(150, 150));
          } else {
                qDebug() << " Erreur : Impossible de charger l'image depuis le chemin spécifié :" << affichePath;
            }
        } else {
            qDebug() << " Aucune donnée d'image disponible.";
        }
    } else {
        QMessageBox::warning(this, "Erreur", " Impossible de charger les détails de l'événement.");
    }
}

//  Bouton "Modifier Photo"
void Dialog::on_modifier_affiche_clicked()
{
    Evenement event;
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            //  Mettre à jour l'aperçu de l'image dans le bouton
            ui->pushButton_affiche->setIcon(QIcon(pixmap));
            ui->pushButton_affiche->setIconSize(QSize(150, 150));


            //  Convertir l'image en QByteArray pour la sauvegarde
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");

            //  Stocker le QByteArray dans l'objet evenement
            event.setAffiche(byteArray);
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image sélectionnée.");
        }
    }


}

void Dialog::on_enregistrer_clicked()
{
    // Créer une instance d'Evenement (à adapter selon ta classe)
    Evenement evenement;

    // Récupérer les informations du formulaire et afficher les valeurs dans les logs
    qDebug() << "Titre de l'événement :" << ui->titrel->text();
    qDebug() << "Type :" << ui->typl->text();
    qDebug() << "Capacité :" << ui->capl->text();
    qDebug() << "Prix :" << ui->prixl->value();
    qDebug() << "Description :" << ui->desl->toPlainText();
    qDebug() << "Date début brute :" << ui->ddl->date().toString();
    qDebug() << "Date fin brute :" << ui->dfl->date().toString();
    qDebug() << "Catégorie :" << ui->catl->text();
    qDebug() << "Statut :" << ui->statusl->currentText();
    qDebug() << "Organisateur :" << ui->orgl->text();
    qDebug() << "id_espace :" << ui->lieul->text();  // Ajout du debug pour lieu

    // Vérification des champs obligatoires
    if (ui->titrel->text().isEmpty() || ui->desl->toPlainText().isEmpty() || ui->catl->text().isEmpty() ||
        ui->typl->text().isEmpty() || ui->orgl->text().isEmpty() || ui->lieul->text().isEmpty() ||
        ui->statusl->currentText().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return; // Arrêter l'exécution de la fonction si un champ est vide
    }

    // Vérification du prix (doit être un nombre positif)
    QString prixStr = QString::number(ui->prixl->value()).trimmed().replace(",", ".");
    prixStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    if (prixStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un prix !");
        return;
    }

    // Vérifier et convertir le prix en double
    bool ok;
    double prix = prixStr.toDouble(&ok);
    qDebug() << "Valeur convertie en double pour le prix :" << prix;

    if (!ok) {
        qDebug() << "Erreur : la conversion du prix a échoué !";
        QMessageBox::warning(this, "Erreur", "Le prix doit être un nombre valide !");
        return;
    }

    // Contrôle que le prix est positif
    if (prix <= 0) {
        QMessageBox::warning(this, "Erreur", "Le prix doit être positif !");
        return;
    }

    // Vérification de la capacité (doit être un nombre et supérieur à 10)
    int capacite = ui->capl->text().toInt();
    if (capacite <= 10) {
        QMessageBox::warning(this, "Erreur", "La capacité doit être supérieure à 10 !");
        return;
    }
    int lieu = ui->lieul->text().toInt();

    // Vérification de la date de fin (doit être après la date de début)
    QDate dateDebut = ui->ddl->date();
    QDate dateFin = ui->dfl->date();
    if (dateFin <= dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être supérieure à la date de début !");
        return;
    }

    // Appeler la fonction modifier() avec les valeurs du formulaire, y compris le lieu
    if (evenement.modifier(eventId,
                           ui->titrel->text(),
                           ui->typl->text(),
                           capacite,
                           prix,
                           ui->desl->toPlainText(),
                           ui->ddl->date().toString("dd-MM-yy"),  // Format de date pour Oracle
                           ui->dfl->date().toString("dd-MM-yy"),  // Format de date pour Oracle
                           ui->catl->text(),
                           ui->statusl->currentText(),
                           ui->orgl->text(),
                           lieu))  // Ajout du lieu dans modifier()
    {
        QMessageBox::information(this, "Succès", "Événement modifié avec succès !");
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'événement.");
    }
}
