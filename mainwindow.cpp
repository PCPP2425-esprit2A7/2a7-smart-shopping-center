#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"
#include <QMessageBox>  // Ajouté pour éviter l'erreur
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
#include <QTableWidget>
#include "connexion.h"
#include <QPrinter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTabBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->searchText->setPlaceholderText("🔎 Rechercher ...");

    todoModel = new QStandardItemModel(this);
    todoModel->setColumnCount(3);
    todoModel->setHorizontalHeaderLabels(QStringList() << "✔" << "Tâche" << "Statut");

    ui->table_todo->setModel(todoModel);
    ui->table_todo->horizontalHeader()->setStretchLastSection(true);
    ui->table_todo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_todo->setAlternatingRowColors(true);
    ui->table_todo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_todo->verticalHeader()->setVisible(false);
    ui->table_todo->horizontalHeader()->setStretchLastSection(true);
    ui->table_todo->setStyleSheet(R"(
    QTableView {
        font-family: 'Segoe UI', sans-serif;
        font-size: 14px;
        background-color: #f4f6f7;
        alternate-background-color: #e3e9ed;
        gridline-color: #d0d7de;
    }
    QHeaderView::section {
        background-color: #dbe5eb;
        padding: 6px;
        border: 1px solid #c2cfd6;
        font-weight: bold;
    }
)");


    Connection c;
    c.createconnect();
    tablewidget=new QTableWidget();
    model = new QSqlTableModel(this);
    model->setTable("evenements"); // Remplace "evenements" par le nom correct de ta table
    model->select();
    ui->tableView->setModel(model);

    //connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->liste_ev, &QPushButton::clicked, this, &MainWindow::afficherEvenement);
    connect(ui->stat_ev, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->btn_trierDate_ev, &QPushButton::clicked, this, &MainWindow::on_btn_trierDate_clicked);
    connect(ui->searchText, &QLineEdit::textChanged, this, &MainWindow::rechercherevenement);
    connect(ui->table_todo, &QTableView::clicked, this, &MainWindow::on_table_todo_clicked);


    // Initialiser le modèle pour afficher les événements dans le QListView
    eventModel = new QStringListModel(this);// listViewEvents doit être l'objet QListView dans ton UI




    // Cacher la barre des onglets pour forcer l'utilisateur à utiliser les boutons
    ui->tabWidget->tabBar()->hide();

    // Connexion des boutons pour changer d'onglet
    connect(ui->liste_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->ajout_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Affichage";
    });

    connect(ui->stat_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
    });
    connect(ui->chatbot, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Statistiques";
    });
    connect(ui->todo, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(6);
        qDebug() << "Passage à l'onglet TODO";
    });

    openAIClient = new OpenAIClient(this);
    connect(openAIClient, &OpenAIClient::reponseRecue, this, [](const QString &reponse) {
        qDebug() << "Réponse du chatbot : " << reponse;
    });
    connect(ui->btnEnvoyer_ev, &QPushButton::clicked, this, [=]() {
        QString question = ui->lineEditQuestion->text();
        openAIClient->envoyerRequete(question);
    });
    connect(openAIClient, &OpenAIClient::reponseRecue, this, [=](const QString &reponse) {

        QString stylesheet = "background-color: rgb(205, 239, 255); "
                             "border: 1px solid #A0C4FF; "
                             "border-radius: 16px; "
                             "padding: 4px 8px; "
                             "color: rgb(17, 35, 51); "
                             "font-size: 12px; "
                             "font-family: 'Arial Black', Arial, sans-serif; "
                             "max-width: 400px; "
                             "box-shadow: 1px 1px 5px rgba(0, 0, 0, 0.1);";

        QString stylesheet2 = "background-color: rgb(227, 241, 244); "
                              "border: 1px solid #BFD7EA; "
                              "border-radius: 16px; "
                              "padding: 4px 8px; "
                              "color: rgb(17, 35, 51); "
                              "font-size: 12px; "
                              "font-family: 'Arial Black', Arial, sans-serif; "
                              "max-width: 400px; "
                              "box-shadow: 1px 1px 5px rgba(0, 0, 0, 0.05);";


        ui->listWidget->setFrameShape(QFrame::NoFrame);
        addLabelToRightInListWidget(ui->listWidget, "\n"+ui->lineEditQuestion->text()+"\n",stylesheet,true,200);
        ui->lineEditQuestion->clear();
        addLabelToRightInListWidget(ui->listWidget,"\n"+reponse+"\n",stylesheet2,false,400);
        ui->listWidget->scrollToItem(ui->listWidget->item(ui->listWidget->count() - 1), QAbstractItemView::PositionAtBottom);



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
        "background-color: rgb(197, 203, 255); "
        "}";



    Evenement ev;
    QSqlQueryModel* model = ev.afficher();

    if (model) {
        ui->tableView->setModel(model);}

       copyTableViewToTableWidget(ui->tableView, tablewidget);
    // Appliquer le style à chaque bouton
    ui->liste_ev->setStyleSheet(buttonStyle);
    ui->ajout_ev->setStyleSheet(buttonStyle);
    ui->stat_ev->setStyleSheet(buttonStyle);
    ui->pushButton_choisirImage->setStyleSheet(buttonStyle);




    for(int i=0;i<tablewidget->rowCount();i++)tablewidget->setRowHeight(i,100);
    for(int i=0;i<tablewidget->columnCount();i++)tablewidget->setColumnWidth(i,180);
    tablewidget->horizontalHeader()->setFixedHeight(170);
    currentdate=QDate::currentDate();
    ui->groupBox_6moez_2->hide();
    connect(ui->calendrier, &QPushButton::clicked, this, [=]() {
        ui->calendrier_2->clearContents();
        int verif=0;
        QDate datee=currentdate;
        QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
        QDate datelable(currentdate);
        ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
        for(int i=0;i<ui->calendrier_2->rowCount();i++)
        {
            for(int j=0;j<ui->calendrier_2->columnCount();j++)
            {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
                label3->setFixedSize(70, 30);
                label3->setAlignment(Qt::AlignCenter);
                QWidget *container = new QWidget();

                // Créer un QVBoxLayout (vertical)
                QVBoxLayout *layout = new QVBoxLayout(container);
                //layout->addStretch();
                layout->addWidget(label3); // Espace flexible avant le label

                    // L'espace flexible après le label

                // Réinitialiser les marges du layout pour éviter l'espace inutile
                layout->setContentsMargins(0, 0, 0, 0);

                container->setLayout(layout);

                // Appliquer le style complet au container
                container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




                label3->setStyleSheet(
                    "border: none;"
                    "background-color :transparent; "  // Couleur de fond

                    );
                // Insérer le widget dans la cellule
                ui->calendrier_2->setCellWidget(i, j, container);
                ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

                qDebug()<<"verif est"<<verif;
                if(verif>0)verif--;
                for(int k=0;k<tablewidget->rowCount();k++)
                {
                    //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                    if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                    {



                        if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                        QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                        QDate date=firstDayOfMonth;
                        Evenement ev;
                        query = ev.getEvenementByDate(date);
                        while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                        QLabel *label = new QLabel(query.value("TITRE").toString());
                        label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                        label2->setAlignment(Qt::AlignCenter);
                        // Taille plus petite que la cellule
                        label->setFixedSize(140, 30);  // Change selon ton besoin
                        label2->setFixedSize(70, 30);
                        // Conteneur pour centrer le label sans changer la cellule
                        QWidget *container = new QWidget();

                        // Créer un QVBoxLayout (vertical)
                        QVBoxLayout *layout = new QVBoxLayout(container);

                        // Ajoute un espace flexible pour pousser le label vers le bas


                        layout->addStretch();
                        layout->addWidget(label2); // Espace flexible avant le label
                        layout->addWidget(label);       // Le label sera en bas
                            // L'espace flexible après le label

                        // Réinitialiser les marges du layout pour éviter l'espace inutile
                        layout->setContentsMargins(0, 0, 0, 0);

                        container->setLayout(layout);

                        // Appliquer le style complet au container
                        container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                        label->setStyleSheet(
                            "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                            );
                        label2->setStyleSheet(
                            "border: none;"
                            "background-color :transparent; "  // Couleur de fond

                            );
                        // Insérer le widget dans la cellule
                        ui->calendrier_2->setCellWidget(i, j, container);
            }
        }

       firstDayOfMonth=firstDayOfMonth.addDays(1);}

    }


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
    QString description = ui->desl->toPlainText(); // Utiliser toPlainText() pour QTextEdit
    QDate dateDebut = ui->ddl->date();
    QDate dateFin = ui->dfl->date();
    int capacite = ui->capl->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl->value();
    QString statut = ui->statusl->currentText();
    QString categorie = ui->catl->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl->text();
    int id_espace = ui->lieul->text().toInt();
    QByteArray imageData;
    if (!cheminImagePDP.isEmpty()) {
        QFile file(cheminImagePDP);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll();
            file.close();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de lire l'image pour l'affiche.");
        }    // Ajout du champ lieu (assurez-vous que lieul existe dans l'UI)
    }
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
    Evenement ev(titre, type, capacite, prix, description, dateDebut, dateFin, categorie, statut, organisateur, id_espace , imageData);

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
            // Affichage dans l'interface
            ui->affichel->setPixmap(pixmap);
            ui->affichel->setScaledContents(true);
            ui->affichel->setFixedSize(150, 150);
            cheminImagePDP = filePath;

            // Lecture du fichier image en QByteArray
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray imageData = file.readAll();
                file.close();

                // Transférer l'image à l'objet Evenement (exemple ici avec un membre evenementCourant)
                evenementCourant.setAffiche(imageData); // Assure-toi d'avoir ce setter dans Evenement
            } else {
                QMessageBox::warning(this, "Erreur", "Impossible de lire le fichier image.");
            }
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
    copyTableViewToTableWidget(ui->tableView,tablewidget);

}


void MainWindow::on_supprimer_event_clicked()
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

void MainWindow::chargerDetailsEvenement(int eventId)
{
    Evenement event;
    if (event.loadById(eventId)) {
        ui->modif_titre->setText(event.getTitre());
        ui->modif_type->setText(event.getType());
        ui->modif_capacite->setText(QString::number(event.getCapacite())); // ✅ CORRECT
        ui->modif_prix->setValue(event.getPrix());
        ui->modif_description->setPlainText(event.getDescription());
        ui->modif_dateDebut->setDate(event.getDateDebut());
        ui->modif_dateFin->setDate(event.getDateFin());
        ui->modif_categorie->setText(event.getCategorie());
        ui->modif_statut->setCurrentText(event.getStatut());
        ui->modif_organisateur->setText(event.getOrganisateur());
        ui->modif_lieu->setText(QString::number(event.getid_espace()));

        QString affichePath = event.getAffiche();
        if (!affichePath.isEmpty()) {
            QPixmap pixmap;
            if (pixmap.load(affichePath)) {
                ui->pushButton_affiche->setIcon(QIcon(pixmap));
                ui->pushButton_affiche->setIconSize(QSize(150, 150));            }
        }

        idEvenementAModifier = eventId;
        ui->tabWidget->setCurrentIndex(4); // aller à la page 5
    }
}

void MainWindow::on_pushButton_save_modif_clicked()
{
    if (idEvenementAModifier == -1) return;

    Evenement evenement;
    int capacite = ui->modif_capacite->text().toDouble();
    double prix = ui->modif_prix->value();
    int lieu = ui->modif_lieu->text().toInt();

    if (ui->modif_dateFin->date() <= ui->modif_dateDebut->date()) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        return;
    }

    if (evenement.modifier(idEvenementAModifier,
                           ui->modif_titre->text(),
                           ui->modif_type->text(),
                           capacite,
                           prix,
                           ui->modif_description->toPlainText(),
                           ui->modif_dateDebut->date().toString("dd-MM-yy"),
                           ui->modif_dateFin->date().toString("dd-MM-yy"),
                           ui->modif_categorie->text(),
                           ui->modif_statut->currentText(),
                           ui->modif_organisateur->text(),
                           lieu)) {
        ui->tabWidget->setCurrentIndex(1);
        afficherEvenement();
        QMessageBox::information(this, "Succès", "Événement modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::on_pushButton_modif_affiche_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_affiche->setIcon(QIcon(pixmap));
            ui->pushButton_affiche->setIconSize(QSize(150, 150));
            // ✅ En option : sauvegarde du chemin de l’image
            // evenemnt.setAffiche(filePath);  ← À gérer si tu veux stocker l'image
        } else {
            QMessageBox::warning(this, "Erreur", "Image invalide.");
        }
    }
}

