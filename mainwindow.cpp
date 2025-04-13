#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "espace.h"
#include "modifierespacesDialog.h"
#include "dimensionsdialog.h"
#include "espace3dview.h"
#include "clickablerectitem.h"
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QPushButton>
#include <QCompleter>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QLayoutItem>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
//const QString OPENAI_API_KEY = "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";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);

    // Initialize the QCompleter
    completer = new QCompleter(this);
    completer = new QCompleter(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_Nom->setCompleter(completer);
    // Connect UI actions (buttons)
    ui->tabWidget->tabBar()->hide();

    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
    });
    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
    });
    connect(ui->stat_2, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        on_stat_2_clicked();
    });
    connect(ui->research, &QPushButton::clicked, this, &MainWindow::on_research_clicked);
    connect(ui->map, &QPushButton::clicked, this, &MainWindow::on_map_clicked);
    connect(ui->lineEdit_id_locataire, &QLineEdit::editingFinished, this, &MainWindow::on_id_locataire_entered);
    afficherEspaces();
}

MainWindow::~MainWindow() {
    delete ui;  // Deleting the UI pointer or other cleanup code
}
void MainWindow::on_lineEdit_Nom_textChanged(const QString &text)
{
    if (text.length() >= 1) {
        getAISuggestions(text);
    }
}

void MainWindow::getAISuggestions(const QString &inputText)
{
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + OPENAI_API_KEY.toUtf8());

    QJsonObject message;
    message["role"] = "user";
    message["content"] = QString("Donne-moi 5 suggestions de noms pour un espace immobilier à partir de ce mot : \"%1\". Sépare chaque suggestion par une nouvelle ligne.").arg(inputText);

    QJsonArray messages;
    messages.append(message);

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    json["messages"] = messages;
    json["max_tokens"] = 150;
    json["temperature"] = 0.7;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            
            if (jsonResponse.isObject()) {
                QJsonObject obj = jsonResponse.object();
                if (obj.contains("choices")) {
                    QJsonArray choices = obj["choices"].toArray();
                    if (!choices.isEmpty()) {
                        QJsonObject choice = choices[0].toObject();
                        if (choice.contains("message")) {
                            QString suggestion = choice["message"].toObject()["content"].toString().trimmed();
                            
                            // Debug
                            qDebug() << "Suggestion reçue de l'API:\n" << suggestion;
                            
                            // Nettoyage et séparation
                            QStringList suggestionsList = suggestion.split("\n", Qt::SkipEmptyParts);
                            for (QString &s : suggestionsList) {
                                s = s.trimmed();
                                // Remove any numbering or bullet points using QRegularExpression
                                s = s.remove(QRegularExpression("^[0-9]+\\.\\s*"));
                                s = s.remove(QRegularExpression("^-\\s*"));
                            }
                            
                            // Filter out empty strings
                            suggestionsList.removeAll("");
                            
                            if (!suggestionsList.isEmpty()) {
                                completer->setModel(new QStringListModel(suggestionsList, this));
                                completer->complete();
                            }
                        }
                    }
                }
            }
        } else {
            qDebug() << "Erreur OpenAI: " << reply->errorString();
            qDebug() << "Réponse complète: " << reply->readAll();
        }
        reply->deleteLater();
    });
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
    QString type = ui->comboBox_Type->currentText();
    int superficie = ui->spinBox_Superficie->value();
    QString emplacement = ui->lineEdit_Emplacement->text();
    QString statut = ui->comboBox->currentText();
    QString dateDebut = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString dateFin = ui->dateEdit_2->date().toString("yyyy-MM-dd");

    // Get locataire ID
    QString idLocataireStr = ui->lineEdit_id_locataire->text().trimmed();
    if (idLocataireStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID du locataire !");
        return;
    }

    bool okId;
    int idLocataire = idLocataireStr.toInt(&okId);
    if (!okId || idLocataire <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID du locataire doit être un nombre valide !");
        return;
    }

    // Check if locataire exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM LOCATAIRE WHERE ID = :id");
    checkQuery.bindValue(":id", idLocataire);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification de l'ID du locataire !");
        return;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun locataire trouvé avec cet ID !");
        return;
    }

    // Check other required fields
    if (nom.isEmpty() || emplacement.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // Get and validate loyer
    QString loyerStr = ui->lineEdit_loyer->text().replace(",", ".");
    loyerStr.remove(QChar(0x200E));
    if (loyerStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un loyer !");
        return;
    }

    bool ok;
    double loyer = loyerStr.toDouble(&ok);
    if (!ok || loyer <= 0) {
        QMessageBox::warning(this, "Erreur", "Le loyer doit être un nombre valide et supérieur à 0 !");
        return;
    }

    // Check date consistency
    QDate startDate = ui->dateEdit->date();
    QDate endDate = ui->dateEdit_2->date();
    if (startDate >= endDate) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être après la date de début !");
        return;
    }

    // Create Espace and add to DB
    Espace Espace(nom, type, superficie, emplacement, statut, loyer, startDate, endDate, idLocataire);
    if (Espace.ajouter()) {
        QMessageBox::information(this, "Succès", "Espace ajouté avec succès !");

        // Clear form
        ui->lineEdit_Nom->clear();
        ui->comboBox_Type->setCurrentIndex(0);
        ui->spinBox_Superficie->setValue(1);
        ui->lineEdit_Emplacement->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->lineEdit_loyer->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit_2->setDate(QDate::currentDate().addYears(1));
        ui->lineEdit_id_locataire->clear();
        ui->lineEdit_Nom_3->clear();

        // Switch to list tab
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'espace !");
    }
}

