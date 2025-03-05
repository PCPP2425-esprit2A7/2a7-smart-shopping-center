#include "modifierservicedialog.h"
#include "ui_modifierservicedialog.h"
#include "service.h"
#include <QMessageBox>


ModifierServiceDialog::ModifierServiceDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifierServiceDialog),
    serviceId(id)  // Stocke l'ID du service
{
    ui->setupUi(this);  // S'assurer que l'UI est bien liée
}

ModifierServiceDialog::~ModifierServiceDialog()
{
    delete ui;
}

void ModifierServiceDialog::loadServiceDetails()
{
    // Créer un objet Service avec l'ID et récupérer les détails du service à modifier
    Service service;
    if (service.loadById(serviceId)) {
        ui->lineEdit_Nom->setText(service.getNom());
        ui->textEdit_Description->setPlainText(service.getDescription());
        ui->lineEdit_Cout->setText(QString::number(service.getCout()));
        ui->comboBox_Frequence->setCurrentText(service.getFrequence());
        ui->comboBox_Statut->setCurrentText(service.getStatut());
        ui->dateEdit_Debut->setDate(QDate::fromString(service.getDateDebut(), "dd-MM-yy"));
        ui->dateEdit_Fin->setDate(QDate::fromString(service.getDateFin(), "dd-MM-yy"));
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les détails du service.");
    }
}

void ModifierServiceDialog::on_saveButton_clicked()
{
    // Récupérer les valeurs des champs
    QString nom = ui->lineEdit_Nom->text();
    QString description = ui->textEdit_Description->toPlainText();
    double cout = ui->lineEdit_Cout->text().toDouble();
    QString frequence = ui->comboBox_Frequence->currentText();
    QString statut = ui->comboBox_Statut->currentText();
    QString dateDebut = ui->dateEdit_Debut->date().toString("dd-MM-yy");
    QString dateFin = ui->dateEdit_Fin->date().toString("dd-MM-yy");

    // Appeler une méthode de la classe `Service` pour enregistrer la modification
    Service service;
    if (service.modifier(serviceId, nom, description, cout, frequence, statut, dateDebut, dateFin)) {
        QMessageBox::information(this, "Succès", "Service modifié avec succès.");
        accept(); // Fermer la fenêtre de modification
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification du service.");
    }
}