void MainWindow::on_btnUpdate_ev_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à modifier.");
        return;
    }

    int eventId = selectedRows.first().siblingAtColumn(0).data().toInt(); // colonne 0 = ID
    chargerDetailsEvenement(eventId);  // Ouvre page 5 avec détails remplis
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


void MainWindow::on_pdf_ev_clicked()
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}



void MainWindow::on_btn_trierDate_clicked() {
    // Récupérer l'index de la colonne sélectionnée
    //1. Récupérer l’index de la colonne sélectionnée
    int columnIndex = ui->tableView->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    //2. Vérifier si aucune colonne n’est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return; // Arrêter la fonction si aucune colonne n'est sélectionnée
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    //Créer un QSortFilterProxyModel si besoin
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

void MainWindow::on_dateSelect_dateChanged(const QDate &date)
{
    updateEventList(date);
}

void MainWindow::updateEventList(const QDate &date)
{
    // Vérifier si des événements existent pour la date sélectionnée
    if (evenementsParDate.contains(date)) {
        QStringList events = evenementsParDate[date];
        eventModel->setStringList(events);  // Mettre à jour le modèle avec les événements
    } else {
        eventModel->setStringList(QStringList() << "Aucun événement pour cette date");
    }
}




void MainWindow::on_flechedroite_clicked()
{

    ui->calendrier_2->clearContents();
    currentdate=currentdate.addMonths(1);
    //ui->calendrier_2->clearContents();
    int verif=0;
    QDate datee=currentdate;
    QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
    QDate datelable(currentdate);
    ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
    for(int i=0;i<ui->calendrier_2->rowCount();i++)
    {
        for(int j=0;j<ui->calendrier_2->columnCount();j++)
        {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
            label3->setFixedSize(70, 30);
            label3->setAlignment(Qt::AlignCenter);
            QWidget *container = new QWidget();

            // Créer un QVBoxLayout (vertical)
            QVBoxLayout *layout = new QVBoxLayout(container);
            //layout->addStretch();
            layout->addWidget(label3); // Espace flexible avant le label

            // L'espace flexible après le label

            // Réinitialiser les marges du layout pour éviter l'espace inutile
            layout->setContentsMargins(0, 0, 0, 0);

            container->setLayout(layout);

            // Appliquer le style complet au container
            container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




            label3->setStyleSheet(
                "border: none;"
                "background-color :transparent; "  // Couleur de fond

                );
            // Insérer le widget dans la cellule
            ui->calendrier_2->setCellWidget(i, j, container);
            ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

            qDebug()<<"verif est"<<verif;
            if(verif>0)verif--;
            for(int k=0;k<tablewidget->rowCount();k++)
            {
                //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                {



                    if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                    QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                    QDate date=firstDayOfMonth;
                    Evenement ev;
                    query = ev.getEvenementByDate(date);
                    while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                    QLabel *label = new QLabel(query.value("TITRE").toString());
                    label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                    label2->setAlignment(Qt::AlignCenter);
                    // Taille plus petite que la cellule
                    label->setFixedSize(140, 30);  // Change selon ton besoin
                    label2->setFixedSize(70, 30);
                    // Conteneur pour centrer le label sans changer la cellule
                    QWidget *container = new QWidget();

                    // Créer un QVBoxLayout (vertical)
                    QVBoxLayout *layout = new QVBoxLayout(container);

                    // Ajoute un espace flexible pour pousser le label vers le bas


                    layout->addStretch();
                    layout->addWidget(label2); // Espace flexible avant le label
                    layout->addWidget(label);       // Le label sera en bas
                        // L'espace flexible après le label

                    // Réinitialiser les marges du layout pour éviter l'espace inutile
                    layout->setContentsMargins(0, 0, 0, 0);

                    container->setLayout(layout);

                    // Appliquer le style complet au container
                    container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                    label->setStyleSheet(
                        "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                        );
                    label2->setStyleSheet(
                        "border: none;"
                        "background-color :transparent; "  // Couleur de fond

                        );
                    // Insérer le widget dans la cellule
                    ui->calendrier_2->setCellWidget(i, j, container);
                }
            }

            firstDayOfMonth=firstDayOfMonth.addDays(1);}

    }

}


void MainWindow::on_flechegauche_clicked()
{
    ui->calendrier_2->clearContents();
    currentdate=currentdate.addMonths(-1);
    //ui->calendrier_2->clearContents();
    int verif=0;
    QDate datee=currentdate;
    QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
    QDate datelable(currentdate);
    ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
    for(int i=0;i<ui->calendrier_2->rowCount();i++)
    {
        for(int j=0;j<ui->calendrier_2->columnCount();j++)
        {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
            label3->setFixedSize(70, 30);
            label3->setAlignment(Qt::AlignCenter);
            QWidget *container = new QWidget();

            // Créer un QVBoxLayout (vertical)
            QVBoxLayout *layout = new QVBoxLayout(container);
            //layout->addStretch();
            layout->addWidget(label3); // Espace flexible avant le label

            // L'espace flexible après le label

            // Réinitialiser les marges du layout pour éviter l'espace inutile
            layout->setContentsMargins(0, 0, 0, 0);

            container->setLayout(layout);

            // Appliquer le style complet au container
            container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




            label3->setStyleSheet(
                "border: none;"
                "background-color :transparent; "  // Couleur de fond

                );
            // Insérer le widget dans la cellule
            ui->calendrier_2->setCellWidget(i, j, container);
            ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

            qDebug()<<"verif est"<<verif;
            if(verif>0)verif--;
            for(int k=0;k<tablewidget->rowCount();k++)
            {
                //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                {



                    if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                    QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                    QDate date=firstDayOfMonth;
                    Evenement ev;
                    query = ev.getEvenementByDate(date);
                    while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                    QLabel *label = new QLabel(query.value("TITRE").toString());
                    label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                    label2->setAlignment(Qt::AlignCenter);
                    // Taille plus petite que la cellule
                    label->setFixedSize(140, 30);  // Change selon ton besoin
                    label2->setFixedSize(70, 30);
                    // Conteneur pour centrer le label sans changer la cellule
                    QWidget *container = new QWidget();

                    // Créer un QVBoxLayout (vertical)
                    QVBoxLayout *layout = new QVBoxLayout(container);

                    // Ajoute un espace flexible pour pousser le label vers le bas


                    layout->addStretch();
                    layout->addWidget(label2); // Espace flexible avant le label
                    layout->addWidget(label);       // Le label sera en bas
                        // L'espace flexible après le label

                    // Réinitialiser les marges du layout pour éviter l'espace inutile
                    layout->setContentsMargins(0, 0, 0, 0);

                    container->setLayout(layout);

                    // Appliquer le style complet au container
                    container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                    label->setStyleSheet(
                        "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                        );
                    label2->setStyleSheet(
                        "border: none;"
                        "background-color :transparent; "  // Couleur de fond

                        );
                    // Insérer le widget dans la cellule
                    ui->calendrier_2->setCellWidget(i, j, container);
                }
            }

            firstDayOfMonth=firstDayOfMonth.addDays(1);}

    }
}

void MainWindow::on_pushButton_13moez_2_clicked()
{
    ui->groupBox_6moez_2->hide();
}


void MainWindow::on_calendrier_2_cellClicked(int row, int column)
{
    // Récupérer les valeurs des champs
    ui->titrel_3->clear();
    ui->desl_3->clear();
    ui->ddl_3->clear();
    ui->dfl_3->clear();
    ui->capl_3->clear();
    ui->prixl_3->clear();
    ui->catl_3->clear();
    ui->typl_3->clear();
    ui->orgl_3->clear();
    ui->lieul_3->clear();

    ui->groupBox_6moez_2->show();
QWidget *cellWidget = ui->calendrier_2->cellWidget(row, column);
QList<QLabel*> labels = cellWidget->findChildren<QLabel*>();
int nombreLabels = labels.size();




    if ( nombreLabels == 1){
        qDebug()<<"dateee"<< ui->calendrier_2->item(row,column)->text()+ui->label_15moez_2->text();
        QString input = ui->calendrier_2->item(row,column)->text()+ui->label_15moez_2->text();;
        input = input.trimmed();  // Supprimer les espaces


        // Expression régulière : extraire jour (2 chiffres), mois (1-2 chiffres), année (4 chiffres)
        QRegularExpression re("(\\d{1,2})(\\d{1,2})/(\\d{4})");
        QRegularExpressionMatch match = re.match(input);

        if (match.hasMatch()) {
            int day = match.captured(1).toInt();
            int month = match.captured(2).toInt();
            int year = match.captured(3).toInt();

            QDate date(year, month, day);
            if (date.isValid()) {
                QDate date(year, month, day);
                ui->ddl_3->setDate(date);
                qDebug() << "✅ Date valide:" << date.toString("dd/MM/yyyy");
            } else {
                qDebug() << "❌ Date invalide après conversion";
            }
        } else {
            qDebug() << "❌ Format non reconnu";
        }





              // "2025"

            /*QDate date(year, month, day);
            ui->ddl_3->setDate(date);*/}

    else
    {
        QString input = ui->calendrier_2->item(row,column)->text()+ui->label_15moez_2->text();;
        input = input.trimmed();  // Supprimer les espaces



        // Expression régulière : extraire jour (2 chiffres), mois (1-2 chiffres), année (4 chiffres)
        QRegularExpression re("(\\d{1,2})(\\d{1,2})/(\\d{4})");
        QRegularExpressionMatch match = re.match(input);

        if (match.hasMatch()) {
            int day = match.captured(1).toInt();
            int month = match.captured(2).toInt();
            int year = match.captured(3).toInt();

            QDate date(year, month, day);
            if (date.isValid()) {
                QDate date(year, month, day);
                Evenement ev;
                query = ev.getEvenementByDate(date);
                while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                ui->titrel_3->setText(query.value("TITRE").toString());
                ui->desl_3->setPlainText(query.value("DESCRIPTION").toString());
                ui->ddl_3->setDate(query.value("DATE_DEB").toDate());
                ui->dfl_3->setDate(query.value("DATE_FIN").toDate());
                ui->capl_3->setText(query.value("CAPACITE").toString());
                ui->prixl_3->setValue(query.value("PRIX").toDouble());
                ui->statusl_3->setCurrentText(query.value("STATUT").toString());
                ui->catl_3->setText(query.value("CATEGORIE").toString());
                ui->typl_3->setText(query.value("TYPE").toString());
                ui->orgl_3->setText(query.value("ORGANISATEUR").toString());
                ui->lieul_3->setText(query.value("ID_ESPACE").toString());
                qDebug() << "✅ Date valide:" << date.toString("dd/MM/yyyy");
            } else {
                qDebug() << "❌ Date invalide après conversion";
            }
        } else {
            qDebug() << "❌ Format non reconnu";
        }

    }




}


void MainWindow::on_planifier_clicked() //valider la planification
{
    ui->groupBox_6moez_2->hide();
    QString titre = ui->titrel_3->text();
    QString description = ui->desl_3->toPlainText(); // Utiliser toPlainText() pour QTextEdit
    QDate dateDebut = ui->ddl_3->date();
    QDate dateFin = ui->dfl_3->date();
    int capacite = ui->capl_3->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl_3->value();
    QString statut = ui->statusl_3->currentText();
    QString categorie = ui->catl_3->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl_3->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl_3->text();
    int id_espace = ui->lieul_3->text().toInt();


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
    Evenement ev(titre, type, capacite, prix, description, dateDebut, dateFin,
                 categorie, statut, organisateur, id_espace);


    // Appeler la méthode ajouter()
    if (ev.ajouter()) {
        QMessageBox::information(this, "Succès", "Événement ajouté avec succès !");
        afficherEvenement();
        ui->calendrier_2->clearContents();

        int verif=0;
        //SQDate datee=currentdate;
        QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
        QDate datelable(currentdate);
        ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
        for(int i=0;i<ui->calendrier_2->rowCount();i++)
        {
            for(int j=0;j<ui->calendrier_2->columnCount();j++)
            {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
                label3->setFixedSize(70, 30);
                label3->setAlignment(Qt::AlignCenter);
                QWidget *container = new QWidget();

                // Créer un QVBoxLayout (vertical)
                QVBoxLayout *layout = new QVBoxLayout(container);
                //layout->addStretch();
                layout->addWidget(label3); // Espace flexible avant le label

                // L'espace flexible après le label

                // Réinitialiser les marges du layout pour éviter l'espace inutile
                layout->setContentsMargins(0, 0, 0, 0);

                container->setLayout(layout);

                // Appliquer le style complet au container
                container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




                label3->setStyleSheet(
                    "border: none;"
                    "background-color :transparent; "  // Couleur de fond

                    );
                // Insérer le widget dans la cellule
                ui->calendrier_2->setCellWidget(i, j, container);
                ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

                qDebug()<<"verif est"<<verif;
                if(verif>0)verif--;
                for(int k=0;k<tablewidget->rowCount();k++)
                {
                    //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                    if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                    {



                        if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                        QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                        QDate date=firstDayOfMonth;
                        Evenement ev;
                        query = ev.getEvenementByDate(date);
                        while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                        QLabel *label = new QLabel(query.value("TITRE").toString());
                        label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                        label2->setAlignment(Qt::AlignCenter);
                        // Taille plus petite que la cellule
                        label->setFixedSize(140, 30);  // Change selon ton besoin
                        label2->setFixedSize(70, 30);
                        // Conteneur pour centrer le label sans changer la cellule
                        QWidget *container = new QWidget();

                        // Créer un QVBoxLayout (vertical)
                        QVBoxLayout *layout = new QVBoxLayout(container);

                        // Ajoute un espace flexible pour pousser le label vers le bas


                        layout->addStretch();
                        layout->addWidget(label2); // Espace flexible avant le label
                        layout->addWidget(label);       // Le label sera en bas
                            // L'espace flexible après le label

                        // Réinitialiser les marges du layout pour éviter l'espace inutile
                        layout->setContentsMargins(0, 0, 0, 0);

                        container->setLayout(layout);

                        // Appliquer le style complet au container
                        container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                        label->setStyleSheet(
                            "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                            );
                        label2->setStyleSheet(
                            "border: none;"
                            "background-color :transparent; "  // Couleur de fond

                            );
                        // Insérer le widget dans la cellule
                        ui->calendrier_2->setCellWidget(i, j, container);
                    }
                }

                firstDayOfMonth=firstDayOfMonth.addDays(1);}

        }

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l’ajout de l’événement !");
    }
}

void MainWindow::on_supprimer_ev_clicked() //supprimer l'evenement du calendrier
{

    ui->groupBox_6moez_2->hide();
    Evenement ev;
    if (ev.supprimer(query.value("ID").toInt())) {
        QMessageBox::information(this, "Succès", "Événement supprimé avec succès.");
        // Rafraîchir l'affichage après suppression
        afficherEvenement();
        ui->calendrier_2->clearContents();
        int verif=0;
        //QDate datee=currentdate;
        QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
        QDate datelable(currentdate);
        ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
        ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
        for(int i=0;i<ui->calendrier_2->rowCount();i++)
        {
            for(int j=0;j<ui->calendrier_2->columnCount();j++)
            {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
                label3->setFixedSize(70, 30);
                label3->setAlignment(Qt::AlignCenter);
                QWidget *container = new QWidget();

                // Créer un QVBoxLayout (vertical)
                QVBoxLayout *layout = new QVBoxLayout(container);
                //layout->addStretch();
                layout->addWidget(label3); // Espace flexible avant le label

                // L'espace flexible après le label

                // Réinitialiser les marges du layout pour éviter l'espace inutile
                layout->setContentsMargins(0, 0, 0, 0);

                container->setLayout(layout);

                // Appliquer le style complet au container
                container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




                label3->setStyleSheet(
                    "border: none;"
                    "background-color :transparent; "  // Couleur de fond

                    );
                // Insérer le widget dans la cellule
                ui->calendrier_2->setCellWidget(i, j, container);
                ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

                qDebug()<<"verif est"<<verif;
                if(verif>0)verif--;
                for(int k=0;k<tablewidget->rowCount();k++)
                {
                    //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                    if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                    {



                        if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                        QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                        QDate date=firstDayOfMonth;
                        Evenement ev;
                        query = ev.getEvenementByDate(date);
                        while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                        QLabel *label = new QLabel(query.value("TITRE").toString());
                        label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                        label2->setAlignment(Qt::AlignCenter);
                        // Taille plus petite que la cellule
                        label->setFixedSize(140, 30);  // Change selon ton besoin
                        label2->setFixedSize(70, 30);
                        // Conteneur pour centrer le label sans changer la cellule
                        QWidget *container = new QWidget();

                        // Créer un QVBoxLayout (vertical)
                        QVBoxLayout *layout = new QVBoxLayout(container);

                        // Ajoute un espace flexible pour pousser le label vers le bas


                        layout->addStretch();
                        layout->addWidget(label2); // Espace flexible avant le label
                        layout->addWidget(label);       // Le label sera en bas
                            // L'espace flexible après le label

                        // Réinitialiser les marges du layout pour éviter l'espace inutile
                        layout->setContentsMargins(0, 0, 0, 0);

                        container->setLayout(layout);

                        // Appliquer le style complet au container
                        container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                        label->setStyleSheet(
                            "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                            );
                        label2->setStyleSheet(
                            "border: none;"
                            "background-color :transparent; "  // Couleur de fond

                            );
                        // Insérer le widget dans la cellule
                        ui->calendrier_2->setCellWidget(i, j, container);
                    }
                }

                firstDayOfMonth=firstDayOfMonth.addDays(1);}

        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'événement.");
    }
}


void MainWindow::on_valide_date_clicked() //pour la recherche de la date
{
    ui->calendrier_2->clearContents();
    currentdate=ui->date->date();
    //ui->calendrier_2->clearContents();
    int verif=0;
    //QDate datee=currentdate;
    QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
    QDate datelable(currentdate);
    ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
    for(int i=0;i<ui->calendrier_2->rowCount();i++)
    {
        for(int j=0;j<ui->calendrier_2->columnCount();j++)
        {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
            label3->setFixedSize(70, 30);
            label3->setAlignment(Qt::AlignCenter);
            QWidget *container = new QWidget();

            // Créer un QVBoxLayout (vertical)
            QVBoxLayout *layout = new QVBoxLayout(container);
            //layout->addStretch();
            layout->addWidget(label3); // Espace flexible avant le label

            // L'espace flexible après le label

            // Réinitialiser les marges du layout pour éviter l'espace inutile
            layout->setContentsMargins(0, 0, 0, 0);

            container->setLayout(layout);

            // Appliquer le style complet au container
            container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




            label3->setStyleSheet(
                "border: none;"
                "background-color :transparent; "  // Couleur de fond

                );
            // Insérer le widget dans la cellule
            ui->calendrier_2->setCellWidget(i, j, container);
            ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

            qDebug()<<"verif est"<<verif;
            if(verif>0)verif--;
            for(int k=0;k<tablewidget->rowCount();k++)
            {
                //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                {



                    if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                    QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                    QDate date=firstDayOfMonth;
                    Evenement ev;
                    query = ev.getEvenementByDate(date);
                    while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                    QLabel *label = new QLabel(query.value("TITRE").toString());
                    label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                    label2->setAlignment(Qt::AlignCenter);
                    // Taille plus petite que la cellule
                    label->setFixedSize(140, 30);  // Change selon ton besoin
                    label2->setFixedSize(70, 30);
                    // Conteneur pour centrer le label sans changer la cellule
                    QWidget *container = new QWidget();

                    // Créer un QVBoxLayout (vertical)
                    QVBoxLayout *layout = new QVBoxLayout(container);

                    // Ajoute un espace flexible pour pousser le label vers le bas


                    layout->addStretch();
                    layout->addWidget(label2); // Espace flexible avant le label
                    layout->addWidget(label);       // Le label sera en bas
                        // L'espace flexible après le label

                    // Réinitialiser les marges du layout pour éviter l'espace inutile
                    layout->setContentsMargins(0, 0, 0, 0);

                    container->setLayout(layout);

                    // Appliquer le style complet au container
                    container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                    label->setStyleSheet(
                        "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                        );
                    label2->setStyleSheet(
                        "border: none;"
                        "background-color :transparent; "  // Couleur de fond

                        );
                    // Insérer le widget dans la cellule
                    ui->calendrier_2->setCellWidget(i, j, container);
                }
            }

            firstDayOfMonth=firstDayOfMonth.addDays(1);}
    }}


void MainWindow::on_calendrier_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
    ui->calendrier_2->clearContents();
    int verif=0;
    QDate datee=currentdate;
    QDate firstDayOfMonth(currentdate.year(), currentdate.month(), 1);
    QDate datelable(currentdate);
    ui->label->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_4->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_5->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_6->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_7->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_9->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_10->setText("    "+datelable.toString("dddd"));datelable=datelable.addDays(1);
    ui->label_15moez_2->setText(QString::number( currentdate.month())+"/"+QString::number( currentdate.year()));
    for(int i=0;i<ui->calendrier_2->rowCount();i++)
    {
        for(int j=0;j<ui->calendrier_2->columnCount();j++)
        {   QLabel *label3 = new QLabel(firstDayOfMonth.toString("dd"));
            label3->setFixedSize(70, 30);
            label3->setAlignment(Qt::AlignCenter);
            QWidget *container = new QWidget();

            // Créer un QVBoxLayout (vertical)
            QVBoxLayout *layout = new QVBoxLayout(container);
            //layout->addStretch();
            layout->addWidget(label3); // Espace flexible avant le label
            // Réinitialiser les marges du layout pour éviter l'espace inutile
            layout->setContentsMargins(0, 0, 0, 0);

            container->setLayout(layout);

            // Appliquer le style complet au container
            container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");




            label3->setStyleSheet(
                "border: none;"
                "background-color :transparent; "  // Couleur de fond

                );
            // Insérer le widget dans la cellule
            ui->calendrier_2->setCellWidget(i, j, container);
            ui->calendrier_2->setItem(i,j,new QTableWidgetItem("      "+firstDayOfMonth.toString("dd")));

            qDebug()<<"verif est"<<verif;
            if(verif>0)verif--;
            for(int k=0;k<tablewidget->rowCount();k++)
            {
                //qDebug()<<"date est "<< QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd");

                if(QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd")==firstDayOfMonth || verif>0)
                {



                    if(verif==0)verif=QDate::fromString(tablewidget->item(k,7)->text().split("T").first(), "yyyy-MM-dd").daysTo(QDate::fromString(tablewidget->item(k,8)->text().split("T").first(), "yyyy-MM-dd"))+1;


                    QLabel *label2 = new QLabel(firstDayOfMonth.toString("dd"));

                    QDate date=firstDayOfMonth;
                    Evenement ev;
                    query = ev.getEvenementByDate(date);
                    while(!query.next()){date=date.addDays(-1);query = ev.getEvenementByDate(date);}
                    QLabel *label = new QLabel(query.value("TITRE").toString());
                    label->setAlignment(Qt::AlignCenter); // Centrer le texte dans le QLabel
                    label2->setAlignment(Qt::AlignCenter);
                    // Taille plus petite que la cellule
                    label->setFixedSize(140, 30);  // Change selon ton besoin
                    label2->setFixedSize(70, 30);
                    // Conteneur pour centrer le label sans changer la cellule
                    QWidget *container = new QWidget();

                    // Créer un QVBoxLayout (vertical)
                    QVBoxLayout *layout = new QVBoxLayout(container);

                    // Ajoute un espace flexible pour pousser le label vers le bas


                    layout->addStretch();
                    layout->addWidget(label2); // Espace flexible avant le label
                    layout->addWidget(label);       // Le label sera en bas
                        // L'espace flexible après le label

                    // Réinitialiser les marges du layout pour éviter l'espace inutile
                    layout->setContentsMargins(0, 0, 0, 0);

                    container->setLayout(layout);

                    // Appliquer le style complet au container
                    container->setStyleSheet(R"(
    background-color: rgba(255, 182, 193, 0.3);  /* Rose bébé transparent */
    border: none;                               /* Aucune bordure */
    border-radius: 12px;                        /* Coins arrondis */
    padding: 8px 12px;                          /* Espace intérieur */
    color: #4a3c3c;                             /* Texte doux */
)");



                    label->setStyleSheet(
                        "background-color: rgba(173, 216, 230, 0.3); margin-right:10px;border: none; "  // Couleur de fond

                        );
                    label2->setStyleSheet(
                        "border: none;"
                        "background-color :transparent; "  // Couleur de fond

                        );
                    // Insérer le widget dans la cellule
                    ui->calendrier_2->setCellWidget(i, j, container);
                }
            }

            firstDayOfMonth=firstDayOfMonth.addDays(1);
        }}}


void MainWindow::on_todo_ok_clicked()
{
    ui->write_todo->setStyleSheet(R"(
    QTextEdit {
        background-color: #f4f6f7;
        border: 1px solid #c2cfd6;
        border-radius: 8px;
        padding: 8px;
        font-family: 'Segoe UI', sans-serif;
        font-size: 14px;
        color: #1c1c1c;
    }
    QTextEdit:focus {
        border: 1px solid #0078d4;
        background-color: #ffffff;
    }
)");

    QString taskText = ui->write_todo->toPlainText().trimmed();
    if (taskText.isEmpty()) return;

    // ✅ Vraie checkbox cliquable
    QStandardItem *checkItem = new QStandardItem();
    checkItem->setCheckable(true);
    checkItem->setCheckState(Qt::Unchecked);
    checkItem->setEditable(false);

    // 📝 Texte de la tâche
    QStandardItem *taskItem = new QStandardItem(taskText);
    taskItem->setEditable(false);

    // 🔴 Statut : Pas encore fait (rouge)
    QStandardItem *statusItem = new QStandardItem("Pas encore fait");
    statusItem->setEditable(false);
    statusItem->setForeground(QBrush(QColor("#c62828")));  // rouge

    QList<QStandardItem*> rowItems;
    rowItems << checkItem << taskItem << statusItem;
    todoModel->appendRow(rowItems);

    ui->write_todo->clear();
}

void MainWindow::on_table_todo_clicked(const QModelIndex &index)
{
    if (index.column() == 0) {
        QStandardItem *checkItem = todoModel->item(index.row(), 0);
        QStandardItem *statusItem = todoModel->item(index.row(), 2);

        bool isChecked = (checkItem->checkState() == Qt::Checked);

        if (isChecked) {
            checkItem->setCheckState(Qt::Unchecked);
            statusItem->setText("Pas encore fait");
            statusItem->setForeground(QBrush(QColor("#c62828")));  // rouge
        } else {
            checkItem->setCheckState(Qt::Checked);
            statusItem->setText("Fait");
            statusItem->setForeground(QBrush(QColor("#2e7d32")));  // vert
        }
    }
}


void MainWindow::on_btnVoix_ev_clicked()
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

    QString scriptPath = "C:/Users/TIR ELLIL/Documents/event/voice_recognizer.py";
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
        ui->lineEditQuestion->setText(finalOutput);
    } else {
        ui->lineEditQuestion->setPlaceholderText("Aucune voix détectée");
    }

    process->deleteLater();
    process = nullptr;
    bufferOutput.clear();
}




