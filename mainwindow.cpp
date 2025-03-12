#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"
#include "dialog.h"
#include <QMessageBox>
#include <QTabBar>  // Ajouté pour éviter l'erreur
#include <QDebug>   // Pour utiliser qDebug()
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);  // Initialisation de l'interface
    afficherEvenement();
    connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::afficherEvenement);
    connect(ui->btnDialog, &QPushButton::clicked, this, &MainWindow::on_btnDialog_clicked);



    // Cacher la barre des onglets pour forcer l'utilisateur à utiliser les boutons
    ui->tabWidget->tabBar()->hide();

    // Connexion des boutons pour changer d'onglet
    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Affichage";
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


void MainWindow::on_ajouter_button_clicked()
{
    // Récupérer les valeurs des champs
    QString titre = ui->titrel->text();
    QString description = ui->desl->text(); // Utiliser text() pour QLineEdit
    QDate dateDebut = ui->ddl->date();
    QDate dateFin = ui->dfl->date();
    int capacite = ui->capl->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl->value();
    QString statut = ui->statusl->currentText();
    QString categorie = ui->catl->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl->text();
    QString lieu = ui->lieul->text(); // Ajout du champ lieu (assurez-vous que lieul existe dans l'UI)

    // Créer l’objet Evenement avec le lieu ajouté
    Evenement ev(titre, type, capacite, prix, lieu, description, dateDebut, dateFin, categorie, statut, organisateur, QString::number(1));

    // Appeler la méthode ajouter()
    if (ev.ajouter()) {
        QMessageBox::information(this, "Succès", "Événement ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout de l’événement !");
    }
}

void MainWindow::on_pushButton_choisirImage_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->affichel->setPixmap(pixmap);  // Utilise setPixmap pour un QLabel
            ui->affichel->setScaledContents(true);  // Permet à l'image de s'adapter à la taille du QLabel
            ui->affichel->setFixedSize(150, 150);  // Fixe la taille du QLabel à 150x150
            cheminImagePDP = filePath;
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
        }
    }
}
void MainWindow::afficherEvenement() {
    Evenement ev;
    QSqlQueryModel* model = ev.afficher();

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->setStyleSheet(
            "QTableView {"
            "   background-color: #f5f5f5;"
            "   border: 1px solid #ccc;"
            "   gridline-color: #ccc;"
            "   selection-background-color: #A3C1DA;"
            "   selection-color: white;"
            "   font: bold 12px;"
            "}"
            "QTableView::item {"
            "   padding: 10px;"
            "   border-bottom: 1px solid #ddd;"
            "}"
            "QTableView::item:selected {"
            "   background-color: #6fa3ef;"
            "}"
            "QHeaderView::section {"
            "   background-color: #2e3d4e;"
            "   color: white;"
            "   padding: 5px;"
            "   border: 1px solid #aaa;"
            "}"
            );
        qDebug() << "Liste des services chargée avec succès.";
    } else {
        qDebug() << "Erreur lors du chargement des services.";
    }

}


void MainWindow::on_supprimer_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à supprimer.");
        return;
    }

    // Récupérer l'ID depuis la première colonne (colonne 0)
    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    // Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet événement ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Evenement E;
        if (E.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Événement supprimé avec succès.");
            // Rafraîchir l'affichage après suppression
            afficherEvenement();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'événement.");
        }
    }
}





void MainWindow::on_btnDialog_clicked()
{
    // Récupérer le modèle de sélection de la table
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();

    // Vérifier si une ligne est sélectionnée
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un service à modifier.");
        return;
    }

    // Récupérer l'ID du service sélectionné depuis la première colonne
    int idEvent = selectedRows.first().data().toInt();

    // Vérifier que l'ID est valide
    if (idEvent <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide pour le service.");
        return;
    }

    // Ouvrir la fenêtre de modification avec l'ID du service
    Dialog dialog(idEvent, this);

    // Afficher le dialogue pour permettre la modification
    if (dialog.exec() == QDialog::Accepted) {
        // Si le dialogue est validé, effectuer des actions supplémentaires si nécessaire
        // Par exemple, recharger ou rafraîchir les données de la table
    }
}



