#include "modifierdialog.h"
#include "ui_modifierdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QBuffer>

ModifierDialog::ModifierDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifierDialog)
{
    ui->setupUi(this);

    // Connexion des boutons
    //connect(ui->pushButton_modifierPhoto, &QPushButton::clicked, this, &ModifierDialog::on_pushButton_modifierPhoto_clicked);
    //connect(ui->pushButton_enregistrer, &QPushButton::clicked, this, &ModifierDialog::on_pushButton_enregistrer_clicked);

}


ModifierDialog::~ModifierDialog()
{
    delete ui;
}

void ModifierDialog::setEmploye(const Employe &employe)
{
    this->employe = employe;

    ui->nom->setText(employe.getNom());
    ui->prenom->setText(employe.getPrenom());
    ui->date_embauche->setDate(employe.getDateEmbauche());
    ui->poste->setText(employe.getPoste());
    ui->salaire->setText(QString::number(employe.getSalaire()));
    ui->email->setText(employe.getEmail());
    ui->sexe->setCurrentText(employe.getSexe());
    ui->telephone->setText(employe.getTelephone());
    ui->statut->setText(employe.getStatut());

    // ✅ Conversion de QByteArray vers QPixmap
    if (!employe.getPdp().isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(employe.getPdp());  // 🔥 Vérifie que la conversion est réussie

        if (!pixmap.isNull()) {
            ui->pushButton_pdp1->setIcon(QIcon(pixmap));
            ui->pushButton_pdp1->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2->setIconSize(QSize(50, 50));
        } else {
            qDebug() << "❌ Erreur : Impossible de charger l'image depuis les données binaires.";
        }
    } else {
        qDebug() << "⚠️ Aucune donnée d'image disponible.";
    }
}

Employe ModifierDialog::getEmployeModifie() const
{
    Employe employe;
    employe.setNom(ui->nom->text());
    employe.setPrenom(ui->prenom->text());
    employe.setDateEmbauche(ui->date_embauche->date());
    employe.setPoste(ui->poste->text());
    employe.setSalaire(ui->salaire->text().toDouble());
    employe.setEmail(ui->email->text());
    employe.setSexe(ui->sexe->currentText());
    employe.setTelephone(ui->telephone->text());
    employe.setStatut(ui->statut->text());

    // 🔥 Conserver le QByteArray de la photo de profil
    employe.setPdp(this->employe.getPdp());

    return employe;
}

// 🔥 Bouton "Modifier Photo"
void ModifierDialog::on_pushButton_modifierPhoto_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            // 🔥 Mettre à jour l'aperçu de l'image dans le bouton
            ui->pushButton_pdp1->setIcon(QIcon(pixmap));
            ui->pushButton_pdp1->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2->setIconSize(QSize(50, 50));
            ui->pushButton_pdp2->setIconSize(QSize(50, 50));
            ui->pushButton_pdp2->setStyleSheet("border-radius: 25px; border: none;");

            // 🔥 Convertir l'image en QByteArray pour la sauvegarde
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");

            // 🔥 Stocker le QByteArray dans l'objet Employe
            employe.setPdp(byteArray);
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image sélectionnée.");
        }
    }
}

// 🔥 Bouton "Enregistrer"
void ModifierDialog::on_pushButton_enregistrer_clicked()
{
    if (ui->nom->text().isEmpty() || ui->prenom->text().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis !");
        return;
    }

    accept(); // Ferme la boîte de dialogue et retourne QDialog::Accepted
}
