#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "service.h"
#include <QMessageBox>
#include <QDebug>
#include "modifierservicedialog.h"

void MainWindow::afficherServices() {
    Service service;
    QSqlQueryModel* model = service.afficher();

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
void MainWindow::supprimerService(int idService)
{
    // Créer une requête SQL pour supprimer un service
    QSqlQuery query;
    query.prepare("DELETE FROM SERVICE WHERE id = :id");
    query.bindValue(":id", idService);  // Remplacer ":id" par l'ID du service

    // Exécuter la requête
    if (query.exec()) {
        // Si la suppression réussit, afficher un message
        QMessageBox::information(this, "Succès", "Le service a été supprimé avec succès.");

        // Rafraîchir la liste des services pour refléter la suppression
        afficherServices();
    } else {
        // Si la suppression échoue, afficher un message d'erreur
        QMessageBox::critical(this, "Erreur", "Échec de la suppression : " + query.lastError().text());
    }
}

void MainWindow::on_deleteButton_clicked()
{
    // Récupérer l'ID du service sélectionné dans la QTableView
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un service à supprimer.");
        return;
    }

    // Récupérer l'ID du service depuis la première colonne de la ligne sélectionnée
    int idService = selectedRows.first().data().toInt();  // Exemple avec la première colonne qui contient l'ID

    // Appeler la fonction pour supprimer le service
    supprimerService(idService);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Cacher la barre des onglets pour forcer l'utilisation des boutons
    ui->tabWidget->tabBar()->hide();
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherServices);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);
    connect(ui->modifyButton, &QPushButton::clicked, this, &MainWindow::on_modifyButton_clicked);



    // Connexion des boutons pour changer d'onglet
    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->form, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Retour à l'onglet Affichage";
    });

    connect(ui->stat
            , &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_2_clicked()
{
    // Récupérer les valeurs des champs du formulaire
    QString nom = ui->lineEdit_Nom_2->text().trimmed();
    QString description = ui->textEdit_Description_2->toPlainText().trimmed();
    QString statut = ui->comboBox_Statut_2->currentText();
    QString frequence = ui->comboBox_Frequence_2->currentText();
    QString dateDebut = ui->dateEdit_Debut_2->date().toString("dd-MM-yy");
    QString dateFin = ui->dateEdit_Fin_2->date().toString("dd-MM-yy");

    // Vérifier si le champ coût est vide
    QString coutStr = ui->lineEdit_Cout_2->text().trimmed().replace(",", ".");
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

    // Création du service et tentative d'ajout
    Service s(nom, description, cout, frequence, statut, dateDebut, dateFin, 1);
    if (s.ajouter()) {
        QMessageBox::information(this, "Succès", "Service ajouté avec succès !");

        // Nettoyage du formulaire après ajout
        ui->lineEdit_Nom_2->clear();
        ui->textEdit_Description_2->clear();
        ui->lineEdit_Cout_2->clear();
        ui->comboBox_Statut_2->setCurrentIndex(0);
        ui->comboBox_Frequence_2->setCurrentIndex(0);
        ui->dateEdit_Debut_2->setDate(QDate::currentDate());
        ui->dateEdit_Fin_2->setDate(QDate::currentDate());

        // Passage automatique à l'onglet "Liste"
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du service !");
    }
}













void MainWindow::on_modifyButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un service à modifier.");
        return;
    }

    // Vérifier que l'ID est valide
    int idService = selectedRows.first().data().toInt();
    if (idService <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide pour le service.");
        return;
    }

    // Ouvrir la fenêtre de modification en passant l'ID du service
    ModifierServiceDialog dialog(idService, this);
    dialog.exec();  // Cette fonction bloquera l'exécution jusqu'à la fermeture de la fenêtre
}



