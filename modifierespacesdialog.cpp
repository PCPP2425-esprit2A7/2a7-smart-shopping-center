#include "modifierespacesdialog.h"
#include "ui_modifierespacesdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

modifierespacesDialog::modifierespacesDialog(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifierespacesDialog),
    espaceID(id)
{
    ui->setupUi(this);
    chargerDonnees();
    connect(ui->enregistrer, &QPushButton::clicked, this, &modifierespacesDialog::on_enregistrer_clicked);

}

modifierespacesDialog::~modifierespacesDialog()
{
    delete ui;
}

void modifierespacesDialog::chargerDonnees()
{
    QSqlQuery query;
    query.prepare("SELECT NOM, TYPE, SUPERFICIE, EMPLACEMENT, STATUT, LOYER_M, DATE_DEB_CONTRAT, DATE_FIN "
                  "FROM espace WHERE ID = :id");
    query.bindValue(":id", espaceID);

    if (query.exec() && query.next()) {
        ui->lineEdit_Nom_2->setText(query.value("NOM").toString());
        ui->lineEdit_Type_2->setText(query.value("TYPE").toString());
        ui->spinBox_Superficie_2->setValue(query.value("SUPERFICIE").toDouble());
        ui->lineEdit_Emplacement_2->setText(query.value("EMPLACEMENT").toString());
        ui->comboBox_2->setCurrentText(query.value("STATUT").toString());
        ui->lineEdit_loyer_2->setText(query.value("LOYER_M").toString());
        QString dateDebStr = query.value("DATE_DEB_CONTRAT").toString();
        QString dateFinStr = query.value("DATE_FIN").toString();

        QDate dateDeb = QDate::fromString(dateDebStr, Qt::ISODate);
        QDate dateFin = QDate::fromString(dateFinStr, Qt::ISODate);

        if (!dateDeb.isValid()) {
            qDebug() << "Invalid start date, using current date.";
            dateDeb = QDate::currentDate();  // Mettre une date par défaut si nécessaire
        }

        if (!dateFin.isValid()) {
            qDebug() << "Invalid end date, using current date.";
            dateFin = QDate::currentDate();
        }

        ui->dateEdit_3->setDate(dateDeb);
        ui->dateEdit_4->setDate(dateFin);

    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données !");
        reject();
    }
}

void modifierespacesDialog::on_enregistrer_clicked()
{
    Espace espace;


    // Vérifier si le champ loyer est vide
    QString loyerStr = ui->lineEdit_loyer_2->text().trimmed().replace(",", ".");
    loyerStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    qDebug() << "Valeur après suppression des caractères invisibles :" << loyerStr;

    if (loyerStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un loyer !");
        return;
    }

    // Vérifier et convertir le loyer
    bool ok;
    double loyer = loyerStr.toDouble(&ok);
    qDebug() << "Valeur convertie en double :" << loyer;

    if (!ok) {
        qDebug() << "Erreur : la conversion a échoué !";
        QMessageBox::warning(this, "Erreur", "Le loyer doit être un nombre valide !");
        return;
    }

    // Modifier l'espace
    if (espace.modifier(espaceID,
                        ui->lineEdit_Nom_2->text(),
                        ui->lineEdit_Type_2->text(),
                        ui->spinBox_Superficie_2->text().toDouble(),
                        ui->lineEdit_Emplacement_2->text(),
                        ui->comboBox_2->currentText(),
                        loyer,
                        ui->dateEdit_3->date(),
                        ui->dateEdit_4->date())
                        ) {
        QMessageBox::information(this, "Succès", "Espace modifié avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'espace.");
    }
}
