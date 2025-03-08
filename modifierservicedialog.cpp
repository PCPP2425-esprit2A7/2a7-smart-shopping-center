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
    loadServiceDetails();
    connect(ui->saveButton, &QPushButton::clicked, this, &ModifierServiceDialog::on_saveButton_clicked);


}

ModifierServiceDialog::~ModifierServiceDialog()
{
    delete ui;
}

void ModifierServiceDialog::loadServiceDetails()
{
    Service service;
    if (service.loadById(serviceId)) {  // Charger les détails du service sélectionné
        ui->lineEdit_Nom->setText(service.getNom());
        ui->textEdit_Description->setPlainText(service.getDescription());
        ui->lineEdit_Cout->setText(QString::number(service.getCout()));
        ui->comboBox_Frequence->setCurrentText(service.getFrequence());
        ui->comboBox_Statut->setCurrentText(service.getStatut());
        ui->dateEdit_Debut->setDate(QDate::fromString(service.getDateDebut(), "DD-MM-YY"));
        ui->dateEdit_Fin->setDate(QDate::fromString(service.getDateFin(), "DD-MM-YY"));
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les détails du service.");
    }
}



void ModifierServiceDialog::on_saveButton_clicked()
{
    Service service;
    qDebug() << "Nom du service :" << service.getNom();
    qDebug() << "Description :" << service.getDescription();
    qDebug() << "Coût :" << service.getCout();
    qDebug() << "Fréquence :" << service.getFrequence();
    qDebug() << "Statut :" << service.getStatut();
    qDebug() << "Date début brute :" << service.getDateDebut();
    qDebug() << "Date fin brute :" << service.getDateFin();

    // Vérifier si le champ coût est vide
    QString coutStr = ui->lineEdit_Cout->text().trimmed().replace(",", ".");
    coutStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    qDebug() << "Valeur après suppression des caractères invisibles :" << coutStr;

    if (coutStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un coût !");
        return;
    }

    // Vérifier et convertir le coût
    bool ok;
    double cout = coutStr.toDouble(&ok);
    qDebug() << "Valeur convertie en double :" << cout;

    if (!ok) {
        qDebug() << "Erreur : la conversion a échoué !";
        QMessageBox::warning(this, "Erreur", "Le coût doit être un nombre valide !");
        return;
    }
    if (service.modifier(serviceId, ui->lineEdit_Nom->text(),
                         ui->textEdit_Description->toPlainText(),
                         cout,
                         ui->comboBox_Frequence->currentText(),
                         ui->comboBox_Statut->currentText(),
                         ui->dateEdit_Debut->date().toString("dd-MM-yy"),
                         ui->dateEdit_Fin->date().toString("dd-MM-yy"))) {
        QMessageBox::information(this, "Succès", "Service modifié avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du service.");
    }


}
