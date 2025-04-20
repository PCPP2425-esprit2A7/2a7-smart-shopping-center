#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "locataire.h"
#include <QMessageBox>
#include <QDebug>
#include "dialog.h"
#include <QSortFilterProxyModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlError>
#include <QPixmap>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QDate>
#include <QSslSocket>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QDesktopServices>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     afficherLocataires();
    // ✅ Appliquer une police moderne globalement
    QFont defaultFont("Segoe UI", 10);  // ou "Arial", "Calibri"...
    QApplication::setFont(defaultFont);

    // Connexions des boutons
   connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherLocataires);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::trierlocataire);
    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::exporterPDF);
    connect(ui->chmprech, &QLineEdit::textChanged, this, &MainWindow::rechercherLocataire);
    connect(ui->upload, &QPushButton::clicked, this, &MainWindow::on_upload_clicked);
    connect(ui->btnAfficherStatistiques, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);

    // Connexion avec lambda pour extraction OpenAI
    connect(ui->pushButton_extraire, &QPushButton::clicked, this, [this]() {
        if (cheminContrat.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun contrat sélectionné !");
            return;
        }
        envoyerImageAOpenAI(cheminContrat);
    });

    // Onglet masqué et navigation entre les onglets
    ui->tabWidget->tabBar()->hide();

    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Liste";
        afficherLocataires();
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Ajout";
    });

    connect(ui->stat, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
        afficherStatistiques();
    });

    // Optionnel : afficher les locataires directement au démarrage
    afficherLocataires();
    ui->chmprech->setPlaceholderText("🔎 Rechercher un locataire...");

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

    // Chargement du fichier contrat en QByteArray
    QByteArray contratData;
    if (!cheminContrat.isEmpty()) {
        QFile file(cheminContrat);
        if (file.open(QIODevice::ReadOnly)) {
            contratData = file.readAll();
            file.close();
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de lire le fichier du contrat.");
            return;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un contrat.");
        return;
    }

    // Création et ajout du locataire
    Locataire locataire(nom, prenom, typeact, email, telephone, duree, datedeb, datefin, paiement, contratData);

    if (locataire.ajouter()) {
        QMessageBox::information(this, "Succès", "Locataire ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du locataire !");
    }
}

void MainWindow::afficherLocataires()
{
    Locataire locataire;
    QSqlQueryModel* model = locataire.afficher();

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
            "   color: black;"  // ✅ texte noir
            "   padding: 10px;"
            "   border-bottom: 1px solid #ddd;"
            "}"
            "QTableView::item:selected {"
            "   background-color: #6fa3ef;"
            "   color: white;"  // texte blanc sur sélection
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

void MainWindow::trierlocataire() {
    // Récupérer l'index de la colonne sélectionnée
    int columnIndex = ui->tableView->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return; // Arrêter la fonction si aucune colonne n'est sélectionnée
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());
    if (!proxyModel) {
        // Si aucun proxyModel n'existe, on en crée un
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView->setModel(proxyModel); // Appliquer le proxyModel à tableView
    }

    // Appliquer le tri sur la colonne sélectionnée
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    qDebug() << "Tri effectué sur la colonne : " << columnIndex;
}
void MainWindow::exporterPDF() {

        QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
        if (filePath.isEmpty()) return;

        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSize(QPageSize::A4);
        pdfWriter.setResolution(300);

        QPainter painter(&pdfWriter);
        int left = 50;
        int top = 60;
        int right = 50;
        int x = left;
        int y = top;

        // Titre
        QString title = "📜 Liste des locataires";
        painter.setFont(QFont("Helvetica", 16, QFont::Bold));
        painter.setPen(QColor(30, 30, 100));
        QRect titleRect(left, y, pdfWriter.width() - left - right, 60);
        painter.drawText(titleRect, Qt::AlignCenter, title);
        y += 80;

        QFont headerFont("Helvetica", 11, QFont::Bold);
        QFont bodyFont("Helvetica", 10);
        painter.setFont(headerFont);

        // Préparation des colonnes visibles (sans image)
        QAbstractItemModel *model = ui->tableView->model();
        if (!model) return;

        int colCount = model->columnCount();
        int rowCount = model->rowCount();

        int imageCol = -1;
        int descriptionCol = -1;

        for (int i = 0; i < colCount; ++i) {
            QString header = model->headerData(i, Qt::Horizontal).toString().toLower();
            if (header.contains("pdp")) imageCol = i;
            if (header.contains("description")) descriptionCol = i;
        }

        QList<int> visibleCols;
        for (int i = 0; i < colCount; ++i) {
            if (i != imageCol) visibleCols.append(i);
        }

        int cols = visibleCols.size();
        int pageWidth = pdfWriter.width() - left - right;
        int descriptionExtra = 2; // pondération x2 pour la colonne description
        int baseUnit = pageWidth / (cols + (descriptionCol != -1 ? descriptionExtra - 1 : 0));
        QMap<int, int> colWidths;

        for (int i = 0; i < cols; ++i) {
            int col = visibleCols[i];
            colWidths[col] = (col == descriptionCol) ? baseUnit * descriptionExtra : baseUnit;
        }

        int rowHeight = 40;
        x = left;

        // Dessiner les entêtes
        for (int col : visibleCols) {
            int width = colWidths[col];
            QRect rect(x, y, width, rowHeight);
            painter.fillRect(rect, QColor(220, 220, 250));
            painter.setPen(Qt::black);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
            x += width;
        }

        y += rowHeight;
        painter.setFont(bodyFont);

        // Dessiner les données
        for (int row = 0; row < rowCount; ++row) {
            x = left;
            QColor bgColor = (row % 2 == 0) ? QColor(255, 255, 255) : QColor(245, 245, 255);

            for (int col : visibleCols) {
                int width = colWidths[col];
                QRect rect(x, y, width, rowHeight);
                painter.fillRect(rect, bgColor);
                painter.setPen(Qt::gray);
                painter.drawRect(rect);

                QString data = model->data(model->index(row, col)).toString();
                QTextOption opt(Qt::AlignLeft | Qt::AlignVCenter);
                opt.setWrapMode(QTextOption::WordWrap);

                painter.setPen(Qt::black);
                painter.drawText(rect.adjusted(5, 0, -5, 0), data, opt);
                x += width;
            }

            y += rowHeight;

            // Saut de page
            if (y > pdfWriter.height() - 100) {
                pdfWriter.newPage();
                y = top;
            }
        }

        painter.end();
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        QMessageBox::information(this, "Succès", "Le PDF stylisé a été généré avec succès !");
    }




    void MainWindow::rechercherLocataire(const QString &searchText) {
        // Vérifier si un proxyModel existe déjà
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

        if (!proxyModel) {
            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(ui->tableView->model());  // Assurez-vous que le modèle de locataires est le modèle source
            proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
            ui->tableView->setModel(proxyModel);
        }

        // Filtrer par nom ou ID (Supposons que Nom = colonne 1 et ID = colonne 0)
        proxyModel->setFilterKeyColumn(-1);  // Permet de filtrer sur plusieurs colonnes
        proxyModel->setFilterRegularExpression(QRegularExpression(searchText, QRegularExpression::CaseInsensitiveOption));
    }



