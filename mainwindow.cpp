#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "service.h"
#include <QDebug>
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
#include <QTimer>
#include "mailsender.h"
#include <QTextBrowser>
#include <QDateTime>


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
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(shortcut, &QShortcut::activated, ui->sendButton, &QPushButton::click);
    shortcut->setContext(Qt::WidgetWithChildrenShortcut);  // Limite au widget courant et ses enfants

    // Cacher la barre des onglets pour forcer l'utilisation des boutons
    ui->tabWidget->tabBar()->hide();
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::afficherServices);
    connect(ui->liste, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->form, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->save_stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->btnTrierServices, &QPushButton::clicked, this, &MainWindow::trierServices);
    connect(ui->lineEdit_Recherche, &QLineEdit::textChanged, this, &MainWindow::rechercherService);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::envoyerRequete);
    connect(ui->btnGenererDescription, &QPushButton::clicked, this, &MainWindow::genererDescriptionIA);
    connect(ui->lineEdit_id, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_id_textChanged);
    connect(ui->lineEdit_id_2, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_id_textChanged2);


    openAIClient = new OpenAIClient();


    ui->lineEdit_Recherche->setPlaceholderText("🔎 Rechercher un service...");



      setUpNavigationButtons();
    connect(openAIClient, &OpenAIClient::descriptionGeneree, this, &MainWindow::afficherDescription);

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



void MainWindow::on_btnVoix_clicked()
{
    // Nettoie une ancienne instance si elle existe
    if (process) {
        process->kill();
        process->deleteLater();
    }

    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readVoiceOutput);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::processVoiceFinished);
    connect(process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error){
        qDebug() << "Erreur du processus :" << error;
    });

    QString scriptPath = "C:/Users/21698/Desktop/projet c++/ess1/voice_recognizer.py";
    process->start("python", QStringList() << scriptPath);

    if (!process->waitForStarted()) {
        qDebug() << "Échec du démarrage du script.";
    }
}

void MainWindow::readVoiceOutput()
{
    bufferOutput += process->readAllStandardOutput();
}

void MainWindow::processVoiceFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString finalOutput = bufferOutput.trimmed();
    qDebug() << "Texte reconnu :" << finalOutput;

    if (!finalOutput.isEmpty()) {
        ui->requete->setText(finalOutput);
    } else {
        ui->requete->setPlaceholderText("Aucune voix détectée");
    }

    process->deleteLater();
    process = nullptr;
    bufferOutput.clear();
}



void MainWindow::afficherDescription(const QString &description) {

    QTimer *timer = new QTimer(this);
    int *index = new int(0); // Stocke la position actuelle du texte

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*index < description.length()) {
            ui->textEdit_Description_2->insertPlainText(QString(description[*index]));
            (*index)++;
        } else {
            timer->stop();
            delete index;
            timer->deleteLater();
        }
    });

    timer->start(20);  // Vitesse d'affichage (50 ms par caractère)
}

void MainWindow::genererDescriptionIA() {
    QString nom = ui->lineEdit_Nom_2->text().trimmed();

    // Vérification des champs obligatoires
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de service.");
        return;
    }


    QString prompt = QString(
                         "Génère une description  pour un service nommé '%1'. "
                         ).arg(nom);

    openAIClient->envoyerRequeteDescription(prompt);
}


void MainWindow::envoyerRequete()
{
    QString question = ui->requete->toPlainText().trimmed(); // Prendre la question et enlever les espaces inutiles
    if (question.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une requête.");
        return;
    }

    QMessageBox::information(this, "Information", "Envoi de la requête à OpenAI...");

    openAIClient = new OpenAIClient();

    // Connexion au signal pour obtenir la requête SQL générée
    connect(openAIClient, &OpenAIClient::requeteSQLPr, this, &MainWindow::executerRequeteSQL);


    // Envoyer la requête à OpenAI
    openAIClient->envoyerRequete(question);
}

