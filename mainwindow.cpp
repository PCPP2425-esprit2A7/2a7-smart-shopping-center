#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QBuffer>
#include "modifierdialog.h"
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QAbstractItemModel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug>
#include <QEnterEvent>
#include <QEvent>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QSqlQuery>
#include <QMap>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QWidget>
#include <QPen>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cheminImagePDP("")
{
    ui->setupUi(this);
    //applyButtonHoverEffect();
    //applyButtonHoverAnimation(); // Pour l'animation avec opacité



    // 🔥 Charger les employés dès le démarrage
    afficherEmployes();

    // Connexion des boutons aux slots
    //connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    //connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherEmployes);
    connect(ui->pushButton_modifier, &QPushButton::clicked, this, &MainWindow::modifierEmploye);
    connect(ui->boutonTrier, &QPushButton::clicked, this, &MainWindow::trierEmployes);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->ajout, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);

    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);


    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        qDebug() << "✅ Bouton ajout détecté";
    });



    // ✅ Masquer la barre des onglets
    ui->tabWidget->tabBar()->hide();

    // ✅ Gestion du changement d'onglet via les boutons
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

// ✅ Fonction d'affichage des employés
// 🔥 Fonction corrigée pour afficher les employés

void MainWindow::afficherEmployes()
{
    Employe Etmp;
    QSqlQueryModel* model = Etmp.afficher();

    if (model) {
        int rowCount = model->rowCount();
        qDebug() << "📊 Nombre de lignes dans le modèle :" << rowCount;
        //if (rowCount == 0) {
        //    QMessageBox::information(this, "Information", "Aucune donnée trouvée !");
        //}

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

    // Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    // ✅ Vérification du format de l'email
    QRegularExpression regexEmail("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::critical(this, "Erreur", "L'email n'est pas valide !");
        return;
    }

    // ✅ Vérification du numéro de téléphone (ex: 8 chiffres)
    QRegularExpression regexTel("^[0-9]{8,12}$");
    if (!regexTel.match(telephone).hasMatch()) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone doit contenir entre 8 et 12 chiffres !");
        return;
    }

    // ✅ Vérification du salaire
    bool ok;
    int salaire = ui->salaire->text().toInt(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide et positif.");
        return;
    }

    // ✅ Gestion de l'image
    QByteArray imageData;
    if (!cheminImagePDP.isEmpty()) {
        QPixmap pixmap(cheminImagePDP);
        if (!pixmap.isNull()) {
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            if (pixmap.save(&buffer, "PNG")) {
                qDebug() << "✅ Image convertie en QByteArray avec succès (" << imageData.size() << " octets)";
            } else {
                qDebug() << "❌ Échec de la conversion de l'image.";
            }
        } else {
            qDebug() << "❌ Chemin de l'image invalide.";
        }
    }

    // ✅ Création de l'employé avec l'image
    Employe employe(nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, imageData, statut);

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");

        // ✅ Réinitialisation des champs après ajout
        ui->nom->clear();
        ui->prenom->clear();
        ui->date_embauche->setDate(QDate::currentDate());
        ui->poste->clear();
        ui->email->clear();
        ui->sexe->setCurrentIndex(0);
        ui->telephone->clear();
        ui->salaire->clear();
        ui->statut->clear();
        ui->pushButton_pdp->setIcon(QIcon());  // Efface l'icône du bouton
        ui->pushButton_pdp2->setIcon(QIcon());  // Efface l'icône du bouton

        cheminImagePDP.clear();

        afficherEmployes(); // Rafraîchir la liste des employés
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
    }
}

// ✅ Fonction pour choisir une image
void MainWindow::on_pushButton_choisirImage_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_pdp->setIcon(QIcon(pixmap));
            ui->pushButton_pdp->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2->setIconSize(QSize(60, 60));
            cheminImagePDP = filePath;
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
        }
    }
}


// ✅ Fonction pour supprimer un employé
void MainWindow::on_pushButton_supprimer_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int id = selection.first().data().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Êtes-vous sûr de vouloir supprimer cet employé ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    Employe employe;
    if (employe.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'employé !");
    }
}

// ✅ Fonction pour modifier un employé
void MainWindow::modifierEmploye()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un employé à modifier.");
        return;
    }

    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    Employe employe = Employe::rechercher(id);
    if (employe.getNom().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Cet employé n'existe pas !");
        return;
    }

    ModifierDialog dialog(this);
    dialog.setEmploye(employe);

    if (dialog.exec() == QDialog::Accepted) {
        Employe employeModifie = dialog.getEmployeModifie();
        if (employeModifie.modifier(id)) {
            QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès.");
            afficherEmployes();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
        }
    }
}

void MainWindow::on_pushButton_exporterPDF_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Liste des employés");

    QPainter painter(&pdfWriter);

    // ✅ Marges larges pour éviter l'effet de compression
    int marginLeft = 100;
    int marginTop = 120;
    int x = marginLeft;
    int y = marginTop;

    // ✅ Titre clair et centré
    QString title = "Liste des employés";
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

