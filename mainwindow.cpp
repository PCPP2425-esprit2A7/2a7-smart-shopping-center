#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QPushButton>
#include "espace.h"
#include "modifierespacesDialog.h"
#include <QPdfWriter>
#include <QPageSize>
#include <QPageLayout>
#include <QPainter>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QtCharts>
#include <QPieSeries>
#include <QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QSqlTableModel>
#include <QCompleter>
#include <QLineEdit>
#include <QStringListModel>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QSqlError>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // <--- Cette ligne est OBLIGATOIRE



    // ui->tabWidget->tabBar()->hide();

    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Liste";
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Ajout";
    });

    connect(ui->stat_2, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        on_stat_2_clicked();
    });

    // Connect the research button
    connect(ui->research, &QPushButton::clicked, this, &MainWindow::on_research_clicked);
    connect(ui->lineEdit_id_locataire, &QLineEdit::editingFinished, this, &MainWindow::on_id_locataire_entered);
    setupAutocomplete();
    afficherEspaces();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupAutocomplete()
{
    // Create completer with existing space names
    completer = new QCompleter(Espace::getExistingSpaceNames());
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    
    // Set up the line edit with completer
    ui->lineEdit_Nom->setCompleter(completer);
    
    // Connect text changed signal
    connect(ui->lineEdit_Nom, &QLineEdit::textChanged,
            this, &MainWindow::on_lineEdit_Nom_textChanged);
}