void MainWindow::executerRequeteSQL(const QString &sqlQuery)
{
    if (sqlQuery.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "La requête SQL générée est vide !");
        return;
    }


    QSqlQuery query;
    if (!query.exec(sqlQuery)) {
        QMessageBox::critical(this, "Erreur SQL", "L'exécution de la requête SQL a échoué.\n" + query.lastError().text());
        return;
    }

    // Mettre à jour la table avec les résultats
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du chargement des données : " + model->lastError().text());
        delete model;
        return;
    }

    ui->tableView->setModel(model);
    qDebug() << "Résultats affichés avec succès.";

    // Appliquer un style visuel
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



    // Vérifier et convertir le coût
    bool okCout;
    double cout = coutStr.toDouble(&okCout);

    // Conversion de l'ID en utilisant une variable bool distincte
    QString idText = ui->lineEdit_id->text().trimmed();
    idText.remove(QChar(0x200E)); // Supprimer le caractère parasite
    idText.remove(QChar(0x200F)); // Supprimer d'autres caractères invisibles

    bool okId;
    int id_espace = idText.toInt(&okId);

    qDebug() << "ID Entré après nettoyage :" << idText;
    qDebug() << "Conversion réussie ?" << okId << ", ID obtenu :" << id_espace;

    if (!okId || id_espace <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID de l'espace doit être un entier valide et positif !");
        return;
    }

    if (!okCout || cout <= 0) {
        QMessageBox::warning(this, "Erreur", "Le coût doit être un nombre valide et positif !");
        return;
    }

    // Vérifier que la date de fin est postérieure à la date de début
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début !");
        return;
    }

    // Création du service et tentative d'ajout
    Service s(nom, description, cout, frequence, statut, dateDebut.toString("dd-MM-yyyy"), dateFin.toString("dd-MM-yyyy"), id_espace);

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
        ui->lineEdit_id->clear();

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


void MainWindow::on_deleteButton_clicked()
{   Service service;
    // Récupérer l'ID du service sélectionné dans la QTableView
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un service à supprimer.");
        return;
    }

    // Récupérer l'ID du service depuis la première colonne de la ligne sélectionnée
    int idService = selectedRows.first().data().toInt();  // Exemple avec la première colonne qui contient l'ID


    if( service.supprimer(idService))
    { QMessageBox::information(this, "Succès", "Le service est supprimé");
    afficherServices();}

}