void MainWindow::on_id_locataire_entered()
{
    QString idLocataireStr = ui->lineEdit_id_locataire->text().trimmed();

    if (idLocataireStr.isEmpty()) {
        ui->lineEdit_Nom_3->clear();
        return;
    }

    bool ok;
    int idLocataire = idLocataireStr.toInt(&ok);
    if (!ok || idLocataire <= 0) {
        ui->lineEdit_Nom_3->setText("ID invalide !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM LOCATAIRE WHERE ID = :id");
    query.bindValue(":id", idLocataire);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        ui->lineEdit_Nom_3->setText(nom + " " + prenom);
    } else {
        ui->lineEdit_Nom_3->setText("Aucun locataire trouvé avec cet ID");
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

void MainWindow::on_exporterPDF_clicked()
{
    // Demander à l'utilisateur où enregistrer le PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    // Configurer le PDF Writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Liste des espaces");

    // Initialiser le painter pour dessiner sur le PDF
    QPainter painter(&pdfWriter);

    // Marges larges pour éviter l'effet de compression
    int marginLeft = 100;
    int marginTop = 120;
    int x = marginLeft;
    int y = marginTop;

    // Titre clair et centré
    QString title = "Liste des espaces";
    QRect rectTitle(marginLeft, y, pdfWriter.width() - (marginLeft * 2), 80);
    painter.setFont(QFont("Helvetica", 20, QFont::Bold));
    painter.setPen(Qt::darkBlue);
    painter.drawText(rectTitle, Qt::AlignCenter, title);
    y += 100;

    // Hauteur de ligne fixe + colonne max à afficher par page
    int rowHeight = 50;
    int maxColsPerPage = 7;

    int colCount = ui->tableView->model()->columnCount();

    // Exclure la colonne "pdp" si elle existe
    int photoIndex = -1;
    for (int i = 0; i < colCount; ++i) {
        if (ui->tableView->model()->headerData(i, Qt::Horizontal).toString().toLower().contains("pdp")) {
            photoIndex = i;
            break;
        }
    }

    int adjustedColCount = (photoIndex != -1) ? colCount - 1 : colCount;

    // Gestion du nombre de colonnes affichées par page
    int colsToShow = qMin(maxColsPerPage, adjustedColCount);

    // Largeur des colonnes équilibrée
    int colWidth = (pdfWriter.width() - (2 * marginLeft) - (colsToShow * 10)) / colsToShow;

    // En-têtes (fond gris clair + bordures nettes)
    painter.setFont(QFont("Helvetica", 12, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));

    for (int i = 0, j = 0; i < colCount; ++i) {
        if (i == photoIndex) continue;

        if (j >= colsToShow) break;

        // Bordure nette et épaisse
        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight),
                         Qt::AlignCenter, ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
        x += colWidth + 10;
        j++;
    }

    y += rowHeight + 10;
    x = marginLeft;

    // Contenu du tableau (alternance de couleurs)
    painter.setFont(QFont("Helvetica", 10));
    painter.setPen(Qt::black);
    int rowCount = ui->tableView->model()->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QColor rowColor = (row % 2 == 0) ? QColor(245, 245, 245) : QColor(255, 255, 255);
        painter.setBrush(rowColor);

        for (int col = 0, j = 0; col < colCount; ++col) {
            if (col == photoIndex) continue;
            if (j >= colsToShow) break;

            // Bordure nette avec une couleur foncée
            painter.setPen(QColor(150, 150, 150));
            painter.drawRect(x, y, colWidth, rowHeight);

            QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString();

            // Tronquer le texte proprement avec "..."
            data = data.left(colWidth / 10) + (data.length() > colWidth / 10 ? "..." : "");

            // Texte aligné (texte à gauche, nombres à droite)
            Qt::Alignment alignment = (data.toDouble() || data.toInt())
                                          ? Qt::AlignRight | Qt::AlignVCenter
                                          : Qt::AlignLeft | Qt::AlignVCenter;

            painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight), alignment, data);
            x += colWidth + 10;
            j++;
        }

        x = marginLeft;
        y += rowHeight;

        // Saut de page automatique + répétition de l'en-tête
        if (y > pdfWriter.height() - marginTop) {
            pdfWriter.newPage();
            y = marginTop;

            // Répéter l'en-tête après le saut de page
            painter.setFont(QFont("Helvetica", 12, QFont::Bold));
            painter.setPen(Qt::black);
            painter.setBrush(QColor(230, 230, 230));

            x = marginLeft;
            for (int i = 0, j = 0; i < colCount; ++i) {
                if (i == photoIndex) continue;
                if (j >= colsToShow) break;

                painter.drawRect(x, y, colWidth, rowHeight);
                painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight),
                                 Qt::AlignCenter, ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
                x += colWidth + 10;
                j++;
            }

            y += rowHeight + 10;
            x = marginLeft;
        }
    }

    painter.end();

    // Ouvrir le PDF automatiquement avec l'application par défaut
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(filePath))) {
        QMessageBox::warning(this, "Avertissement", "Le PDF a été généré mais n'a pas pu être ouvert automatiquement.");
    } else {
        QMessageBox::information(this, "Succès", "Le fichier PDF a été généré et ouvert avec succès !");
    }
}