void MainWindow::on_lineEdit_Nom_textChanged(const QString &text)
{
    // Get all general suggestions
    QStringList allSuggestions = Espace::getExistingSpaceNames();
    QStringList filteredNames;
    
    // If text is empty, show all suggestions
    if (text.isEmpty()) {
        filteredNames = allSuggestions;
    } else {
        // Filter suggestions based on input
        for (const QString &name : allSuggestions) {
            if (name.contains(text, Qt::CaseInsensitive)) {
                filteredNames.append(name);
            }
        }
        
        // Add intelligent suggestions based on the input
        if (text.length() >= 2) {
            QStringList smartSuggestions;
            
            // Analyze the input text for patterns
            QString lowerText = text.toLower();
            
            // Check for specific keywords and provide relevant suggestions
            if (lowerText.contains("bureau")) {
                smartSuggestions << "Bureau individuel" << "Bureau partagé" << "Bureau avec vue"
                               << "Bureau moderne" << "Bureau classique" << "Bureau exécutif";
            } else if (lowerText.contains("mag")) {
                smartSuggestions << "Magasin de détail" << "Magasin d'exposition" << "Magasin de stockage"
                               << "Magasin de proximité" << "Magasin spécialisé" << "Magasin de luxe";
            } else if (lowerText.contains("sal")) {
                smartSuggestions << "Salle de réunion" << "Salle d'exposition" << "Salle de conférence"
                               << "Salle de sport" << "Salle polyvalente" << "Salle de formation";
            } else if (lowerText.contains("loc")) {
                smartSuggestions << "Local commercial" << "Local artisanal" << "Local industriel"
                               << "Local de stockage" << "Local de service" << "Local technique";
            } else if (lowerText.contains("zon")) {
                smartSuggestions << "Zone de production" << "Zone de service" << "Zone de vente"
                               << "Zone administrative" << "Zone technique" << "Zone de stockage";
            } else if (lowerText.contains("esp")) {
                smartSuggestions << "Espace de coworking" << "Espace de stockage" << "Espace de vente"
                               << "Espace de réception" << "Espace de détente" << "Espace de travail";
            }
            
            // Add location-based suggestions if text contains location keywords
            if (lowerText.contains("centr") || lowerText.contains("ville")) {
                smartSuggestions << "Local Centre-ville" << "Bureau Centre-ville" << "Magasin Centre-ville"
                               << "Espace Centre-ville" << "Zone Centre-ville";
            } else if (lowerText.contains("indus")) {
                smartSuggestions << "Zone industrielle" << "Local industriel" << "Espace industriel"
                               << "Bureau industriel" << "Magasin industriel";
            }
            
            // Add premium/quality-based suggestions
            if (lowerText.contains("lux") || lowerText.contains("prem")) {
                smartSuggestions << "Bureau Premium" << "Magasin de luxe" << "Local Premium"
                               << "Espace Elite" << "Zone Premium";
            }
            
            // Add dynamic combinations based on what's already typed
            QStringList words = text.split(" ", Qt::SkipEmptyParts);
            if (words.size() >= 2) {
                QString lastWord = words.last();
                QStringList adjectives = {"Premium", "Elite", "Standard", "Basic", "Luxe", "Moderne", "Classique"};
                for (const QString &adj : adjectives) {
                    smartSuggestions << lastWord + " " + adj;
                }
            }
            
            // Add smart suggestions to filtered names
            filteredNames.append(smartSuggestions);
        }
    }
    
    // Update the completer model
    completer->setModel(new QStringListModel(filteredNames));
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
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Liste desespaces");

    QPainter painter(&pdfWriter);

    // ✅ Marges larges pour éviter l'effet de compression
    int marginLeft = 100;
    int marginTop = 120;
    int x = marginLeft;
    int y = marginTop;

    // ✅ Titre clair et centré
    QString title = "Liste des espaces";
    QRect rectTitle(marginLeft, y, pdfWriter.width() - (marginLeft * 2), 80);
    painter.setFont(QFont("Helvetica", 20, QFont::Bold));
    painter.setPen(Qt::darkBlue);
    painter.drawText(rectTitle, Qt::AlignCenter, title);
    y += 100;

    // ✅ Hauteur de ligne fixe + colonne max à afficher par page
    int rowHeight = 50;
    int maxColsPerPage = 7;

    int colCount = ui->tableView->model()->columnCount();

    // ✅ Exclure la colonne "pdp"
    int photoIndex = -1;
    for (int i = 0; i < colCount; ++i) {
        if (ui->tableView->model()->headerData(i, Qt::Horizontal).toString().toLower().contains("pdp")) {
            photoIndex = i;
            break;
        }
    }

    int adjustedColCount = (photoIndex != -1) ? colCount - 1 : colCount;

    // ✅ Gestion du nombre de colonnes affichées par page
    int colsToShow = qMin(maxColsPerPage, adjustedColCount);

    // ✅ Largeur des colonnes équilibrée
    int colWidth = (pdfWriter.width() - (2 * marginLeft) - (colsToShow * 10)) / colsToShow;

    // ✅ En-têtes (fond gris clair + bordures nettes)
    painter.setFont(QFont("Helvetica", 12, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));

    for (int i = 0, j = 0; i < colCount; ++i) {
        if (i == photoIndex) continue;

        if (j >= colsToShow) break;

        // ✅ Bordure nette et épaisse
        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight),
                         Qt::AlignCenter, ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
        x += colWidth + 10;
        j++;
    }

    y += rowHeight + 10;
    x = marginLeft;

    // ✅ Contenu du tableau (alternance de couleurs)
    painter.setFont(QFont("Helvetica", 10));
    painter.setPen(Qt::black);
    int rowCount = ui->tableView->model()->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QColor rowColor = (row % 2 == 0) ? QColor(245, 245, 245) : QColor(255, 255, 255);
        painter.setBrush(rowColor);

        for (int col = 0, j = 0; col < colCount; ++col) {
            if (col == photoIndex) continue;
            if (j >= colsToShow) break;

            // ✅ Bordure nette avec une couleur foncée
            painter.setPen(QColor(150, 150, 150));
            painter.drawRect(x, y, colWidth, rowHeight);

            QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString();

            // ✅ Tronquer le texte proprement avec "..."
            data = data.left(colWidth / 10) + (data.length() > colWidth / 10 ? "..." : "");

            // ✅ Texte aligné (texte à gauche, nombres à droite)
            Qt::Alignment alignment = (data.toDouble() || data.toInt())
                                          ? Qt::AlignRight | Qt::AlignVCenter
                                          : Qt::AlignLeft | Qt::AlignVCenter;

            painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight), alignment, data);
            x += colWidth + 10;
            j++;
        }

        x = marginLeft;
        y += rowHeight;

        // ✅ Saut de page automatique + répétition de l'en-tête
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

    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");
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