void MainWindow::on_pdf_clicked()
{
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
    QString title = "Liste des services";
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
    int baseUnit = pageWidth / (cols + descriptionExtra - 1);
    QMap<int, int> colWidths;

    for (int i = 0; i < cols; ++i) {
        int col = visibleCols[i];
        colWidths[col] = (col == descriptionCol) ? baseUnit * descriptionExtra : baseUnit;
    }

    int rowHeight = 40;
    x = left;

    // Dessiner entêtes
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

    // Dessiner les lignes de données
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
void MainWindow::afficherStatistiques() {
    QSqlQuery query;
    query.prepare("SELECT nom, cout FROM SERVICE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête");
        return;
    }

    std::vector<double> couts;
    std::vector<QString> noms;

    while (query.next()) {
        QString nom = query.value(0).toString();
        double cout = query.value(1).toDouble();
        noms.push_back(nom);
        couts.push_back(cout);
    }

    if (noms.empty()) {
        QMessageBox::warning(this, "Aucune donnée", "Aucun service trouvé pour les statistiques.");
        return;
    }

    // ** Création du graphique circulaire **
    QPieSeries *pieSeries = new QPieSeries();
    for (int i = 0; i < noms.size(); ++i) {
        QPieSlice *slice = pieSeries->append(noms[i], couts[i]);
        slice->setLabel(QString("%1\n%2 €").arg(noms[i]).arg(couts[i]));  // Ajout du label
        slice->setLabelVisible(true);
        slice->setExploded(true); // Effet d'explosion pour une meilleure visibilité
        slice->setPen(QPen(Qt::black));
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des Coûts par Service");
    pieChart->legend()->setAlignment(Qt::AlignRight);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setFixedSize(500, 400);  // Réduire la taille des graphiques

    // ** Création du graphique à barres **
    QBarSet *set = new QBarSet("Coût (€)");
    for (double cout : couts) {
        *set << cout;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Coûts des Services");
    barChart->setAnimationOptions(QChart::SeriesAnimations); // ✅ Animation activée

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Coût (€)");
    axisY->setLabelFormat("%.2f");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->setTitleText("Services");
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setFixedSize(500, 400);  // Réduire la taille des graphiques
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

    // Ajouter le titre et les graphiques dans un layout horizontal
    QVBoxLayout *mainLayout = new QVBoxLayout(statistiquesTab);

    // Ajouter les graphiques dans un layout horizontal
    QHBoxLayout *graphLayout = new QHBoxLayout();
    graphLayout->addWidget(pieChartView);
    graphLayout->addWidget(barChartView);
    mainLayout->addLayout(graphLayout);  // Ajouter le layout horizontal dans le layout principal
    statistiquesTab->setLayout(mainLayout);
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
                                   "border-radius: 30px;"
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
        QPoint maxPos = currentPos + QPoint(50, 0); // Limite max

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
        QPoint minPos = currentPos - QPoint(50, 0); // Limite min

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

    // Passer l'ID sélectionné au formulaire de modification (exemple : un QLineEdit)
    ///ui->lineEdit_idService->setText(QString::number(idService));
    idServiceAModifier = idService;

    // Aller à l'onglet d'indice 4
    ui->tabWidget->setCurrentIndex(3);
    loadServiceDetails(idService);


}

void MainWindow::loadServiceDetails(int id)
{
    Service service;
    if (service.loadById(id)) {  // Charger les détails du service sélectionné
        ui->lineEdit_Nom->setText(service.getNom());
        ui->textEdit_Description->setPlainText(service.getDescription());
        ui->lineEdit_Cout->setText(QString::number(service.getCout()));
        ui->comboBox_Frequence->setCurrentText(service.getFrequence());
        ui->comboBox_Statut->setCurrentText(service.getStatut());
        ui->dateEdit_Debut->setDate(QDate::fromString(service.getDateDebut(), "DD-MM-YY"));
        ui->dateEdit_Fin->setDate(QDate::fromString(service.getDateFin(), "DD-MM-YY"));
        ui->lineEdit_id_2->setText(QString::number(service.getIdEspace())); // Ajout de l'ID espace
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les détails du service.");
    }
}



void MainWindow::on_saveButton_clicked()
{
    Service service;
    QString nom = ui->lineEdit_Nom->text().trimmed();
    QString description = ui->textEdit_Description->toPlainText().trimmed();
    QString statut = ui->comboBox_Statut->currentText();
    QString frequence = ui->comboBox_Frequence->currentText();
    QDate dateDebut = ui->dateEdit_Debut->date();
    QDate dateFin = ui->dateEdit_Fin->date();

    // Récupérer et nettoyer le champ coût
    QString coutStr = ui->lineEdit_Cout->text().trimmed().replace(",", ".");
    coutStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    // Récupérer et nettoyer l'ID espace
    QString idEspaceStr = ui->lineEdit_id_2->text().trimmed();
    idEspaceStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom.isEmpty() || description.isEmpty() || coutStr.isEmpty() || idEspaceStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // Vérifier la longueur du nom
    if (nom.length() > 50) {
        QMessageBox::warning(this, "Erreur", "Le nom ne doit pas dépasser 50 caractères !");
        return;
    }




    // Vérifier et convertir le coût
    bool ok;
    double cout = coutStr.toDouble(&ok);
    if (!ok || cout <= 0) {
        QMessageBox::warning(this, "Erreur", "Le coût doit être un nombre valide et positif !");
        return;
    }

    // Vérifier et convertir l'ID espace
    int idEspace = idEspaceStr.toInt(&ok);
    if (!ok || idEspace <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID de l'espace doit être un entier valide et positif !");
        return;
    }

    // Vérifier que la date de fin est postérieure à la date de début
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début !");
        return;
    }

    // Modifier le service avec l'ID espace
    if (service.modifier(idServiceAModifier, nom, description, cout, frequence, statut,
                         dateDebut.toString("dd-MM-yyyy"), dateFin.toString("dd-MM-yyyy"), idEspace)) {
        QMessageBox::information(this, "Succès", "Service modifié avec succès !");
        afficherServices();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du service.");
    }
    QString nouveauStatut = ui->comboBox_Statut->currentText(); // Supposons que c'est un QComboBox



    if (nouveauStatut.toLower() == "suspendu") {
        // Récupération de l'ID_ESPACE du service
        QSqlQuery query;
        query.prepare("SELECT ID_ESPACE FROM SERVICE WHERE ID = :idService");
        query.bindValue(":idService", idServiceAModifier);

        int idEspace = -1;
        if (query.exec() && query.next()) {
            idEspace = query.value(0).toInt();
        } else {
            qDebug() << "Erreur : Impossible de récupérer l'ID_ESPACE.";
            return;
        }

        // Insertion dans la table MAINTENANCE
        QSqlQuery insertQuery;
        insertQuery.prepare(R"(
        INSERT INTO MAINTENANCE (ID, TYPE, DESCRIPTION, COUT, STATUT, DATE_RESOLUTION, ID_EMPLOYE, ID_ESPACE)
        VALUES (MAINTENANCE_SEQ.NEXTVAL, :type, :desc, :cout, :statut, SYSDATE, NULL, :espace)
    )");

        insertQuery.bindValue(":type", "Interruption de service");
        insertQuery.bindValue(":desc", "Ajout automatique suite à suspension du service ID " + QString::number(idServiceAModifier));
        insertQuery.bindValue(":cout", 0);  // si coût inconnu initialement
        insertQuery.bindValue(":statut", "En attente");
        insertQuery.bindValue(":espace", idEspace);

        if (insertQuery.exec()) {
            qDebug() << "Maintenance ajoutée avec succès.";
        } else {
            qDebug() << "Erreur lors de l'ajout de la maintenance :" << insertQuery.lastError().text();
        }

        qDebug() << "Le service est suspendu, envoi de l'e-mail en cours...";
        MailSender mail;
        QString idServiceStr = QString::number(idServiceAModifier);
        QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");

        QString body = R"(
<html>
<body style="font-family: Arial, sans-serif; color: #333;">
    <h2 style="color: #d9534f;">🚨 Alerte : Service Suspendu</h2>
    <p>Bonjour Ahmed,</p>
    <p>Ce message vous informe que le service suivant a été <strong>suspendu</strong> :</p>

    <table border="1" cellpadding="10" cellspacing="0" style="border-collapse: collapse; margin-top: 10px;">
        <tr style="background-color: #f2f2f2;">
            <th>ID du service</th>
            <td>)" + idServiceStr + R"(</td>
        </tr>
        <tr>
            <th>Date</th>
            <td>)" + dateStr + R"(</td>
        </tr>
    </table>

    <p style="margin-top: 20px; color: #8a2be2;">Merci de vérifier la situation dès que possible.</p>
    <p>Cordialement,<br><em>Votre application de gestion des services</em></p>
</body>
</html>
)";


        mail.sendEmail(
            "jouiniridha200@gmail.com",
            "Alerte : Service Suspendu",
            body  // <-- le body HTML
            );


        qDebug() << "Email envoyé ?";
    }

}

