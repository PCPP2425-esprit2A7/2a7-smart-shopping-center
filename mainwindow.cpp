#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cheminImagePDP("")
{
    ui->setupUi(this);

    // 🔥 Charger les employés dès le démarrage
    afficherEmployes();

    // Connexion des boutons aux slots
    connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherEmployes);
    // Dans le constructeur de MainWindow

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

// 🔥 Fonction corrigée pour afficher les employés
void MainWindow::afficherEmployes()
{
    Employe Etmp;
    QSqlQueryModel* model = Etmp.afficher();  // ✅ Correction ici

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
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des employés.");
    }
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    qDebug() << "Tentative d'ajout d'un employé...";

    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QDate date_embauche = ui->date_embauche->date();
    QString poste = ui->poste->text();
    QString email = ui->email->text();
    QString sexe = ui->sexe->currentText();
    QString telephone = ui->telephone->text();
    QString statut = ui->statut->text();

    // Vérification que les champs obligatoires ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    bool ok;
    int salaire = ui->salaire->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide.");
        return;
    }

    qDebug() << "Nom:" << nom << ", Prénom:" << prenom << ", Salaire:" << salaire;

    // Récupérer le chemin de l'image de profil (si applicable)
    QString cheminImagePDP = "chemin/vers/limage.jpg";  // Remplacez cela par votre méthode pour obtenir le chemin de l'image

    // Création et ajout de l'employé
    Employe employe(nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, cheminImagePDP, statut);

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        afficherEmployes();  // Met à jour la table après l'ajout
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
    }
}

void MainWindow::on_pushButton_choisirImage_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_pdp->setIcon(QIcon(pixmap));
            ui->pushButton_pdp->setIconSize(QSize(150, 150));
            cheminImagePDP = filePath;
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
        }
    }
}
void MainWindow::on_pushButton_supprimer_clicked()
{
    // Récupérer l'index de la ligne sélectionnée dans le tableau
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    // Récupérer l'ID de l'employé sélectionné (supposons que l'ID soit dans la première colonne)
    int id = selection.first().data().toInt();

    // Demander une confirmation avant de supprimer
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    // Appel de la fonction supprimer avec l'ID de l'employé
    Employe employe;
    if (employe.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
        afficherEmployes();  // Met à jour la table après suppression
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'employé !");
    }
}
