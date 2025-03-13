#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include "maintenance.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTableView>
#include <QPdfWriter>
#include <QPainter>
#include <QHeaderView>
#include <QAbstractItemModel>


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
    ui->tabWidget_3->tabBar()->hide();

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



    connect(ui->liste_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->ajout_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Affichage";
    });

    connect(ui->stat_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Statistiques";
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
void MainWindow::on_pushButton_ajouter_clicked_maintenace() {
    qDebug() << "Tentative d'ajout d'une maintenance...";

    // Réinitialisation des labels d'erreur
    ui->er_description->clear();
    ui->er_cout->clear();
    ui->er_id_employee->clear();
    ui->er_id_espace->clear();

    QString type = ui->type->currentText();
    QString description = ui->description->text();
    QString statut = ui->statut_maintenance->currentText();
    QDate date_resolution = ui->date_resolution->date();
    QString id_employe = ui->id_employe_maintenance->text();
    QString id_espace = ui->id_espace_maintenance->text();
    QString coutStr = ui->cout->text();

    bool valid = true;

    // Vérification de la description (non vide)
    if (description.isEmpty()) {
        ui->er_description->setText("La description ne peut pas être vide.");
        valid = false;
    }

    // Vérification du coût (doit être un double)
    bool ok;
    double cout = coutStr.toDouble(&ok);
    if (!ok) {
        ui->er_cout->setText("Le coût doit être un nombre valide.");
        valid = false;
    }

    // Vérification de l'existence de l'ID employé dans la base
    int id_employe1 = id_employe.toInt(&ok);
    if (!ok || !Maintenance::employeeExiste(id_employe1)) {
        ui->er_id_employee->setText("L'ID employé n'existe pas.");
        valid = false;
    }

    // Vérification de l'existence de l'ID espace dans la base
    int id_espace1 = id_espace.toInt(&ok);
    if (!ok || !Maintenance::espaceExiste(id_espace1)) {
        ui->er_id_espace->setText("L'ID espace n'existe pas.");
        valid = false;
    }

    if (!valid) {
        return;
    }

    /*qDebug() << "Type:" << type << ", Description:" << description << ", Coût:" << cout
             << ", Statut:" << statut << ", Date résolution:" << date_resolution.toString("yyyy-MM-dd")
             << ", ID Employé:" << id_employe1 << ", ID Espace:" << id_espace1;*/

    Maintenance maintenance(type, description, cout, statut, date_resolution.toString("yyyy-MM-dd"), id_employe1, id_espace1);

    if (maintenance.ajouter()) {
        QMessageBox::information(this, "Succès", "Maintenance ajoutée avec succès !");
        ui->tabWidget_3->setCurrentIndex(1);
        afficher_maintenance();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la maintenance !");
    }
}
void MainWindow::afficher_maintenance()
{
    Maintenance m;
    QSqlQueryModel* model = m.afficher();

    if (model) {
        ui->tab_maintenance->setModel(model);
        ui->tab_maintenance->setStyleSheet(
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
        ui->tab_maintenance->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des maintenances.");
    }
}





void MainWindow::on_tabWidget_3_tabBarClicked(int index)
{
    afficher_maintenance();
}

void MainWindow::on_supprimer_maintenance_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner une maintenance à supprimer.");
        return;
    }

    int id = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cette maintenance ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Maintenance maintenance;
        if (maintenance.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Maintenance supprimée avec succès.");
            afficher_maintenance(); // Met à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}

void MainWindow::on_modifier_maintenance_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Modification", "Veuillez sélectionner une maintenance à modifier.");
        return;
    }
    int id = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toInt();
    Maintenance maintenance;
    maintenance.afficherDetails(id, ui);
}

