#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"
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
      // Utiliser le bon nom du widget 'typl'
    QString organisateur = ui->orgl->text();

    // Créer l’objet
    Evenement ev( titre, type, capacite, prix, "", description, dateDebut, dateFin, categorie, statut, organisateur, 1);

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


/*void MainWindow::on_modifier_clicked()
{
    // Récupérer l'index sélectionné dans la table
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    // Récupérer l'ID de l'événement (assume que la première colonne contient l'ID)
    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    // Créer une requête SQL pour obtenir les données de l'événement sélectionné
    QSqlQuery query;
    query.prepare("SELECT * FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    // Exécuter la requête et charger les données dans les champs
    if (query.exec() && query.next()) {
        // Récupérer les valeurs depuis la base de données
        QString titre = query.value("TITRE").toString();
        QString description = query.value("DESCRIPTION").toString();
        QString type = query.value("TYPE").toString();
        int capacite = query.value("CAPACITE").toInt();
        double prix = query.value("PRIX").toDouble();
        QString categorie = query.value("CATEGORIE").toString();
        QString statut = query.value("STATUT").toString();
        QString organisateur = query.value("ORGANISATEUR").toString();
        QDate dateDebut = query.value("DATE_DEB").toDate();
        QDate dateFin = query.value("DATE_FIN").toDate();

        // Remplir les champs de la page de modification
        ui->titrel_2->setText(titre);
        ui->desl_2->setText(description); // Si c'est un QTextEdit, sinon ->setText() pour un QLineEdit
        ui->typl_2->setText(type);
        ui->capl_2->setText(QString::number(capacite)); // Si c'est QLineEdit
        ui->prixl_2->setValue(prix); // Si c'est QLineEdit
        ui->catl_2->setText(categorie);
        ui->statusl_2->setCurrentText(statut);
        ui->orgl_2->setText(organisateur);
        ui->ddl_2->setDate(dateDebut);
        ui->dfl_2->setDate(dateFin);

        // Afficher la page de modification
        ui->tabWidget->setCurrentWidget(ui->tabWidget);

    } else {
        // Afficher un message d'erreur si la requête échoue
        QMessageBox::critical(this, "Erreur", "Impossible de charger les détails de l'événement.");
        return;
    }
}

{
    int id = ui->tableView->selectionModel()->currentIndex().row();
    if (id == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    QModelIndex index = ui->tableView->model()->index(id, 0); // Supposons que la colonne 0 c'est l'ID
    int idEvenement = index.data().toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", idEvenement);

    if (query.exec() && query.next()) {
        // Récupérer les valeurs
        QString titre = query.value("TITRE").toString();
        QString description = query.value("DESCRIPTION").toString();
        QString type = query.value("TYPE").toString();
        int capacite = query.value("CAPACITE").toInt();
        double prix = query.value("PRIX").toDouble();
        QString categorie = query.value("CATEGORIE").toString();
        QString statut = query.value("STATUT").toString();
        QString organisateur = query.value("ORGANISATEUR").toString();
        QDate dateDebut = query.value("DATE_DEB").toDate();
        QDate dateFin = query.value("DATE_FIN").toDate();

        // Remplir les champs de la page modification
        ui->titrel_2->setText(titre);
        ui->desl_2->setText(description);                // Si c'est un QTextEdit, sinon ->setText() pour un QLineEdit
        ui->typl_2->setText(type);
        ui->capl_2->setText(QString::number(capacite)); // Si c'est QLineEdit
        ui->prixl_2->setValue(prix);                 // Si c'est QLineEdit
        ui->catl_2->setText(categorie);
        ui->statusl_2->setCurrentText(statut);
        ui->orgl_2->setText(organisateur);
        ui->ddl_2->setDate(dateDebut);
        ui->dfl_2->setDate(dateFin);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les détails de l'événement.");
    }
}
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    // Charger les données dans la nouvelle page (formulaire de modification)
    QSqlQuery query;
    query.prepare("SELECT * FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        ui->titrel_2->setText(query.value("TITRE").toString());
        ui->desl_2->setText(query.value("DESCRIPTION").toString());
        ui->ddl_2->setDate(query.value("DATE_DEB").toDate());
        ui->dfl_2->setDate(query.value("DATE_FIN").toDate());
        ui->capl_2->setText(QString::number(valeurCapacite));
        ui->statusl_2->setCurrentText(query.value("STATUT").toString());
        ui->catl_2->setCurrentText(query.value("CATEGORIE").toString());
        ui->typl_2->setCurrentText(query.value("TYPE").toString());
        ui->prixl_2->setValue(query.value("PRIX").toDouble());
        ui->orgl_2->setText(query.value("ORGANISATEUR").toString());
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données.");
        return;
    }

    // Afficher la page de modification
    ui->stackedWidget->setCurrentWidget(ui->pageModifier);
}



void MainWindow::on_modifier_2_clicked()
{
    int id = ui->id->text().toInt();  // récupère l'id de l'événement à modifier

    Evenement E;
    E.setTitre(ui->titrel_2->text());
    QString description = ui->desl->text();
      // Texte long => QTextEdit
    E.setDateDebut(ui->ddl_2->date());
    E.setDateFin(ui->dfl_2->date());
    E.setCapacite(ui->capl_2->text().toInt());
    E.setStatut(ui->statusl_2->currentText());
    E.setType(ui->typl_2->text());
    E.setPrix(ui->prixl_2->value());
    E.setCategorie(ui->catl_2->text());
    E.setOrganisateur(ui->orgl_2->text());

    if (E.modifier(id)) {  // bien passer l'ID ici !
        QMessageBox::information(this, "Succès", "Événement modifié avec succès.");
        afficherEvenement();  // refresh
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}*/