void MainWindow::on_ticket_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un événement.");
        return;
    }

    int row = currentIndex.row();
    QAbstractItemModel* model = ui->tableView->model();

    QString id = model->index(row, 0).data().toString(); // colonne 0 = ID

    QSqlQuery query;
    query.prepare("SELECT TITRE, DATE_DEB, DATE_FIN, PRIX, ID_ESPACE FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString titre = query.value(0).toString();
        QString date_deb = query.value(1).toString();
        QString date_fin = query.value(2).toString();
        QString prix = query.value(3).toString();
        QString id_espace = query.value(4).toString();

        QString scriptPath = "python";
        QString scriptFile = "C:/Users/TIR ELLIL/Documents/event/generate_ticket.py";

        QStringList arguments;
        arguments << scriptFile << titre << date_deb << date_fin << prix << id_espace;

        QProcess *process = new QProcess(this);
        process->start(scriptPath, arguments);

        connect(process, &QProcess::readyReadStandardOutput, [process]() {
            QString output = process->readAllStandardOutput();
            qDebug() << "Ticket généré:\n" << output;
        });

        connect(process, &QProcess::readyReadStandardError, [process]() {
            qDebug() << "Erreur Python:" << process->readAllStandardError();
        });

    } else {
        qDebug() << "Événement non trouvé";
    }
}




