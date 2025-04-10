#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"
#include "dialog.h"
#include <QMessageBox>
#include <QTabBar>  // Ajouté pour éviter l'erreur
#include <QDebug>   // Pour utiliser qDebug()
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QSqlQueryModel>
#include <QtCharts>
#include <QPieSeries>
#include <QPropertyAnimation>
#include <QChartView>
#include <QVBoxLayout>
#include <QFuture>
#include <QSqlTableModel>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("evenements"); // Remplace "evenements" par le nom correct de ta table
    model->select();
    ui->tableView->setModel(model);
    // Initialisation de l'interface
    //statModel = new QSqlQueryModel(this);
    afficherEvenement();

    //connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::afficherEvenement);
    connect(ui->btnDialog, &QPushButton::clicked, this, &MainWindow::on_btnDialog_clicked);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->btn_trierDate, &QPushButton::clicked, this, &MainWindow::on_btn_trierDate_clicked);
    connect(ui->searchText, &QLineEdit::textChanged, this, &MainWindow::rechercherevenement);






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
    connect(ui->chatbot, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Statistiques";
    });
    openAIClient = new OpenAIClient(this);
    connect(openAIClient, &OpenAIClient::reponseRecue, this, [](const QString &reponse) {
        qDebug() << "Réponse du chatbot : " << reponse;
    });
    connect(ui->btnEnvoyer, &QPushButton::clicked, this, [=]() {
        QString question = ui->lineEditQuestion->text();
        openAIClient->envoyerRequete(question);
    });
    connect(openAIClient, &OpenAIClient::reponseRecue, this, [=](const QString &reponse) {
        ui->textBrowserReponse->append("Chatbot : " + reponse);
    });


    // Ajouter le style aux boutons et gérer l'effet hover
    QString buttonStyle =
        "QPushButton { "
        "background-color: rgb(227, 241, 244); "
        "font: 700 9pt 'Segoe UI'; "
        "border: 2px solid black; "
        "border-image-source: linear-gradient(to right, black, white); "
        "border-image-slice: 1; "
        "qproperty-iconSize: 30px 30px; "
        "border-radius: 30px; "
        "} "
        "QPushButton:hover { "
        "background-color: transparent; "
        "}";

    // Appliquer le style à chaque bouton
    ui->liste->setStyleSheet(buttonStyle);
    ui->ajout->setStyleSheet(buttonStyle);
    ui->stat->setStyleSheet(buttonStyle);
    ui->pushButton_choisirImage->setStyleSheet(buttonStyle);

}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ajouter_button_clicked()
{

    // Récupérer les valeurs des champs
    QString titre = ui->titrel->text();
    //QString description = ui->deslwew->text(); // Utiliser text() pour QLineEdit
    QString description = ui->desl->toPlainText(); // Utiliser toPlainText() pour QTextEdit
    QDate dateDebut = ui->ddl->date();
    QDate dateFin = ui->dfl->date();
    int capacite = ui->capl->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl->value();
    QString statut = ui->statusl->currentText();
    QString categorie = ui->catl->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl->text();
    int id_espace = ui->lieul->text().toInt(); // Ajout du champ lieu (assurez-vous que lieul existe dans l'UI)

    // Contrôle des champs obligatoires
    if (titre.isEmpty() || description.isEmpty() || capacite <= 0 || prix <= 0 || statut.isEmpty() ||
        categorie.isEmpty() || type.isEmpty() || organisateur.isEmpty() || id_espace < 0) {
        QMessageBox::critical(this, "Erreur", "Tous les champs doivent être remplis !");
        return; // Arrêter l'exécution de la fonction si un champ est vide
    }

    // Contrôle de la capacité (doit être supérieure à 10)
    if (capacite <= 10) {
        QMessageBox::critical(this, "Erreur", "La capacité doit être supérieure à 10 !");
        return;
    }

    // Contrôle du prix (doit être positif)
    if (prix <= 0) {
        QMessageBox::critical(this, "Erreur", "Le prix doit être positif !");
        return;
    }

    // Contrôle de la date de fin (doit être après la date de début)
    if (dateFin <= dateDebut) {
        QMessageBox::critical(this, "Erreur", "La date de fin doit être supérieure à la date de début !");
        return;
    }

    // Créer l’objet Evenement avec les données validées
    Evenement ev(titre, type, capacite, prix, description, dateDebut, dateFin, categorie, statut, organisateur, id_espace);

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



QList<QList<QString>> MainWindow::getEventData() {
    QList<QList<QString>> eventData;

    // Récupérer toutes les colonnes sauf "AFFICHE"
    QSqlQuery query("SELECT  TITRE, CAPACITE, PRIX, DESCRIPTION, DATE_DEB, DATE_FIN, STATUT, ORGANISATEUR,  ID_ESPACE FROM EVENEMENT");

    while (query.next()) {
        QList<QString> rowData;
        rowData
            << query.value("TITRE").toString()
            << query.value("CAPACITE").toString()
            << query.value("PRIX").toString()
            << query.value("DESCRIPTION").toString()
            << query.value("DATE_DEB").toString()
            << query.value("DATE_FIN").toString()
            << query.value("STATUT").toString()
            << query.value("ORGANISATEUR").toString()
            << query.value("ID_ESPACE").toString();
        eventData.append(rowData);
    }
    // Vérification des données récupérées
    qDebug() << "Données récupérées:";
    for (const auto &row : eventData) {
        qDebug() << row;
    }
    return eventData;
}


void MainWindow::on_pdf_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Liste des événements");

    QPainter painter(&pdfWriter);

    QList<QList<QString>> eventData = getEventData();
    if (eventData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter.");
        return;
    }

    int marginLeft = 100; // ✅ Réduction de la marge
    int marginTop = 120;
    int x = marginLeft;
    int y = marginTop;

    // ✅ Dessiner le titre du PDF avec une police plus petite
    QString title = "Liste des événements";
    QRect rectTitle(marginLeft, y, pdfWriter.width() - (marginLeft * 2), 50);
    painter.setFont(QFont("Helvetica", 14, QFont::Bold)); // ✅ Police réduite
    painter.setPen(Qt::darkBlue);
    painter.drawText(rectTitle, Qt::AlignCenter, title);
    y += 80;

    int rowHeight = 50; // ✅ Hauteur de ligne réduite
    int maxColsPerPage = 7; // ✅ Augmenter légèrement le nombre max de colonnes

    int colCount = eventData[0].size();
    int colsToShow = qMin(maxColsPerPage, colCount);

    // ✅ Calcul automatique de la largeur des colonnes en fonction du nombre total
    int colWidth = (pdfWriter.width() - (2 * marginLeft)) / colsToShow;

    // ✅ Dessiner l'en-tête avec une plus petite police
    painter.setFont(QFont("Helvetica", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));

    QStringList headers = { "Titre", "Capacité", "Prix", "Description", "Date Début", "Date Fin",  "Statut", "Organisateur", "id_espace"};

    for (int i = 0; i < colsToShow; ++i) {
        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(QRect(x + 2, y, colWidth - 4, rowHeight), Qt::AlignCenter, headers[i]);
        x += colWidth;
    }

    y += rowHeight + 5;
    x = marginLeft;

    // ✅ Dessiner les données avec une police plus petite et un alignement optimal
    painter.setFont(QFont("Helvetica", 9));
    painter.setPen(Qt::black);

    for (const auto &row : eventData) {
        QColor rowColor = (y / rowHeight % 2 == 0) ? QColor(245, 245, 245) : QColor(255, 255, 255);
        painter.setBrush(rowColor);

        for (int i = 0; i < colsToShow; ++i) {
            painter.drawRect(x, y, colWidth, rowHeight);
            QString data = row[i];

            // ✅ Réduction du texte proprement avec "...", en fonction de la largeur des colonnes
            if (data.length() > colWidth / 8) {
                data = data.left(colWidth / 8) + "...";
            }

            painter.drawText(QRect(x + 2, y, colWidth - 4, rowHeight), Qt::AlignLeft | Qt::AlignVCenter, data);
            x += colWidth;
        }

        x = marginLeft;
        y += rowHeight;

        // ✅ Gestion du saut de page si nécessaire
        if (y > pdfWriter.height() - marginTop) {
            pdfWriter.newPage();
            y = marginTop;

            // Répéter l'en-tête après le saut de page
            painter.setFont(QFont("Helvetica", 10, QFont::Bold));
            painter.setPen(Qt::black);
            painter.setBrush(QColor(230, 230, 230));

            x = marginLeft;
            for (int i = 0; i < colsToShow; ++i) {
                painter.drawRect(x, y, colWidth, rowHeight);
                painter.drawText(QRect(x + 2, y, colWidth - 4, rowHeight), Qt::AlignCenter, headers[i]);
                x += colWidth;
            }

            y += rowHeight + 5;
            x = marginLeft;
        }
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");
}






void MainWindow::afficherStatistiques() {
    QSqlQuery query;
    query.prepare("SELECT statut, categorie FROM EVENEMENT");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête");
        return;
    }

    QMap<QString, int> statutCount;   // Compteur des statuts
    QMap<QString, int> categorieCount; // Compteur des catégories

    // Récupérer les résultats et stocker les occurrences
    while (query.next()) {
        QString statut = query.value(0).toString();
        QString categorie = query.value(1).toString();

        statutCount[statut]++;
        categorieCount[categorie]++;
    }

    // 📊 Création du graphique circulaire pour les statuts
    QPieSeries *pieSeries = new QPieSeries();
    for (auto it = statutCount.begin(); it != statutCount.end(); ++it) {
        QPieSlice *slice = pieSeries->append(it.key(), it.value());
        slice->setLabel(QString("%1 (%2)").arg(it.key()).arg(it.value()));
        slice->setLabelVisible(true);
        slice->setExploded(true);  // Effet d'explosion pour chaque tranche
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des événements par statut");
    pieChart->legend()->setAlignment(Qt::AlignRight);

    // 🎬 Animation pour le graphique circulaire
    pieChart->setAnimationOptions(QChart::AllAnimations);

    // 📊 Création du graphique à barres pour les catégories
    QBarSet *set = new QBarSet("Nombre d'événements");
    QStringList categoriesList;

    for (auto it = categorieCount.begin(); it != categorieCount.end(); ++it) {
        *set << it.value();
        categoriesList.append(it.key());
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Nombre d'événements par catégorie");

    // 🎯 Configuration des axes du graphique à barres
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categoriesList);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre d'événements");
    axisY->setLabelFormat("%d");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // 🎬 Animation pour le graphique à barres
    barChart->setAnimationOptions(QChart::AllAnimations);

    // 📌 Affichage avec QChartView
    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    pieChartView->setFixedSize(400, 350);
    barChartView->setFixedSize(400, 350);

    // 📌 Ajout des graphiques dans l'onglet statistiques
    QWidget *statistiquesTab = ui->tabWidget->widget(2);

    // Suppression de l'ancien layout s'il existe pour éviter l'empilement des graphiques
    QLayout *oldLayout = statistiquesTab->layout();
    if (oldLayout) {
        delete oldLayout;
    }

    QHBoxLayout *layout = new QHBoxLayout(statistiquesTab);
    layout->addWidget(pieChartView);
    layout->addWidget(barChartView);

    statistiquesTab->setLayout(layout);
}

void MainWindow::on_historique_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter l'historique en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Portrait);
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Historique des événements");

    QPainter painter(&pdfWriter);

    QList<QList<QString>> eventData = getEventData(); // Récupérer les données des événements
    QList<QList<QString>> filteredData;

    for (const auto &row : eventData) {
        if (row.size() >= 7 && row[6] == "terminé") { // Vérifier que l'événement est terminé
            filteredData.append({row[0], row[6]}); // Ajouter seulement le titre et le statut
        }
    }

    if (filteredData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun événement terminé à exporter.");
        return;
    }

    int marginLeft = 100;
    int marginTop = 120;
    int x = marginLeft;
    int y = marginTop;

    // Dessiner le titre du PDF
    QString title = "Historique des événements";
    QRect rectTitle(marginLeft, y, pdfWriter.width() - (marginLeft * 2), 50);
    painter.setFont(QFont("Helvetica", 14, QFont::Bold));
    painter.setPen(Qt::darkBlue);
    painter.drawText(rectTitle, Qt::AlignCenter, title);
    y += 80;

    int rowHeight = 50;
    int colWidth = (pdfWriter.width() - (2 * marginLeft)) / 2;

    // Dessiner l'en-tête
    painter.setFont(QFont("Helvetica", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));

    QStringList headers = {"Titre", "Statut"};
    for (int i = 0; i < 2; ++i) {
        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(QRect(x + 2, y, colWidth - 4, rowHeight), Qt::AlignCenter, headers[i]);
        x += colWidth;
    }

    y += rowHeight + 5;
    x = marginLeft;

    // Dessiner les données
    painter.setFont(QFont("Helvetica", 9));
    painter.setPen(Qt::black);

    for (const auto &row : filteredData) {
        painter.setBrush(Qt::white);
        for (int i = 0; i < 2; ++i) {
            painter.drawRect(x, y, colWidth, rowHeight);
            painter.drawText(QRect(x + 2, y, colWidth - 4, rowHeight), Qt::AlignLeft | Qt::AlignVCenter, row[i]);
            x += colWidth;
        }
        x = marginLeft;
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");
}


void MainWindow::on_btn_trierDate_clicked() {
    // Récupérer l'index de la colonne sélectionnée
    int columnIndex = ui->tableView->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return; // Arrêter la fonction si aucune colonne n'est sélectionnée
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->tableView->model());
    if (!proxyModel) {
        // Si aucun proxyModel n'existe, on en crée un
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView->setModel(proxyModel); // Appliquer le proxyModel à tableView
    }

    // Appliquer le tri sur la colonne sélectionnée (par exemple, tri croissant)
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    qDebug() << "Tri effectué sur la colonne : " << columnIndex;

    // Afficher un message de confirmation
    QMessageBox::information(this, "Tri effectué", "Les événements ont été triés !");
}


void MainWindow::rechercherevenement() {
    QString searchText = ui->searchText->text().trimmed(); // Récupérer le texte de la QLineEdit

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());  // Assure-toi que serviceModel est le modèle principal
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterRole(Qt::DisplayRole);
        ui->tableView->setModel(proxyModel);
    }

    // Obtenir les index des colonnes visibles ID et Titre
    int colId = ui->tableView->horizontalHeader()->logicalIndex(0);
    int colTitre = ui->tableView->horizontalHeader()->logicalIndex(1);

    // Appliquer le filtre sur plusieurs colonnes
    proxyModel->setFilterKeyColumn(-1);  // Active la recherche sur toutes les colonnes

    // Utiliser une expression régulière pour filtrer l'ID et le Titre
    QRegularExpression regex(searchText, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);
}
