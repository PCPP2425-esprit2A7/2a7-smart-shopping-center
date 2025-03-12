#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QPushButton>
#include "espace.h"
#include "modifierespacesDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons to slots
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::on_afficher_clicked);
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

// Function to display spaces
void MainWindow::afficherEspaces()
{
    Espace espaceTmp;
    QSqlQueryModel* model = espaceTmp.afficher();

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
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des espaces.");
    }
}

// Slot to display spaces when "afficher" button is clicked
void MainWindow::on_afficher_clicked()
{
    qDebug() << "Affichage des espaces...";
    afficherEspaces();
}

// Function to add a new space
void MainWindow::on_pushButton_15_clicked()
{
    // Get values from the form
    QString nom = ui->lineEdit_Nom->text();
    QString type = ui->lineEdit_Type->text();
    int superficie = ui->spinBox_Superficie->value();
    QString emplacement = ui->lineEdit_Emplacement->text();
    QString statut = ui->comboBox->currentText();
    QString dateDebut = ui->dateEdit->date().toString("yyyy-MM-dd"); // Using yyyy-MM-dd format for consistency
    QString dateFin = ui->dateEdit_2->date().toString("yyyy-MM-dd"); // Using yyyy-MM-dd format for consistency

    // Check if loyer field is empty
    QString loyerStr = ui->lineEdit_loyer->text().replace(",", ".");
    loyerStr.remove(QChar(0x200E));

    qDebug() << "Valeur après suppression des caractères invisibles :" << loyerStr;

    if (loyerStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un loyer !");
        return;
    }

    // Check and convert loyer
    bool ok;
    double loyer = loyerStr.toDouble(&ok);
    qDebug() << "Valeur convertie en double :" << loyer;

    if (!ok || loyer <= 0) {
        qDebug() << "Erreur : la conversion a échoué !";
        QMessageBox::warning(this, "Erreur", "Le loyer doit être un nombre valide et supérieur à 0 !");
        return;
    }

    // Check consistency of dates
    QDate startDate = ui->dateEdit->date();
    QDate endDate = ui->dateEdit_2->date();

    if (startDate >= endDate) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être après la date de début !");
        return;
    }

    // Create Espace and add to database
    Espace Espace(nom, type, superficie, emplacement, statut, loyer, startDate, endDate, 1);
    if (Espace.ajouter()) {
        QMessageBox::information(this, "Succès", "Espace ajouté avec succès !");

        // Clear the form
        ui->lineEdit_Nom->clear();
        ui->lineEdit_Type->clear();
        ui->spinBox_Superficie->setValue(1);
        ui->lineEdit_Emplacement->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->lineEdit_loyer->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit_2->setDate(QDate::currentDate().addYears(1));

        // Switch to "Liste" tab
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'espace !");
    }
}

// Function to delete an Espace
void MainWindow::on_supprimer_clicked()
{
    // Get the selected row from the table view
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un espace à supprimer !");
        return;
    }

    // Get the ID of the selected space (assumes ID is in the first column)
    int selectedRow = selectedRows.first().row();
    int espaceID = ui->tableView->model()->data(ui->tableView->model()->index(selectedRow, 0)).toInt();

    // Show confirmation dialog
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet espace ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // If confirmed, delete the space
        Espace espaceTmp;
        if (espaceTmp.supprimer(espaceID)) {
            QMessageBox::information(this, "Succès", "Espace supprimé avec succès !");
            afficherEspaces();  // Refresh the table
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'espace !");
        }
    }
}

// Function to modify an Espace
void MainWindow::on_modifier_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un espace à modifier !");
        return;
    }

    int selectedRow = selectedRows.first().row();
    int espaceID = ui->tableView->model()->data(ui->tableView->model()->index(selectedRow, 0)).toInt();

    modifierespacesDialog dialog(espaceID, this);
    if (dialog.exec() == QDialog::Accepted) {
        afficherEspaces();
    }
}