void MainWindow::on_update_maintenance_clicked() {
    int id = ui->up_id->text().toInt();
    QString type = ui->up_type->currentText();
    QString description = ui->up_description->text();
    QString statut = ui->up_statut_maintenance->currentText();
    QDate date_resolution = ui->up_date_resolution->date();
    QString id_employe = ui->up_id_employe_maintenance->text();
    QString id_espace = ui->up_id_espace_maintenance->text();
    QString coutStr = ui->up_cout->text();

    bool valid = true;
    if (description.isEmpty()) {
        ui->er_description_up->setText("La description ne peut pas être vide.");
        valid = false;
    }
    bool ok;
    double cout = coutStr.toDouble(&ok);
    if (!ok) {
        ui->er_cout_up->setText("Le coût doit être un nombre valide.");
        valid = false;
    }
    int id_employe1 = id_employe.toInt(&ok);
    if (!ok || !Maintenance::employeeExiste(id_employe1)) {
        ui->er_id_employee_up->setText("L'ID employé n'existe pas.");
        valid = false;
    }
    int id_espace1 = id_espace.toInt(&ok);
    if (!ok || !Maintenance::espaceExiste(id_espace1)) {
        ui->er_id_espace_up->setText("L'ID espace n'existe pas.");
        valid = false;
    }
    if (!valid) return;
    Maintenance maintenance;
    if (maintenance.update(id, type, description, cout, statut, date_resolution.toString("yyyy-MM-dd"), id_employe1, id_espace1)) {
        QMessageBox::information(this, "Succès", "Maintenance mise à jour avec succès.");
        ui->tabWidget_3->setCurrentIndex(1);
        afficher_maintenance();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour.");
    }
}



void MainWindow::on_liste_maintenance_clicked()
{
    afficher_maintenance();
}


void MainWindow::on_ajouter_maintenance_2_clicked()
{
    afficher_maintenance();
}


void MainWindow::on_exporter_maintenance_clicked()
{
    // Demander à l'utilisateur où enregistrer le fichier
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exporter en PDF"), "", tr("Fichiers PDF (*.pdf)"));
    if (fileName.isEmpty()) {
        return;
    }

    // Création du fichier PDF
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));  // Format A4
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier PDF.");
        return;
    }

    // Définition des marges manuelles
    int marginLeft = 30;
    int marginTop = 50;
    int tableStartY = marginTop + 100; // Position de départ du tableau après le titre

    // Définition des polices
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);

    // Dessiner le titre centré
    painter.setFont(titleFont);
    painter.drawText(QRect(marginLeft, marginTop, writer.width() - 2 * marginLeft, 50), Qt::AlignCenter, "Liste des Maintenances");

    // Ajouter la date d'exportation
    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.setFont(contentFont);
    painter.drawText(writer.width() - 200, marginTop + 20, "Date: " + date);

    // Récupération des données
    QTableView* table = ui->tab_maintenance;
    QAbstractItemModel* model = table->model();

    if (!model) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }

    // Positionnement et dimensions des colonnes
    int startX = marginLeft;
    int startY = tableStartY;
    int rowHeight = 30;
    int colWidth = 120;

    // Dessiner l'en-tête du tableau
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::lightGray);

    int x = startX;
    int y = startY;

    for (int column = 0; column < model->columnCount(); ++column) {
        QString header = model->headerData(column, Qt::Horizontal).toString();
        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(x + 10, y + 20, header);
        x += colWidth;
    }

    y += rowHeight;

    // Dessiner les données du tableau
    painter.setFont(contentFont);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < model->rowCount(); ++row) {
        x = startX;
        for (int column = 0; column < model->columnCount(); ++column) {
            QString data = model->data(model->index(row, column)).toString();
            painter.drawRect(x, y, colWidth, rowHeight);
            painter.drawText(x + 10, y + 20, data);
            x += colWidth;
        }
        y += rowHeight;

        // Passer à une nouvelle page si nécessaire
        if (y > writer.height() - 100) {
            writer.newPage();
            y = tableStartY; // Recommencer en haut de la nouvelle page
        }
    }

    // Ajouter un pied de page avec pagination
    painter.setFont(contentFont);
    painter.drawText(writer.width() / 2 - 40, writer.height() - 40, "Page 1");

    // Fin du dessin
    painter.end();

    QMessageBox::information(this, tr("Succès"), tr("Exportation PDF réussie !"));
}