void MainWindow::trierEmployes() {
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

void MainWindow::afficherStatistiques() {
    // 🟢 Accéder à l'onglet Statistiques
    QWidget *statistiquesTab = ui->tabWidget->widget(2);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(statistiquesTab->layout());

    // Si le layout n'existe pas, on le crée
    if (!layout) {
        layout = new QVBoxLayout(statistiquesTab);
        statistiquesTab->setLayout(layout);
    }

    // Supprimer tous les widgets existants dans le layout (et donc effacer les anciennes statistiques)
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Crée un layout horizontal pour placer les graphiques côte à côte
    QHBoxLayout *hLayout = new QHBoxLayout();
    layout->addLayout(hLayout); // Ajouter le layout horizontal à la disposition principale

    // =========================== 1️⃣ Histogramme des Salaires (avec animation fluide) ===========================
    QMap<QString, double> sommeSalaires;
    QMap<QString, int> nombreEmployes;

    QSqlQuery query("SELECT poste, salaire FROM employe");
    while (query.next()) {
        QString poste = query.value(0).toString();
        double salaire = query.value(1).toDouble();
        sommeSalaires[poste] += salaire;
        nombreEmployes[poste] += 1;
    }

    QBarSeries *series = new QBarSeries();
    QStringList categories;

    for (auto it = sommeSalaires.begin(); it != sommeSalaires.end(); ++it) {
        QBarSet *set = new QBarSet(it.key());
        *set << it.value();  // Somme des salaires pour chaque poste
        set->setColor(QColor::fromHsv(QRandomGenerator::global()->bounded(360), 255, 255)); // Couleurs dynamiques
        series->append(set);
        categories << it.key();
    }

    QChart *chart1 = new QChart();
    chart1->addSeries(series);
    chart1->setTitle("Somme des salaires par poste");
    chart1->setAnimationOptions(QChart::SeriesAnimations);  // ✅ Animation fluide sur les séries
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignBottom);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart1->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Total Salaires (en DT)");
    chart1->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setRenderHint(QPainter::Antialiasing);

    // Ajoute le graphique dans le layout horizontal
    hLayout->addWidget(chartView1);

    // =========================== 2️⃣ Diagramme Circulaire des Sexes (avec animation) ===========================
    int hommes = 0, femmes = 0;
    QSqlQuery querySexe("SELECT sexe FROM employe");
    while (querySexe.next()) {
        if (querySexe.value(0).toString().toLower() == "homme")
            hommes++;
        else
            femmes++;
    }

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Hommes", hommes);
    pieSeries->append("Femmes", femmes);

    QPieSlice *hommeSlice = pieSeries->slices().at(0);
    hommeSlice->setLabel(QString("Hommes (%1)").arg(hommes));
    hommeSlice->setBrush(Qt::blue);
    hommeSlice->setLabelVisible(true);
    hommeSlice->setExploded(true); // ✅ ANIMATION : Partie légèrement sortie

    QPieSlice *femmeSlice = pieSeries->slices().at(1);
    femmeSlice->setLabel(QString("Femmes (%1)").arg(femmes));
    femmeSlice->setBrush(Qt::red);
    femmeSlice->setLabelVisible(true);

    // Animation sur le survol des parts du diagramme circulaire
    QObject::connect(hommeSlice, &QPieSlice::hovered, [hommeSlice](bool state) {
        hommeSlice->setExploded(state); // ✅ Animation lors du survol
    });

    QObject::connect(femmeSlice, &QPieSlice::hovered, [femmeSlice](bool state) {
        femmeSlice->setExploded(state);
    });

    QChart *chart2 = new QChart();
    chart2->addSeries(pieSeries);
    chart2->setTitle("Répartition des employés par sexe");
    chart2->setAnimationOptions(QChart::SeriesAnimations);  // ✅ ANIMATION fluide sur les séries

    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);

    // Ajoute le graphique dans le layout horizontal
    hLayout->addWidget(chartView2);
}

void MainWindow::changerCouleurBouton() {
    QPushButton *button = qobject_cast<QPushButton *>(sender()); // Récupérer le bouton cliqué
    if (!button) return;

    // Liste de tous les boutons
    QList<QPushButton*> boutons = {ui->stat, ui->ajout, ui->liste}; // Ajoute tous tes boutons ici

    // Réinitialiser le style de tous les boutons
    for (QPushButton* btn : boutons) {
        btn->setStyleSheet("QPushButton {"
                           "background-color: rgb(227, 241, 244);"
                           "font: 700 9pt 'Segoe UI';"
                           "border: 2px solid black;"
                           "border-radius: 20px;"
                           "qproperty-iconSize: 30px 30px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: rgb(180, 210, 230);"
                           "}"); // Ajout de l'effet hover pour tous les boutons
    }


    // Appliquer le style uniquement au bouton cliqué
    button->setStyleSheet("QPushButton {"
                          "background-color: rgb(71, 127, 172);"  // Couleur sélectionnée
                          "color: white;"  // Texte en blanc pour contraste
                          "font: 700 9pt 'Segoe UI';"
                          "border: 2px solid black;"
                          "border-radius: 20px;"
                          "qproperty-iconSize: 30px 30px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: rgb(100, 150, 200);"  // Couleur du bouton survolé
                "}");
}