void MainWindow::on_tri_activated(int index)
{
    QString queryStr;

    // Build the query string based on the selected index
    if (index == 0) {
        // Sort by ID in ascending order
        queryStr = "SELECT * FROM ESPACE ORDER BY ID ASC";
    } else if (index == 1) {
        // Sort by Nom alphabetically
        queryStr = "SELECT * FROM ESPACE ORDER BY NOM ASC";
    }

    // Execute the query and set the model for the table
    QSqlQuery query;
    query.prepare(queryStr);
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête pour trier les espaces");
        return;
    }

    // Create a model to display the sorted data
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setQuery(std::move(query));  // Use std::move to avoid copying

    // Check if the model is valid
    if (model) {
        // Set the model to the QTableView
        ui->tableView->setModel(model);

        // Apply styles to the table view
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

        // Resize columns to fit contents
        ui->tableView->resizeColumnsToContents();
    } else {
        // Show an error message if the model couldn't be set
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des espaces.");
    }
}

void MainWindow::on_research_clicked()
{
    QString searchText = ui->lineEdit_search->text().trimmed();
    
    if (searchText.isEmpty()) {
        // If search text is empty, show all spaces
        afficherEspaces();
        return;
    }

    // Create a query to search in multiple columns
    QString queryStr = "SELECT * FROM ESPACE WHERE "
                      "NOM LIKE :search OR "
                      "TYPE LIKE :search OR "
                      "EMPLACEMENT LIKE :search OR "
                      "STATUT LIKE :search";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":search", "%" + searchText + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la recherche");
        return;
    }

    // Create a model to display the search results
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setQuery(std::move(query));

    if (model) {
        // Set the model to the QTableView
        ui->tableView->setModel(model);

        // Apply the same styles as in afficherEspaces()
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

        // Resize columns to fit contents
        ui->tableView->resizeColumnsToContents();

        // Show a message if no results were found
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Recherche", "Aucun espace ne correspond à votre recherche.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les résultats de la recherche.");
    }
}

