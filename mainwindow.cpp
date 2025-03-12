#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "service.h"
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
#include <QPdfWriter>
#include <QPainter>
#include <QSqlRecord>
#include <QPushButton>
#include <QEvent>


HoverButton::HoverButton(QWidget *parent) : QPushButton(parent)  // ✅ Implémentation correcte
{
    setAttribute(Qt::WA_Hover); // Active la gestion du survol
}
void HoverButton::enterEvent(QEnterEvent *event)
{
    // On vérifie si l'événement est bien un QEnterEvent
    if (event->type() == QEvent::Enter) {
        // Ici, on gère le survol du bouton (entrer dans la zone du bouton)
        emit hoverEntered();  // Émettre un signal pour signaler que le curseur est sur le bouton
    }
}

void HoverButton::leaveEvent(QEvent *event)
{
    // Vérification similaire pour leaveEvent
    if (event->type() == QEvent::Leave) {
        // Ici, on gère le départ du survol (le curseur quitte la zone du bouton)
        emit hoverLeft();  // Émettre un signal pour signaler que le curseur a quitté le bouton
    }

    // Appel de la méthode parente (facultatif)
    QPushButton::leaveEvent(event);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Cacher la barre des onglets pour forcer l'utilisation des boutons
    ui->tabWidget->tabBar()->hide();
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherServices);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::afficherServices);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->form, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->save_stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
      setUpNavigationButtons();
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
    // Récupérer les valeurs des champs
    QString nom = ui->lineEdit_Nom_2->text().trimmed();
    QString description = ui->textEdit_Description_2->toPlainText().trimmed();
    QString statut = ui->comboBox_Statut_2->currentText();
    QString frequence = ui->comboBox_Frequence_2->currentText();
    QDate dateDebut = ui->dateEdit_Debut_2->date();
    QDate dateFin = ui->dateEdit_Fin_2->date();

    // Vérifier si le champ coût est vide
    QString coutStr = ui->lineEdit_Cout_2->text().trimmed().replace(",", ".");
    coutStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom.isEmpty() || description.isEmpty() || coutStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // Vérifier la longueur du nom
    if (nom.length() > 50) {
        QMessageBox::warning(this, "Erreur", "Le nom ne doit pas dépasser 50 caractères !");
        return;
    }

    // Vérifier la longueur de la description
    if (description.length() > 255) {
        QMessageBox::warning(this, "Erreur", "La description est trop longue (max 255 caractères) !");
        return;
    }

    // Vérifier et convertir le coût
    bool ok;
    double cout = coutStr.toDouble(&ok);

    if (!ok || cout <= 0) {
        QMessageBox::warning(this, "Erreur", "Le coût doit être un nombre valide et positif !");
        return;
    }

    // Vérifier que la date de fin est postérieure à la date de début
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début !");
        return;
    }

    // Création du service et tentative d'ajout
    Service s(nom, description, cout, frequence, statut, dateDebut.toString("dd-MM-yyyy"), dateFin.toString("dd-MM-yyyy"), 1);

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




