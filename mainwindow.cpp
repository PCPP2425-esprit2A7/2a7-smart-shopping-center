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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Initialisation de l'interface
    afficherEvenement();
    connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::afficherEvenement);
    connect(ui->btnDialog, &QPushButton::clicked, this, &MainWindow::on_btnDialog_clicked);

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
    QString description = ui->desl->text(); // Utiliser text() pour QLineEdit
    QDate dateDebut = ui->ddl->date();
    QDate dateFin = ui->dfl->date();
    int capacite = ui->capl->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl->value();
    QString statut = ui->statusl->currentText();
    QString categorie = ui->catl->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl->text();
    QString lieu = ui->lieul->text(); // Ajout du champ lieu (assurez-vous que lieul existe dans l'UI)

    // Créer l’objet Evenement avec le lieu ajouté
    Evenement ev(titre, type, capacite, prix, lieu, description, dateDebut, dateFin, categorie, statut, organisateur, QString::number(1));

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
    QSqlQuery query("SELECT  TITRE, CAPACITE, PRIX, DESCRIPTION, DATE_DEB, DATE_FIN, STATUT, ORGANISATEUR,  LIEU FROM EVENEMENT");

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
                << query.value("LIEU").toString();
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

    QStringList headers = { "Titre", "Capacité", "Prix", "Description", "Date Début", "Date Fin",  "Statut", "Organisateur", "Lieu"};

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