void MainWindow::trierServices() {
    // Vérifier si une colonne est sélectionnée
    int columnIndex = ui->tableView->currentIndex().column();

    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return;
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());
    if (!proxyModel) {
        // Création du proxyModel si nécessaire
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model()); // Assurez-vous que `serviceModel` est le modèle de la table des services
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView->setModel(proxyModel);
    }

    // Appliquer le tri sur la colonne sélectionnée
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    qDebug() << "Tri effectué sur la colonne : " << columnIndex;
}


void MainWindow::rechercherService(const QString &searchText) {
    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());  // Assure-toi que `serviceModel` est le modèle principal
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView->setModel(proxyModel);
    }

    // Filtrer par nom ou ID (Supposons que Nom = colonne 1 et ID = colonne 0)
    proxyModel->setFilterKeyColumn(-1);  // Permet de filtrer sur plusieurs colonnes
    proxyModel->setFilterRegularExpression(QRegularExpression(searchText, QRegularExpression::CaseInsensitiveOption));
}
void MainWindow::on_lineEdit_id_textChanged(const QString &text)
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Erreur : La base de données n'est pas connectée !";
        return;
    }

    QString idStr = ui->lineEdit_id->text().trimmed().remove(QChar(0x200E)); // Nettoyage ID
    qDebug() << "ID Saisi après nettoyage:" << idStr;

    bool ok;
    int id_espace = idStr.toInt(&ok);
    qDebug() << "ID converti :" << id_espace << ", Conversion réussie ?" << ok;

    if (!ok || id_espace <= 0) {
        qDebug() << "ID invalide";
        ui->nom_espace->setText("ID inexistant");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT nom FROM ESPACE WHERE id = :id");
    query.bindValue(":id", id_espace);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString nomEspace = query.value(0).toString();
        qDebug() << "Nom de l'espace récupéré :" << nomEspace;
        ui->nom_espace->setText(nomEspace);
    } else {
        qDebug() << "Aucun espace trouvé pour cet ID.";
        ui->nom_espace->setText("ID inexistant");
    }
}


void MainWindow::on_lineEdit_id_textChanged2(const QString &text)
{
    bool ok;
    int id_espace = text.trimmed().toInt(&ok);

    qDebug() << "ID saisi :" << text;
    qDebug() << "ID converti :" << id_espace;
    qDebug() << "Conversion réussie ?" << ok;

    if (ok && id_espace > 0) {
        QSqlQuery query;
        query.prepare("SELECT nom FROM ESPACE WHERE id = :id");
        query.bindValue(":id", id_espace);

        if (!query.exec()) {
            qDebug() << "Erreur SQL :" << query.lastError().text();
            return;
        }

        if (query.next()) {
            QString nomEspace = query.value(0).toString();
            qDebug() << "Nom de l'espace récupéré :" << nomEspace;
            ui->nom_espace_2->setText(nomEspace);
        } else {
            qDebug() << "Aucun espace trouvé pour cet ID.";
            ui->nom_espace_2->setText("ID inexistant");
        }
    } else {
        qDebug() << "ID invalide";
        ui->nom_espace_2->setText("ID inexistant");
    }
}
