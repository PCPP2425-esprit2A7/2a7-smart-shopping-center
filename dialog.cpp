#include "dialog.h"
#include "ui_dialog.h"
#include <qmessagebox.h>

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
        ui->desl->setText(event.getDescription());
        ui->ddl->setDate(event.getDateDebut());
        ui->dfl->setDate(event.getDateFin());
        ui->catl->setText(event.getCategorie());
        ui->statusl->setCurrentText(event.getStatut());
        ui->orgl->setText(event.getOrganisateur());
        ui->lieul->setText(event.getLieu());
    } else {
        QMessageBox::warning(this, "Erreur", "❌ Impossible de charger les détails de l'événement.");
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
    qDebug() << "Description :" << ui->desl->text();
    qDebug() << "Date début brute :" << ui->ddl->date().toString();
    qDebug() << "Date fin brute :" << ui->dfl->date().toString();
    qDebug() << "Catégorie :" << ui->catl->text();
    qDebug() << "Statut :" << ui->statusl->currentText();
    qDebug() << "Organisateur :" << ui->orgl->text();
    qDebug() << "Lieu :" << ui->lieul->text();  // Ajout du debug pour lieu

    // Vérification des champs obligatoires
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

    // Vérification de la capacité (valeur numérique)
    int capacite = ui->capl->text().toInt();
    if (capacite <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une capacité valide !");
        return;
    }

    // Appeler la fonction modifier() avec les valeurs du formulaire, y compris le lieu
    if (evenement.modifier(eventId,
                           ui->titrel->text(),
                           ui->typl->text(),
                           capacite,
                           prix,
                           ui->desl->text(),
                           ui->ddl->date().toString("dd-MM-yy"),  // Format de date pour Oracle
                           ui->dfl->date().toString("dd-MM-yy"),  // Format de date pour Oracle
                           ui->catl->text(),
                           ui->statusl->currentText(),
                           ui->orgl->text(),
                           ui->lieul->text()))  // Ajout du lieu dans modifier()
    {
        QMessageBox::information(this, "Succès", "Événement modifié avec succès !");
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'événement.");
    }
}



