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

void ModifierDialog::accept() {
    // Récupération des valeurs
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephone = ui->telephone->text().trimmed();
    QString poste = ui->poste->text().trimmed();
    QString statut = ui->statut->text().trimmed();
    QString sexe = ui->sexe->currentText().trimmed();
    QDate dateEmbauche = ui->date_embauche->date();

    // Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Vérification du format de l'email
    QRegularExpression regexEmail("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::critical(this, "Erreur", "L'email n'est pas valide !");
        return;
    }

    // Vérification du numéro de téléphone (8 à 12 chiffres)
    QRegularExpression regexTel("^[0-9]{8,12}$");
    if (!regexTel.match(telephone).hasMatch()) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone doit contenir entre 8 et 12 chiffres !");
        return;
    }

    // Vérification du salaire
    bool ok;
    double salaire = ui->salaire->text().toDouble(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide et positif.");
        return;
    }

    // Création de l'objet Employe après validation
    Employe employe;
    employe.setNom(nom);
    employe.setPrenom(prenom);
    employe.setDateEmbauche(dateEmbauche);
    employe.setPoste(poste);
    employe.setSalaire(salaire);
    employe.setEmail(email);
    employe.setSexe(sexe);
    employe.setTelephone(telephone);
    employe.setStatut(statut);

    // Ajout de la conservation de la photo (si nécessaire)
    // employe.setPdp(this->employe.getPdp()); // Si la classe Employe gère une photo de profil

    // ✅ Affectation correcte de l'objet Employe
    this->employeModifie = employe;

    // ✅ Fermeture de la fenêtre UNIQUEMENT si tout est valide
    QDialog::accept();
}

// ✅ Fonction pour récupérer l'employé modifié
Employe ModifierDialog::getEmployeModifie() const {
    return employeModifie;
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
