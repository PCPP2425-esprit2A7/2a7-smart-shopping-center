#include "modifierlocatairedialog.h"

ModifierLocataireDialog::ModifierLocataireDialog(int locataireId, QWidget *parent)
    : QDialog(parent), ui(new Ui::ModifierLocataireDialog), idLocataire(locataireId)
{
    ui->setupUi(this);  // Vérifiez que cette ligne est bien présente
}

ModifierLocataireDialog::~ModifierLocataireDialog()
{
    delete ui;
}



void ModifierLocataireDialog::on_btnModifier_clicked()
{
    // Récupérer les nouvelles valeurs et effectuer les validations
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephone = ui->tel->text().trimmed();
    QDate datedeb = ui->datedeb->date();
    QDate datefin = ui->datefin->date();
    QString duree = ui->duree->text().trimmed();
    QString paiement = ui->paiement->text().trimmed();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Modifier les données du locataire
    locataire.setNom(nom);
    locataire.setPrenom(prenom);
    locataire.setEmail(email);
    locataire.setTelephone(tel);
    locataire.setDateDeb(datedeb);
    locataire.setDateFin(datefin);
    locataire.setDureeContrat(duree);
    locataire.setPaiementEffectue(paiement);

    if (locataire.modifier(idLocataire)) {
        QMessageBox::information(this, "Succès", "Locataire modifié avec succès !");
        accept();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du locataire.");
    }
}
