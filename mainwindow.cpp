#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "service.h"
#include <QMessageBox>
#include <QDebug>
#include "modifierservicedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts>
#include <QPieSeries>
#include <QPropertyAnimation>
#include <QChartView>
#include <QVBoxLayout>

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
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::afficherServices);




    // Connexion des boutons pour changer d'onglet
    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->form, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
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




void MainWindow::on_exel_clicked()
{


}

void MainWindow::afficherStatistiques() {
    QSqlQuery query;
    query.prepare("SELECT nom, cout FROM SERVICE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête");
        return;
    }

    std::vector<double> couts;
    std::vector<QString> noms;

    // Récupérer les résultats de la requête et les ajouter aux vecteurs
    while (query.next()) {
        QString nom = query.value(0).toString();  // Nom du service
        double cout = query.value(1).toDouble();  // Le coût du service
        noms.push_back(nom);
        couts.push_back(cout);
    }

    // Créer un graphique circulaire (Pie Chart)
    QPieSeries *pieSeries = new QPieSeries();
    for (int i = 0; i < couts.size(); ++i) {
        pieSeries->append(noms[i], couts[i]);  // Ajouter le service et son coût
    }
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Statistiques des Coûts - Circulaire");

    // Créer un graphique à barres verticales (Bar Chart)
    QBarSet *set = new QBarSet("Coût");
    for (double cout : couts) {
        *set << cout;  // Ajouter les coûts au bar set
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Statistiques des Coûts - Barres");

    // Configuration des axes pour le graphique à barres verticales
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Coût");
    axisX->setLabelFormat("%d");
    barChart->addAxis(axisX, Qt::AlignBottom);  // L'axe des X (coût)

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    QStringList nomsList;
    for (const QString& nom : noms) {
        nomsList.append(nom);
    }
    axisY->append(nomsList);  // Ajouter les noms des services à l'axe Y
    axisY->setTitleText("Services");
    barChart->addAxis(axisY, Qt::AlignLeft);  // L'axe des Y (services)

    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);

    // Créer un QChartView pour afficher les graphiques
    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    // Fixer une taille pour les graphiques (par exemple, 400x300)
    pieChartView->setFixedSize(600, 550);  // Taille du graphique circulaire
    barChartView->setFixedSize(600, 550);  // Taille du graphique à barres

    // Ajouter les graphiques côte à côte dans un layout horizontal
    QWidget *statistiquesTab = ui->tabWidget->widget(2);  // Accéder à l'onglet des statistiques
    QHBoxLayout *layout = new QHBoxLayout(statistiquesTab);  // Utiliser QHBoxLayout pour l'affichage horizontal

    // Ajouter les graphiques (circulaire et à barres) au layout
    layout->addWidget(pieChartView);
    layout->addWidget(barChartView);

    // Rafraîchir l'onglet des statistiques
    statistiquesTab->setLayout(layout);
}
