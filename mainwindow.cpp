#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include "maintenance.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cheminImagePDP("")
{
    ui->setupUi(this);  // Initialisation de l'interface

    // Afficher les données de maintenance au démarrage
    Maintenance m;
    ui->tab_maintenance->setModel(m.afficher());

    // Connexion des boutons aux slots
    connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);

    //button ajouter maintenance
    connect(ui->ajouter_maintenance, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked_maintenace);


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

    // Connect buttons to switch pages
    connect(ui->button_employe, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->button_maintenance, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->pushButton_locataire, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
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

    bool ok;
    int salaire = ui->salaire->text().toInt(&ok);
    if (!ok) {
        qDebug() << "Erreur : Le salaire n'est pas un nombre valide ->" << ui->salaire->text();
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide.");
        return;
    }

    qDebug() << "Nom:" << nom << ", Prénom:" << prenom << ", Date embauche:" << date_embauche
             << ", Poste:" << poste << ", Salaire:" << salaire << ", Email:" << email
             << ", Sexe:" << sexe << ", Téléphone:" << telephone << ", Statut:" << statut
             << ", Photo de profil:" << cheminImagePDP;

    // Création et ajout de l'employé
    Employe employe(nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, cheminImagePDP, statut);

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
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

            // Sauvegarde du chemin de l'image
            cheminImagePDP = filePath;
            qDebug() << "Image sélectionnée :" << cheminImagePDP;
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
        }
    }
}
void MainWindow::afficherEmployes()
{
    Employe employe;
    QSqlTableModel* model = employe.getModel();

    if (!model) {
        qDebug() << "Erreur : modèle QSqlTableModel non initialisé !";
        return;
    }

    qDebug() << "Nombre de lignes trouvées :" << model->rowCount();

    if (model->rowCount() == 0) {
        qDebug() << "Aucun employé trouvé dans la base.";
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();  // Ajuster la taille des colonnes
}

//maintenace
void MainWindow::on_pushButton_ajouter_clicked_maintenace(){
    qDebug() << "Tentative d'ajout d'une maintenance...";

    QString type = ui->type->currentText();  // Type depuis la ComboBox
    QString description = ui->description->text();  // Texte multi-lignes
    QString statut = ui->statut_maintenance->currentText();  // Statut
    QDate date_resolution = ui->date_resolution->date();  // Date
    QString id_employe = ui->id_employe_maintenance->text();  // ID Employé
    QString id_espace = ui->id_espace_maintenance->text();  // ID Espace

    // Vérification du coût (FLOAT dans la base)
    //bool ok;
    int cout = 123;
    /*if (!ok) {
        qDebug() << "Erreur : Le coût n'est pas un nombre valide ->" << ui->cout->text();
        QMessageBox::critical(this, "Erreur", "Le coût doit être un nombre valide.");
        return;
    }*/

    // Vérification des ID (optionnel : vérifier si l'employé et l'espace existent)
    /*if (id_employe <= 0 || id_espace <= 0) {
        QMessageBox::critical(this, "Erreur", "L'ID Employé et l'ID Espace doivent être valides.");
        return;
    }*/

    qDebug() << "Type:" << type << ", Description:" << description << ", Coût:" << cout
             << ", Statut:" << statut << ", Date résolution:" << date_resolution.toString("yyyy-MM-dd")
             << ", ID Employé:" << id_employe << ", ID Espace:" << id_espace;

    int id_employe1=id_employe.toInt();
    int id_espace1=id_espace.toInt();

    // Création de l'objet Maintenance et ajout dans la base
    Maintenance maintenance(type, description, cout, statut, date_resolution.toString("yyyy-MM-dd"), id_employe1, id_espace1);

    if (maintenance.ajouter()) {
        QMessageBox::information(this, "Succès", "Maintenance ajoutée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la maintenance !");
    }
}
void MainWindow::afficher_maintenance()
{
    Maintenance m;
    QSqlQueryModel* model = m.afficher();
    ui->tab_maintenance->setModel(model);
}




void MainWindow::on_tabWidget_3_tabBarClicked(int index)
{
    afficher_maintenance();
}