void MainWindow::on_upload_clicked()
{
    // Ouvre le dialogue de fichier pour sélectionner une image
    QString chemin = QFileDialog::getOpenFileName(
        this,
        "Sélectionner un contrat (image)",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if (!chemin.isEmpty()) {
        cheminContrat = chemin; // sauvegarde si besoin plus tard

        // Option 1 : Afficher le chemin dans un QLineEdit
        ui->contrat->setText(chemin);

        // Option 2 : Afficher l'image dans un QLabel
        QPixmap image(chemin);
        ui->contrat->setPixmap(image.scaled(ui->contrat->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
void MainWindow::envoyerImageAOpenAI(const QString &cheminContrat) {
    qDebug() << "SSL supported:" << QSslSocket::supportsSsl();
    qDebug() << "SSL library version:" << QSslSocket::sslLibraryVersionString();
    // Lire et encoder l'image en base64
    QFile file(cheminContrat);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur lors de l'ouverture du fichier image";
        return;
    }

    QByteArray imageData = file.readAll().toBase64();
    file.close();

    // Construire le message pour GPT-4 Vision
    QJsonObject imageObject{
        {"type", "image_url"},
        {"image_url", QJsonObject{
                          {"url", "data:image/jpeg;base64," + QString::fromUtf8(imageData)}
                      }}
    };

    QJsonObject textObject{
        {"type", "text"},
        {"text", "Ce document est un contrat de location. Peux-tu extraire les informations utiles pour un système de gestion locative ? Les données recherchées sont : nom complet, adresse email, date de naissance et adresse. Ce traitement est automatisé dans une application."}
    };

    QJsonObject message{
        {"role", "user"},
        {"content", QJsonArray{ textObject, imageObject }}
    };

    QJsonObject body{
        {"model", "gpt-4o"},
        {"messages", QJsonArray{ message }},
        {"max_tokens", 1000}
    };

    QJsonDocument doc(body);

    // cette partie est la partie du clé api
    
    // Envoyer la requête
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::traiterReponseOpenAI);
    manager->post(request, doc.toJson());
}

void MainWindow::traiterReponseOpenAI(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur réseau:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isObject()) {
        qDebug() << "Réponse invalide de l'API";
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QString contenu = jsonObj["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();

    qDebug() << "Contenu extrait par l'IA:" << contenu;

    QRegularExpression regexNomComplet("Nom complet\\s*:\\s*([A-Za-zÀ-ÿ\\-\\s]+)");
    QRegularExpression regexEmail("Adresse email\\s*:\\s*([\\w.-]+@[\\w.-]+)");
    QRegularExpression regexDate("Date de naissance\\s*:\\s*(\\d{2}/\\d{2}/\\d{4})");
    QRegularExpression regexAdresse("Adresse\\s*:\\s*(.+)$");

    // 🔄 Exemple de parsing (à adapter selon le format que renvoie OpenAI)
    QRegularExpressionMatch match;

    match = regexNomComplet.match(contenu);
    if (match.hasMatch()) {
        QString nomComplet = match.captured(1).trimmed();
        QStringList noms = nomComplet.split(" ");
        if (noms.size() >= 2) {
            ui->prenom->setText(noms.first());
            ui->nom->setText(noms.last());
        }
    }

    match = regexEmail.match(contenu);
    if (match.hasMatch()) ui->email->setText(match.captured(1));

}

void MainWindow::afficherStatistiques() {
    QSqlQuery query;

    // 1. Taux de renouvellement des contrats (doublons)
    query.prepare("SELECT NOM, PRENOM, EMAIL, COUNT(*) FROM LOCATAIRE GROUP BY NOM, PRENOM, EMAIL HAVING COUNT(*) > 1");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête pour le taux de renouvellement des contrats.");
        return;
    }

    int totalRenouvellements = 0;
    while (query.next()) {
        totalRenouvellements++;
    }

    // 2. Répartition des locataires par type d'activité
    query.prepare("SELECT TYPE_ACTIVITE, COUNT(*) FROM LOCATAIRE GROUP BY TYPE_ACTIVITE");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête pour la répartition des locataires.");
        return;
    }

    std::vector<int> counts;
    std::vector<QString> types;
    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        types.push_back(type);
        counts.push_back(count);
    }

    // 3. Montant total des loyers payés (avec conversion des paiements en nombres)
    query.prepare("SELECT SUM(TO_NUMBER(PAIMENT_EFFECTUE)) FROM LOCATAIRE WHERE PAIMENT_EFFECTUE IS NOT NULL AND PAIMENT_EFFECTUE != ''");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête pour le montant total des loyers payés.");
        return;
    }

    double totalLoyers = 0;
    if (query.next()) {
        totalLoyers = query.value(0).toDouble();
    }

    // Si aucune donnée n'est trouvée
    if (types.empty()) {
        QMessageBox::warning(this, "Aucune donnée", "Aucune donnée trouvée pour les statistiques.");
        return;
    }

    // ** Création du graphique circulaire pour la répartition des locataires par type d'activité **
    QPieSeries *pieSeries = new QPieSeries();
    for (int i = 0; i < types.size(); ++i) {
        QPieSlice *slice = pieSeries->append(types[i], counts[i]);
        slice->setLabel(QString("%1\n%2").arg(types[i]).arg(counts[i]));
        slice->setLabelVisible(true);
        slice->setExploded(true); // Effet d'explosion pour une meilleure visibilité
        slice->setPen(QPen(Qt::black));
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des Locataires par Type d'Activité");
    pieChart->legend()->setAlignment(Qt::AlignRight);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setFixedSize(600, 500);

    // ** Création du graphique à barres pour le nombre de locataires par type d'activité **
    QBarSet *set = new QBarSet("Locataires");
    for (int count : counts) {
        *set << count;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Nombre de Locataires par Type d'Activité");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de Locataires");
    axisY->setLabelFormat("%d");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    // Convertir std::vector en QStringList
    QStringList typeList;
    for (const QString &type : types) {
        typeList << type;
    }

    axisX->append(typeList);
    axisX->setTitleText("Type d'Activité");
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setFixedSize(600, 500);

    // ** Mise à jour de l'affichage dans l'onglet des statistiques **
    QWidget *statistiquesTab = ui->tabWidget->widget(2);

    // Supprimer l'ancien layout s'il existe
    if (statistiquesTab->layout()) {
        QLayoutItem *item;
        while ((item = statistiquesTab->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete statistiquesTab->layout();
    }

    // Ajouter les graphiques dans un layout horizontal
    QHBoxLayout *layout = new QHBoxLayout(statistiquesTab);
    layout->addWidget(pieChartView);
    layout->addWidget(barChartView);
    statistiquesTab->setLayout(layout);

    // Afficher le taux de renouvellement et le total des loyers
    QMessageBox::information(this, "Statistiques",
                             QString("Taux de renouvellement des contrats : %1\nTotal des loyers payés : %.2f €")
                                 .arg(totalRenouvellements).arg(totalLoyers));
}