void MainWindow::on_pdf_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);
    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Arial", 10));

    int pageWidth = pdfWriter.width();
    int startX = 50;
    int startY = 80;
    int rowHeight = 40;  // Hauteur des lignes ajustée
    int colWidths[] = {50, 100, 200, 80, 80, 100, 80}; // Largeur des colonnes ajustée

    QSqlQuery query("SELECT * FROM SERVICE");
    QSqlRecord record = query.record();
    int columnCount = record.count();

    // Titre du document
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(startX, startY - 30, "Liste des Services");

    // Dessiner l'en-tête du tableau
    painter.setBrush(Qt::lightGray);
    painter.setPen(Qt::black);
    int colStartX = startX;

    for (int i = 0; i < columnCount; i++) {
        painter.drawRect(colStartX, startY, colWidths[i], rowHeight);
        painter.drawText(colStartX + 5, startY + 25, record.fieldName(i));
        colStartX += colWidths[i];
    }

    startY += rowHeight;
    painter.setFont(QFont("Arial", 10));
    painter.setBrush(Qt::NoBrush);

    // Dessiner les lignes du tableau
    while (query.next()) {
        colStartX = startX;
        for (int i = 0; i < columnCount; i++) {
            QString data = query.value(i).toString();
            QRect rect(colStartX, startY, colWidths[i], rowHeight);
            painter.drawRect(rect);
            painter.drawText(rect.adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::TextWordWrap, data);
            colStartX += colWidths[i];
        }
        startY += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");

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


void MainWindow::on_save_stat_clicked()
{
    // Sélectionner l'endroit où enregistrer l'image
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer l'image", "", "Images (*.png)");

    if (!filePath.isEmpty()) {
        // Capture du widget contenant les statistiques (remplace "statistique" par le bon nom)
        QPixmap pixmap = ui->tabWidget->widget(2)->grab();

        // Enregistrement de l'image
        if (pixmap.save(filePath)) {
            QMessageBox::information(this, "Succès", "L'image des statistiques a été enregistrée !");
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de l'enregistrement de l'image.");
        }
    }
}

void MainWindow::changerCouleurBouton() {
    QPushButton *button = qobject_cast<QPushButton *>(sender()); // Récupérer le bouton cliqué
    if (!button) return;

    // Liste de tous les boutons
    QList<QPushButton*> boutons = {ui->stat, ui->form, ui->liste, ui->save_stat}; // Ajoute tous tes boutons ici

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
void MainWindow::setUpNavigationButtons()
{
    // Créer un tableau de tous les boutons à gérer
    QList<QPushButton*> boutons = {ui->pushButton_1, ui->pushButton_2, ui->pushButton_3,
                                    ui->pushButton_5, ui->pushButton_7, ui->pushButton_6};

    // Remplacer chaque bouton classique par un HoverButton
    for (QPushButton* btn : boutons) {
        // Créer un nouveau HoverButton et le configurer
        HoverButton *hoverButton = new HoverButton(btn);

        // Assurez-vous que le parent du bouton est un QWidget valide
        QWidget* parentWidget = qobject_cast<QWidget*>(btn->parent());
        if (parentWidget) {
            hoverButton->setParent(parentWidget);  // Définir le parent pour la gestion mémoire
        }

        // Ajuster la taille et la position du HoverButton pour qu'il remplace visuellement l'ancien bouton
        hoverButton->setGeometry(btn->geometry());  // Copier la géométrie de l'ancien bouton

        // Définir explicitement l'icône du bouton si nécessaire
        hoverButton->setIcon(btn->icon());  // Copier l'icône de l'ancien bouton

        // Appliquer les styles à l'icône et autres propriétés
        hoverButton->setStyleSheet("QPushButton {"
                                   "background-color: rgb(227, 241, 244);"
                                   "border: 2px solid black;"
                                   "border-radius: 50px;"
                                   "qproperty-iconSize: 52px 52px;"
                                   "}");

        // Connexion des signaux hoverEntered et hoverLeft
        connect(hoverButton, &HoverButton::hoverLeft, this, &MainWindow::onLeaveNavigationButton);
        connect(hoverButton, &HoverButton::hoverEntered, this, &MainWindow::onEnterNavigationButton);


        // Ajouter le HoverButton au layout et l'afficher
        hoverButton->show();

        // Masquer l'ancien bouton
        btn->hide();
    }
}
void MainWindow::onEnterNavigationButton()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QPoint currentPos = button->pos();
        QPoint maxPos = currentPos + QPoint(70, 0); // Limite max

        // Vérifier que le bouton ne dépasse pas la limite
        if (button->x() < maxPos.x()) {
            QPropertyAnimation* animation = new QPropertyAnimation(button, "pos");
            animation->setDuration(200);
            animation->setEasingCurve(QEasingCurve::InOutQuad);

            animation->setEndValue(maxPos);
            animation->start();
        }
    }
}

void MainWindow::onLeaveNavigationButton()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QPoint currentPos = button->pos();
        QPoint minPos = currentPos - QPoint(70, 0); // Limite min

        // Vérifier que le bouton ne dépasse pas la limite
        if (button->x() > minPos.x()) {
            QPropertyAnimation* animation = new QPropertyAnimation(button, "pos");
            animation->setDuration(200);
            animation->setEasingCurve(QEasingCurve::InOutQuad);

            animation->setEndValue(minPos);
            animation->start();
        }
    }
}