void MainWindow::on_voiraffiche_clicked()
{
    // Obtenir l'index de la ligne sélectionnée dans le QTableView
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un événement dans la table.");
        return;
    }

    int selectedRow = selectedRows.first().row();

    // Récupérer les données de la ligne sélectionnée - CORRECTION: colonne 0 pour le TITRE
    QString titre = ui->tableView->model()->data(ui->tableView->model()->index(selectedRow, 1)).toString(); // colonne 1 : TITRE

    qDebug() << "Titre sélectionné : " << titre;

    // Le reste du code reste inchangé...
    QSqlQuery query;
    query.prepare("SELECT ID, AFFICHE FROM EVENEMENT WHERE TITRE = :titre");
    query.bindValue(":titre", titre);

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer l'affiche depuis la base.");
        return;
    }

    if (!query.next()) {
        qDebug() << "Aucun événement trouvé avec ce titre.";
        QMessageBox::critical(this, "Erreur", "Aucun événement trouvé avec ce titre.");
        return;
    }

    int eventId = query.value(0).toInt();
    QByteArray imageData = query.value(1).toByteArray();

    qDebug() << "Taille de l'affiche récupérée : " << imageData.size();

    if (imageData.isEmpty()) {
        QMessageBox::information(this, "Aucune affiche", "Cet événement n'a pas d'affiche.");
        return;
    }

    QPixmap pixmap;
    if (!pixmap.loadFromData(imageData)) {
        qDebug() << "Erreur de chargement de l'image à partir des données.";
        QMessageBox::critical(this, "Erreur", "Impossible de charger l'affiche depuis les données.");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Aperçu de l'affiche");
    dialog->setMinimumSize(500, 600);
    dialog->setStyleSheet("background-color: #fdfdfd; border: 1px solid #ccc; border-radius: 10px;");

    // Titre de l’événement
    QLabel *titreLabel = new QLabel(titre, dialog);
    titreLabel->setAlignment(Qt::AlignCenter);
    titreLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333; margin-bottom: 15px;");

    // Affiche
    QLabel *imageLabel = new QLabel(dialog);
    imageLabel->setPixmap(pixmap.scaled(450, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: 1px solid #ddd;");

    // Bouton Fermer
    QPushButton *closeButton = new QPushButton("Fermer", dialog);
    closeButton->setFixedWidth(100);
    closeButton->setStyleSheet("padding: 8px 16px; font-size: 14px;");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    // Layout principal
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(titreLabel);
    layout->addWidget(imageLabel);
    layout->addSpacing(10);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    dialog->setLayout(layout);
    dialog->exec();

}