void MainWindow::on_stat_2_clicked()
{
    // --- Statut (Occupé / Libre) ---
    int occupeCount = 0, libreCount = 0;
    QSqlQuery queryStatus("SELECT STATUT, COUNT(*) FROM ESPACE GROUP BY STATUT");
    while (queryStatus.next()) {
        QString statut = queryStatus.value(0).toString();
        int count = queryStatus.value(1).toInt();
        if (statut == "occupé")
            occupeCount = count;
        else if (statut == "libre")
            libreCount = count;
    }

    QPieSeries *statusSeries = new QPieSeries();
    statusSeries->append("Occupé", occupeCount);
    statusSeries->append("Libre", libreCount);

    for (QPieSlice *slice : statusSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
        // Animation pour chaque part
        slice->setExploded(true);
        slice->setLabelFont(QFont("Arial", 10));
    }

    QChart *statusChart = new QChart();
    statusChart->addSeries(statusSeries);
    statusChart->setTitle("Espaces par Statut");
    statusChart->legend()->setAlignment(Qt::AlignRight);

    // Activation de l'animation
    statusChart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *statusView = new QChartView(statusChart);
    statusView->setRenderHint(QPainter::Antialiasing);
    statusView->setFixedSize(500, 400);

    // --- Loyers par tranches ---
    int tranche1 = 0, tranche2 = 0, tranche3 = 0, tranche4 = 0;
    QSqlQuery queryLoyers("SELECT LOYER_M FROM ESPACE");
    while (queryLoyers.next()) {
        double loyer = queryLoyers.value(0).toDouble();
        if (loyer < 500)
            tranche1++;
        else if (loyer < 1000)
            tranche2++;
        else if (loyer < 1500)
            tranche3++;
        else
            tranche4++;
    }

    QBarSet *loyerSet = new QBarSet("Nombre d'espaces");
    *loyerSet << tranche1 << tranche2 << tranche3 << tranche4;

    QStringList tranches;
    tranches << "< 500€" << "500-999€" << "1000-1499€" << "1500€ et +";

    QBarSeries *loyerSeries = new QBarSeries();
    loyerSeries->append(loyerSet);

    QChart *loyerChart = new QChart();
    loyerChart->addSeries(loyerSeries);
    loyerChart->setTitle("Répartition des Espaces par Tranches de Loyer");

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(tranches);
    axisX->setTitleText("Tranches de Loyer");
    loyerChart->addAxis(axisX, Qt::AlignBottom);
    loyerSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre d'espaces");
    axisY->setLabelFormat("%d");
    loyerChart->addAxis(axisY, Qt::AlignLeft);
    loyerSeries->attachAxis(axisY);

    // Animation pour le graphique en barres
    loyerChart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *loyerView = new QChartView(loyerChart);
    loyerView->setRenderHint(QPainter::Antialiasing);
    loyerView->setFixedSize(500, 400);

    // --- Vérification de l'onglet des statistiques ---
    ui->tabWidget->setCurrentIndex(2);  // Assurez-vous que l'index de l'onglet est bien 2
    QWidget *tab = ui->tabWidget->widget(2);

    // Nettoyer l'ancien layout s'il existe
    QLayout *oldLayout = tab->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete oldLayout;
    }

    // Nouveau layout avec les 2 graphiques
    QHBoxLayout *layout = new QHBoxLayout();  // Pour les afficher côte à côte
    layout->addWidget(statusView);
    layout->addWidget(loyerView);
    tab->setLayout(layout);
}

void MainWindow::on_map_clicked()
{
    qDebug() << "Bouton map cliqué";

    // Vérifier si l'onglet de carte existe déjà
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->tabText(i) == "Carte") {
            qDebug() << "Onglet carte trouvé, passage à l'onglet";
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }

    qDebug() << "Création d'un nouvel onglet carte";
    showMapView();
}

