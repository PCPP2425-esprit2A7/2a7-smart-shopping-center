#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "locataire.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion des boutons aux slots
    connect(ui->btnAjouterLocataire, &QPushButton::clicked, this, &MainWindow::on_btnAjouterLocataire_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAjouterLocataire_clicked()
{
    qDebug() << "Tentative d'ajout d'un locataire...";

    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString typeact = ui->typeac->text();
    QString email = ui->email->text();
    QString telephone = ui->tel->text();
    QDate datedeb = ui->datedeb->date();
    QDate datefin = ui->datefin->date();
    QString duree = ui->duree->text();
    QString paiement = ui->paiement->text();

    // Vérification que les champs obligatoires ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || typeact.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Création et ajout du locataire avec ID
    Locataire locataire(nom, prenom, typeact, email, telephone, duree, datedeb, datefin, paiement);

    if (locataire.ajouter()) {
        QMessageBox::information(this, "Succès", "Locataire ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du locataire !");
    }
}
