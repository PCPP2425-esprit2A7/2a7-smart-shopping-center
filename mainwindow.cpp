#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "locataire.h"
#include <QMessageBox>
#include <QDebug>
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion des boutons aux slots
    //connect(ui->btnAjouterLocataire, &QPushButton::clicked, this, &MainWindow::on_btnAjouterLocataire_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherLocataires);


    // Cacher la barre des onglets
    ui->tabWidget->tabBar()->hide();

    // Connexions des boutons pour changer d'onglet
    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Liste";
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Ajout";
    });

    connect(ui->stat, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAjouterLocataire_clicked()
{
    qDebug() << "Tentative d'ajout d'un locataire...";
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString typeact = ui->typeac->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephoneStr = ui->tel->text().trimmed();
    int telephone = telephoneStr.toInt();
    QDate datedeb = ui->datedeb->date();
    QDate datefin = ui->datefin->date();
    QString duree = ui->duree->text().trimmed();
    QString paiement = ui->paiement->text().trimmed();

    // Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || typeact.isEmpty() || email.isEmpty() || telephoneStr.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // Vérification du numéro de téléphone valide
    if (telephoneStr.length() < 8 || telephone == 0) {
        QMessageBox::critical(this, "Erreur", "Veuillez entrer un numéro de téléphone valide.");
        return;
    }

    // Création et ajout du locataire
    Locataire locataire(nom, prenom, typeact, email, telephone, duree, datedeb, datefin, paiement);

    if (locataire.ajouter()) {
        QMessageBox::information(this, "Succès", "Locataire ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du locataire !");
    }
}
void MainWindow::afficherLocataires()
{
    qDebug() << "Tentative d'affichage des locataires...";

    Locataire Ltmp;
    QSqlQueryModel* model = Ltmp.afficher();

    if (model) {
        qDebug() << "Modèle récupéré, nombre de lignes :" << model->rowCount();

        if (model->rowCount() > 0) {
            ui->tableView->setModel(model);
            ui->tableView->resizeColumnsToContents();
            qDebug() << "Affichage réussi !";
        } else {
            qDebug() << "Aucune donnée trouvée dans la table.";
            QMessageBox::information(this, "Info", "Aucun locataire trouvé dans la base de données.");
        }
    } else {
        qDebug() << "Erreur : modèle non récupéré.";
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des locataires.");
    }
}

void MainWindow::on_btnSupprimerLocataire_clicked()
{
    // Récupérer la ligne sélectionnée dans le QTableView
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un locataire à supprimer.");
        return;
    }

    // Récupérer l'ID du locataire sélectionné (première colonne du tableau)
    int id = selection.first().data().toInt();

    // Supprimer le locataire
    Locataire loc;
    if (loc.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Locataire supprimé avec succès !");
        afficherLocataires(); // 🔥 Met à jour la liste
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer le locataire !");
    }
}

void MainWindow::on_btnModifierLocataire_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();  // Récupérer le modèle de sélection
    QModelIndexList selectedRows = select->selectedRows();  // Obtenir la liste des lignes sélectionnées

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un locataire à modifier.");
        return;
    }

    // Vérifier que l'ID du locataire est valide
    int idLocataire = selectedRows.first().data().toInt();  // Extraire l'ID de la ligne sélectionnée
    if (idLocataire <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide pour le locataire.");
        return;
    }

    // Ouvrir la fenêtre de modification en passant l'ID du locataire
    Dialog dialog(idLocataire, this);  // Modifier l'appel pour utiliser ModifierLocataireDialog
    dialog.exec();  // Cette fonction bloquera l'exécution jusqu'à la fermeture de la fenêtre
}