void MainWindow::showMapView()
{
    qDebug() << "Début de showMapView()";

    // Création de l'onglet carte
    QWidget *mapTab = new QWidget();
    QVBoxLayout *mapLayout = new QVBoxLayout(mapTab);

    // Création de la ComboBox pour sélectionner un espace
    QComboBox *spaceSelector = new QComboBox();
    spaceSelector->addItem("🔍 Rechercher un espace");
    spaceSelector->setMinimumWidth(300);
    mapLayout->addWidget(spaceSelector, 0, Qt::AlignLeft);

    // Création de la scène
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1000);

    // Vue graphique avec paramètres de zoom améliorés
    QGraphicsView *view = new QGraphicsView(scene);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    view->viewport()->installEventFilter(this);  // Pour intercepter les événements de molette
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QBrush(QColor(240, 248, 255)));
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Changé pour permettre le défilement
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // Ajout des boutons de zoom
    QHBoxLayout *zoomLayout = new QHBoxLayout();
    QPushButton *zoomInBtn = new QPushButton("Zoom +");
    QPushButton *zoomOutBtn = new QPushButton("Zoom -");
    QPushButton *resetZoomBtn = new QPushButton("Reset Zoom");

    zoomLayout->addWidget(zoomOutBtn);
    zoomLayout->addWidget(resetZoomBtn);
    zoomLayout->addWidget(zoomInBtn);
    mapLayout->addLayout(zoomLayout);

    mapLayout->addWidget(view);

    // Connexions pour les boutons de zoom
    connect(zoomInBtn, &QPushButton::clicked, [view]() {
        view->scale(1.2, 1.2);
    });

    connect(zoomOutBtn, &QPushButton::clicked, [view]() {
        view->scale(1.0/1.2, 1.0/1.2);
    });

    connect(resetZoomBtn, &QPushButton::clicked, [view, scene]() {
        view->resetTransform();
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    });

    // [RESTE DU CODE ORIGINAL POUR LA CRÉATION DES ZONES ET ESPACES...]
    // Dessiner les zones A, B, C, D
    QMap<QString, QRectF> zones;
    zones["A"] = QRectF(100, 100, 600, 400);
    zones["B"] = QRectF(750, 100, 600, 400);
    zones["C"] = QRectF(100, 550, 600, 400);
    zones["D"] = QRectF(750, 550, 600, 400);

    // Couleurs des zones avec transparence
    QMap<QString, QColor> zoneColors = {
        {"A", QColor(230, 230, 250, 50)},
        {"B", QColor(255, 228, 225, 50)},
        {"C", QColor(240, 255, 240, 50)},
        {"D", QColor(255, 250, 205, 50)}
    };

    // Dessiner les zones et leurs labels
    for (const QString &zone : zones.keys()) {
        QGraphicsRectItem *zoneRect = new QGraphicsRectItem(zones[zone]);
        zoneRect->setBrush(QBrush(zoneColors[zone]));
        zoneRect->setPen(QPen(Qt::black, 2, Qt::DashLine));
        scene->addItem(zoneRect);

        QGraphicsTextItem *zoneLabel = new QGraphicsTextItem("Zone " + zone);
        zoneLabel->setFont(QFont("Arial", 16, QFont::Bold));
        zoneLabel->setDefaultTextColor(Qt::darkBlue);
        zoneLabel->setPos(zones[zone].x() + 10, zones[zone].y() + 10);
        scene->addItem(zoneLabel);
    }

    // Couleurs par type d'espace
    QMap<QString, QColor> typeColors = {
        {"Boutique de vêtements", QColor(255, 182, 193)},
        {"Restaurant", QColor(152, 251, 152)},
        {"Fast-food", QColor(255, 160, 122)},
        {"Supermarché", QColor(135, 206, 235)},
        {"Bijouterie", QColor(255, 215, 0)},
        {"Électronique", QColor(147, 112, 219)},
        {"Librairie", QColor(222, 184, 135)},
        {"Cosmétiques", QColor(255, 192, 203)},
        {"Cinéma", QColor(70, 130, 180)},
        {"Salle de jeux", QColor(255, 140, 0)}
    };

    // Récupérer les espaces
    QSqlQuery query;
    if (!query.exec("SELECT ID, NOM, TYPE, STATUT, EMPLACEMENT FROM ESPACE")) {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des espaces: " + query.lastError().text());
        return;
    }

    QMap<QString, ClickableRectItem*> espaceRects;
    QMap<QString, int> espaceCountPerZone;

    while (query.next()) {
        int id = query.value("ID").toInt();
        QString nom = query.value("NOM").toString();
        QString type = query.value("TYPE").toString();
        QString statut = query.value("STATUT").toString();
        QString emplacement = query.value("EMPLACEMENT").toString().toUpper();

        if (!zones.contains(emplacement)) {
            emplacement = "A";
        }

        QColor color = typeColors.value(type, QColor(200, 200, 200));
        if (statut.toLower() == "occupe") color = color.darker(125);

        // Position dans la zone
        if (!espaceCountPerZone.contains(emplacement)) {
            espaceCountPerZone[emplacement] = 0;
        }
        int countInZone = espaceCountPerZone[emplacement]++;
        int col = countInZone % 4;
        int row = countInZone / 4;

        qreal xPos = zones[emplacement].x() + 20 + col * 150;
        qreal yPos = zones[emplacement].y() + 50 + row * 120;

        // Rectangle pour l'espace
        ClickableRectItem *rect = new ClickableRectItem();
        rect->setRect(0, 0, 140, 100);
        rect->setBrush(QBrush(color));
        rect->setPen(QPen(Qt::black, 2));
        rect->setPos(xPos, yPos);

        // Connect signals
        connect(rect, &ClickableRectItem::hoverEntered, [rect]() {
            rect->setPen(QPen(Qt::red, 3));
        });
        
        connect(rect, &ClickableRectItem::hoverLeft, [rect]() {
            rect->setPen(QPen(Qt::black, 2));
        });
        
        connect(rect, &ClickableRectItem::clicked, [this, name = nom]() {
            show3DView(name);
        });

        // Nom de l'espace (centré)
        QString displayedName = nom.length() > 15 ? nom.left(12) + "..." : nom;
        QGraphicsTextItem *title = new QGraphicsTextItem(displayedName);
        title->setFont(QFont("Arial", 9, QFont::Bold));
        title->setDefaultTextColor(Qt::black);
        title->setPos(xPos + (140 - title->boundingRect().width()) / 2, yPos + 5);

        // Informations de base
        QGraphicsTextItem *info = new QGraphicsTextItem(QString("ID: %1\nType: %2").arg(id).arg(type));
        info->setFont(QFont("Arial", 7));
        info->setDefaultTextColor(Qt::black);
        info->setPos(xPos + 5, yPos + 25);

        // Bouton de statut
        QString statusColor = (statut.toLower() == "occupe") ? "#e74c3c" : "#2ecc71";
        QString statusText = (statut.toLower() == "occupe") ? "Occupé" : "Libre";

        QGraphicsRectItem *statusBox = new QGraphicsRectItem(xPos + 15, yPos + 68, 80, 20);
        statusBox->setBrush(QColor(statusColor));
        statusBox->setPen(QPen(Qt::black, 1));

        QGraphicsTextItem *statusBtn = new QGraphicsTextItem(statusText);
        statusBtn->setFont(QFont("Arial", 8, QFont::Bold));
        statusBtn->setDefaultTextColor(Qt::white);
        statusBtn->setPos(xPos + 15 + (80 - statusBtn->boundingRect().width()) / 2, yPos + 70);

        scene->addItem(rect);
        scene->addItem(title);
        scene->addItem(info);
        scene->addItem(statusBox);
        scene->addItem(statusBtn);

        espaceRects[nom] = rect;
        spaceSelector->addItem(nom + " (Zone " + emplacement + ")");
    }

    // Légende améliorée
    int legendX = 100;
    int legendY = 980;  // En bas de la carte
    QGraphicsRectItem *legendBg = new QGraphicsRectItem(legendX, legendY, 800, 40 + typeColors.size() * 25);
    legendBg->setBrush(QBrush(QColor(255, 255, 255, 200)));
    legendBg->setPen(QPen(Qt::black, 1));
    scene->addItem(legendBg);

    QGraphicsTextItem *legendTitle = new QGraphicsTextItem("🗺️ Légende");
    legendTitle->setFont(QFont("Arial", 12, QFont::Bold));
    legendTitle->setPos(60, 25);
    scene->addItem(legendTitle);

    int lx = 60, ly = 50;
    for (auto it = typeColors.begin(); it != typeColors.end(); ++it) {
        QGraphicsRectItem *colorBox = new QGraphicsRectItem(0, 0, 15, 15);
        colorBox->setBrush(it.value());
        colorBox->setPen(QPen(Qt::black));
        colorBox->setPos(lx, ly);
        scene->addItem(colorBox);

        QGraphicsTextItem *text = new QGraphicsTextItem(it.key());
        text->setFont(QFont("Arial", 8));
        text->setPos(lx + 20, ly - 3);
        scene->addItem(text);

        ly += 20;
    }

    // Légende des statuts
    QGraphicsRectItem *occupiedBox = new QGraphicsRectItem(lx, ly, 80, 20);
    occupiedBox->setBrush(QColor("#e74c3c"));
    occupiedBox->setPen(QPen(Qt::black, 1));
    scene->addItem(occupiedBox);

    QGraphicsTextItem *occupiedText = new QGraphicsTextItem("Occupé");
    occupiedText->setFont(QFont("Arial", 8, QFont::Bold));
    occupiedText->setDefaultTextColor(Qt::white);
    occupiedText->setPos(lx + (80 - occupiedText->boundingRect().width()) / 2, ly + 2);
    scene->addItem(occupiedText);

    QGraphicsRectItem *freeBox = new QGraphicsRectItem(lx + 100, ly, 80, 20);
    freeBox->setBrush(QColor("#2ecc71"));
    freeBox->setPen(QPen(Qt::black, 1));
    scene->addItem(freeBox);

    QGraphicsTextItem *freeText = new QGraphicsTextItem("Libre");
    freeText->setFont(QFont("Arial", 8, QFont::Bold));
    freeText->setDefaultTextColor(Qt::white);
    freeText->setPos(lx + 100 + (80 - freeText->boundingRect().width()) / 2, ly + 2);
    scene->addItem(freeText);

    // Centrer sur l'espace choisi
    connect(spaceSelector, &QComboBox::currentIndexChanged,
            this, [=](int index) {
                if (index > 0) {
                    QString selectedName = spaceSelector->currentText().split(" (Zone")[0];
                    if (espaceRects.contains(selectedName)) {
                        QGraphicsRectItem *selected = espaceRects[selectedName];
                        view->centerOn(selected);

                        // Reset toutes les bordures
                        for (QGraphicsRectItem *rect : espaceRects) {
                            rect->setPen(QPen(Qt::black, 2));
                        }

                        // Mettre en évidence l'espace sélectionné
                        selected->setPen(QPen(Qt::red, 3));
                    }
                }
            });

    // Ajouter l'onglet
    int mapTabIndex = ui->tabWidget->addTab(mapTab, "🗺️ Carte");
    ui->tabWidget->setCurrentIndex(mapTabIndex);
}

void MainWindow::show3DView(const QString &spaceName)
{
    // Show dimensions dialog
    DimensionsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        float largeur = dialog.getLargeur();
        float hauteur = dialog.getHauteur();
        float profondeur = dialog.getProfondeur();

        // Create and show 3D view
        Espace3DView *view3D = new Espace3DView(largeur, hauteur, profondeur);
        view3D->setWindowTitle("Vue 3D - " + spaceName);
        view3D->resize(800, 600);
        view3D->show();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    QGraphicsView *view = findChild<QGraphicsView*>();
    if (!view || watched != view->viewport()) {
        return QMainWindow::eventFilter(watched, event);
    }

    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);

        // Déterminer le facteur de zoom
        double factor = (wheelEvent->angleDelta().y() > 0) ? 1.1 : 1.0/1.1;

        // Limiter le zoom min/max
        if (view->transform().m11() * factor > 5.0 || view->transform().m11() * factor < 0.3) {
            return true;
        }

        // Appliquer le zoom
        view->scale(factor, factor);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}
