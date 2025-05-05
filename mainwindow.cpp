#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maintenance.h"
#include "openaihelper.h"
#include "mailjetsender.h"
#include "service.h"
#include "mailsender.h"
#include "chatmanager.h"
#include "chatwindow.h"
#include "employe.h"
#include "event.h"
#include "espace.h"
#include "dimensionsdialog.h"
#include "espace3dview.h"
#include "clickablerectitem.h"
#include "locataire.h"
#include "dialog.h"

// Qt Core
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QFuture>
#include <QProcess>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QShortcut>
#include <QTimer>
#include <QTimerEvent>
#include <QByteArray>
#include <QRect>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QInputDialog>

// Qt GUI
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QFontDatabase>
#include <QDesktopServices>
#include <QUrl>
#include <QBuffer>
#include <QCheckBox>
#include <QCompleter>
#include <QFileDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QTabBar>
#include <QTableView>
#include <QTableWidget>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTextStream>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSplitter>

// SQL
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

// Charts
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// PDF
#include <QPdfWriter>
#include <QPrinter>

// Camera / Media
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QImageCapture>

// Network / JSON
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

// WebSocket
#include <QWebSocketServer>
#include <QWebSocket>

#include <QGraphicsProxyWidget>
#include <QGraphicsOpacityEffect>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    arduino(new Arduino(this)),
    dernierIdEmploye(-1)
{
    ui->setupUi(this); // D'abord configurer l'UI







    //__________________yassmine____________________







    // Connexion des signaux Arduino
    connect(arduino, &Arduino::empreinteEnregistree,
            this, &MainWindow::onEmpreinteEnregistree);

    connect(arduino, &Arduino::messageStatut,
            this, [this](const QString &msg){
                ui->statusbar->showMessage(msg, 3000); // attention à "statusbar" vs "statusBar"
            });

    // Connexion automatique
    if (!arduino->connecter()) {
        QTimer::singleShot(3000, this, [this]() {
            arduino->connecter();
        });
    }

    afficherEmployes();


    // Connexion des signaux
    connect(arduino, &Arduino::empreinteEnregistree, this, &MainWindow::onEmpreinteEnregistree);
    // Exemple d’init si arduino déjà instancié :
    bool ok = connect(arduino, &Arduino::empreinteReconnue,
                      this,  &MainWindow::slotEmpreinteReconnue);
    qDebug() << "[CONNECT empreinteReconnue/MainWindow] Connect status: " << ok;    connect(arduino, &Arduino::empreinteNonTrouvee, this, &MainWindow::slotEmpreinteNonTrouvee);
    // Tu peux aussi connecter messageStatut() ou erreurCapteur() à un QLabel ou statusBar



    connect(ui->nom, &QLineEdit::textChanged, this, [=](){
        suggestEmailsByIA(ui->prenom->text(), ui->nom->text());
    });
    connect(ui->prenom, &QLineEdit::textChanged, this, [=](){
        suggestEmailsByIA(ui->prenom->text(), ui->nom->text());
    });

    //connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherEmployes);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::trierEmployes);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    //connect(ui->pushButton_faceId, &QPushButton::clicked, this, &MainWindow::on_pushButton_faceId_clicked);
    connect(ui->pushButton_appliquerModif, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_appliquerModif_clicked);
    connect(ui->searchText, &QLineEdit::textChanged, this, &MainWindow::rechercherEmployes);
    ui->searchText->setPlaceholderText("🔎 Rechercher ...");
        ui->tabWidget->tabBar()->hide();

    connect(ui->liste, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(1);
        afficherEmployes();

        qDebug() << "Passage à l'onglet Liste";
    });

    connect(ui->ajout, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Ajout";
    });
    connect(ui->login, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(4);
        qDebug() << "Retour à l'onglet login";
    });
    connect(ui->pushButton_8, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(5);
        chargementTodoList(); //  Appelle cette fonction ici
    });
    connect(ui->pushButton_ajouter, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(6);
    });

    connect(ui->stat, &QPushButton::clicked, this, [=]() {
        ui->tabWidget->setCurrentIndex(2);
        afficherStatistiques();
    });

    connect(ui->pushButton_modifier, &QPushButton::clicked, this, [=]() {
        QModelIndex index = ui->tableView->currentIndex();

        if (!index.isValid()) {
            QMessageBox::warning(this, "Attention", "Veuillez d'abord sélectionner un employé.");
            return;
        }

        int row = index.row();
        auto model = ui->tableView->model();

        Employe e;

        e.setId(model->index(row, 1).data().toInt());
        e.setNom(model->index(row, 2).data().toString());
        e.setPrenom(model->index(row, 3).data().toString());

        QDate d = model->index(row, 4).data().toDate();
        e.setDateEmbauche(d.isValid() ? d : QDate::fromString(model->index(row, 4).data().toString(), "yyyy-MM-dd"));

        e.setPoste(model->index(row, 5).data().toString());
        e.setSalaire(model->index(row, 6).data().toDouble());
        e.setEmail(model->index(row, 7).data().toString());
        e.setSexe(model->index(row, 8).data().toString());
        e.setTelephone(model->index(row, 9).data().toString());
        e.setStatut(model->index(row, 10).data().toString());

        QStandardItemModel* m = qobject_cast<QStandardItemModel*>(model);
        if (m) {
            QStandardItem* photoItem = m->item(row, 0);
            if (photoItem) {
                QVariantMap datas = photoItem->data(Qt::UserRole + 1).toMap();
                e.setPdp(datas.value("pdp").toByteArray());
            }
        }

        setEmploye(e);

        ui->tabWidget->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Modification pour l'employé ID:" << e.getId();
    });






    //____________________youssef___________________





    try {
        // Démarrer le serveur de chat sur le port 1234
        m_chatManager = new ChatManager(1234, this);

        // Connecter le bouton "Ouvrir le chat"
        connect(ui->chatButton, &QPushButton::clicked, this, &MainWindow::openChatWindow);
        //connect(ui->chatButton_2, &QPushButton::clicked, this, &MainWindow::openChatWindow);
        //connect(ui->chatButton_3, &QPushButton::clicked, this, &MainWindow::openChatWindow);
        //connect(ui->chatButton_4, &QPushButton::clicked, this, &MainWindow::openChatWindow);

    } catch (const ChatException& e) {
        QMessageBox::critical(this, "Erreur",
                              QString("Impossible de démarrer le serveur de chat: %1").arg(e.what()));
    }

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(shortcut, &QShortcut::activated, ui->sendButton_srv, &QPushButton::click);
    shortcut->setContext(Qt::WidgetWithChildrenShortcut);  // Limite au widget courant et ses enfants

    // Cacher la barre des onglets pour forcer l'utilisation des boutons
    ui->tabWidget_srv->tabBar()->hide();
    connect(ui->stat_srv, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesServ);
    connect(ui->liste_srv, &QPushButton::clicked, this, &MainWindow::afficherServices);
    connect(ui->liste_srv, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->form_srv, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->stat_srv, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->save_stat, &QPushButton::clicked, this, &MainWindow::changerCouleurBouton);
    connect(ui->btnTrierServices, &QPushButton::clicked, this, &MainWindow::trierServices);
    connect(ui->recherche_srv, &QLineEdit::textChanged, this, &MainWindow::rechercherService);
    connect(ui->sendButton_srv, &QPushButton::clicked, this, &MainWindow::envoyerRequete);
    connect(ui->btnGenererDescription, &QPushButton::clicked, this, &MainWindow::genererDescriptionIA);
    connect(ui->lineEdit_id_join, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_id_textChanged);
    connect(ui->lineEdit_id_join_mod, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_id_textChanged2);



    openAIClient = new OpenAIClient();


    ui->recherche_srv->setPlaceholderText("🔎 Rechercher un service...");

    connect(ui->tabWidget_srv, &QTabWidget::currentChanged,
            this, &MainWindow::updateChatSenderName);

    //setUpNavigationButtons();
    connect(openAIClient, &OpenAIClient::descriptionGeneree, this, &MainWindow::afficherDescription);

    connect(ui->liste_srv, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_srv->setCurrentIndex(0);
        qDebug() << "Passage à l'onglet Ajout";

    });

    connect(ui->form_srv, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_srv->setCurrentIndex(1);
        qDebug() << "Retour à l'onglet Affichage";
    });

    connect(ui->stat_srv, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_srv->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
    });



    //_________________________MALEK_________________________



    ui->searchText_2->setPlaceholderText("🔎 Rechercher ...");
        todoModelEvent = new QStandardItemModel(this);
    todoModelEvent->setColumnCount(3);
    todoModelEvent->setHorizontalHeaderLabels(QStringList() << "✔" << "Tâche" << "Statut");

    ui->table_todo_ev->setModel(todoModel);
    ui->table_todo_ev->horizontalHeader()->setStretchLastSection(true);
    ui->table_todo_ev->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_todo_ev->setAlternatingRowColors(true);
    ui->table_todo_ev->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_todo_ev->verticalHeader()->setVisible(false);
    ui->table_todo_ev->horizontalHeader()->setStretchLastSection(true);
    ui->table_todo_ev->setStyleSheet(R"(
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



    tablewidget=new QTableWidget();
    model = new QSqlTableModel(this);
    model->setTable("evenements"); // Remplace "evenements" par le nom correct de ta table
    model->select();
    ui->tableView_ev->setModel(model);

    //connect(ui->pushButton_choisirImage, &QPushButton::clicked, this, &MainWindow::on_pushButton_choisirImage_clicked);
    connect(ui->liste_ev, &QPushButton::clicked, this, &MainWindow::afficherEvenement);
    connect(ui->stat_ev, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesEvent);
    connect(ui->btn_trierDate_ev, &QPushButton::clicked, this, &MainWindow::on_btn_trierDate_clicked);
    connect(ui->searchText_2, &QLineEdit::textChanged, this, &MainWindow::rechercherevenement);
    connect(ui->table_todo_ev, &QTableView::clicked, this, &MainWindow::on_table_todo_ev_clicked);


    // Initialiser le modèle pour afficher les événements dans le QListView
    eventModel = new QStringListModel(this);// listViewEvents doit être l'objet QListView dans ton UI




    // Cacher la barre des onglets pour forcer l'utilisateur à utiliser les boutons
    ui->tabWidget_event->tabBar()->hide();

    // Connexion des boutons pour changer d'onglet
    connect(ui->liste_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_event->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->ajout_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_event->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Affichage";
    });

    connect(ui->stat_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_event->setCurrentIndex(2);
        qDebug() << "Passage à l'onglet Statistiques";
    });
    connect(ui->chatbot, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_event->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Statistiques";
    });
    connect(ui->todo_ev, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_event->setCurrentIndex(6);
        qDebug() << "Passage à l'onglet TODO";
    });

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
        ui->tableView_ev->setModel(model);}

    copyTableViewToTableWidget(ui->tableView_ev, tablewidget);
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









    //_________________________Ahmed______________________











    // Afficher les données de maintenance au démarrage
    Maintenance m;
    ui->tab_maintenance->setModel(m.afficher());


    ui->tabWidget_3->tabBar()->hide();






    //ajout
    connect(ui->description_maintenance, &QTextEdit::textChanged, this, [=]() {
        bool valide = !ui->description_maintenance->toPlainText().isEmpty();
        validerChamp2(ui->description_maintenance, ui->er_description, valide ? "✔️ Description valide" : "La description est vide.", valide);
    });


    connect(ui->cout_maintenance, &QLineEdit::textChanged, this, [=]() {
        bool ok;
        ui->cout_maintenance->text().toDouble(&ok);
        validerChamp(ui->cout_maintenance, ui->er_cout, ok ? "✔️ Montant valide" : "Le coût est invalide.", ok);
    });

    connect(ui->id_employe_maintenance, &QLineEdit::textChanged, this, [=]() {
        bool ok;
        int id = ui->id_employe_maintenance->text().toInt(&ok);
        bool existe = ok && Maintenance::employeeExiste(id);
        validerChamp(ui->id_employe_maintenance, ui->er_id_employee, existe ? "✔️ Employé existe" : "ID employé invalide ou inexistant.", existe);
    });

    connect(ui->id_espace_maintenance, &QLineEdit::textChanged, this, [=]() {
        bool ok;
        int id = ui->id_espace_maintenance->text().toInt(&ok);
        bool existe = ok && Maintenance::espaceExiste(id);
        validerChamp(ui->id_espace_maintenance, ui->er_id_espace, existe ? "✔️ Espace existe" : "ID espace invalide ou inexistant.", existe);
    });



    connect(ui->liste_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(1);
        qDebug() << "Passage à l'onglet Ajout";
    });

    connect(ui->ajout_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(0);
        qDebug() << "Retour à l'onglet Affichage";
    });


    connect(ui->stat_maintenance, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_3->setCurrentIndex(3);
        qDebug() << "Passage à l'onglet Statistiques";
    });

    connect(ui->rech_maintenance, &QLineEdit::textChanged, this, &MainWindow::rechercherMaintenance);


    connect(ui->tab_maintenance, &QTableView::clicked, this, &MainWindow::afficherDescriptionComplete);

    connect(ui->tab_maintenance, &QTableView::doubleClicked, this, &MainWindow::afficherDetailsEmployeMain);
    connect(ui->tab_maintenance, &QTableView::clicked, this, &MainWindow::handleTableClick);

    //tri
    connect(ui->tab_maintenance->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int col){
        ui->tab_maintenance->selectColumn(col); // facultatif : met en évidence
        trierMaintenance();
    });

    //stat
    connect(ui->stat_maintenance, &QPushButton::clicked, this, &MainWindow::on_stat_maintenance_clicked);



    //analyse
    OpenAIHelper *openai = new OpenAIHelper(this);

    // Connecter le signal à une fonction pour traiter la réponse
    connect(openai, &OpenAIHelper::analysisReady, this, [=](const QString &result) {
        // 1. Afficher dans un PDF
        // 2. Sauvegarder dans la base
        // 3. Afficher un message
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer l'analyse", "", "PDF Files (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            QPainter painter(&writer);
            painter.setFont(QFont("Arial", 12));
            painter.drawText(QRect(100, 100, writer.width() - 200, writer.height() - 200), Qt::TextWordWrap, result);
            painter.end();
        }


    });


    //arduino
    /*player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile("C:/alerte.wav"));
    audioOutput->setVolume(100);  // 100% volume
    arduino = new Arduino(this);
    if (arduino->connecter()) {
        connect(arduino, &Arduino::gazDetecte, this, [=]() {
            QString description = "Détection de gaz potentiellement toxique dans l’espace technique.";


            OpenAIHelper *openai = new OpenAIHelper(this);
            connect(openai, &OpenAIHelper::analysisReady, this, [=](const QString &analyseResult) {
                QString analyseNettoyee = normaliserTexte(analyseResult);

                static const QRegularExpression regex("\\b(electricien|plombier|technicien|informaticien|menuisier|nettoyeur)\\b");
                QRegularExpressionMatch match = regex.match(analyseNettoyee);

                if (!match.hasMatch()) {
                    qDebug() << "⚠️ Aucun métier détecté dans l'analyse IA.";
                    return;
                }

                QString metier = match.captured(1);
                qDebug() << "✅ Métier détecté :" << metier;

                QMap<QString, QString> employeData = Maintenance::getEmployeInactifByPoste(metier);
                if (employeData.isEmpty()) {
                    qDebug() << "⚠️ Aucun employé inactif trouvé pour :" << metier;
                    return;
                }

                int id_employe = employeData["id"].toInt();
                QString employeNom = employeData["nom"] + " " + employeData["prenom"];
                QString employeEmail = employeData["email"];

                Maintenance::updateEmployeStatut(id_employe, "actif");

                Maintenance maintenance("Instantané", description, 0.0, "Non traité", QDate::currentDate().toString("yyyy-MM-dd"), id_employe, 123);
                maintenance.setAnalyse(analyseResult);

                if (maintenance.ajouter()) {
                    qDebug() << "✅ Maintenance insérée pour l'employé " << employeNom;

                    // ✉️ Mail
                    MailJetSender *sender = new MailJetSender();
                    sender->envoyerMail("SMM", "ayadia819@gmail.com", employeEmail,
                                        "Nouvelle Maintenance Urgente",
                                        "Bonjour " + employeNom + ",\nUne fuite de gaz a été détectée. Veuillez intervenir rapidement.\n\n" +
                                            "Analyse IA :\n" + analyseResult);

                    ui->tabWidget_3->setCurrentIndex(1);
                    afficher_maintenance();
                    QMessageBox::warning(this, "🚨 Alerte Gaz", "Fuite de gaz détectée ! Analyse en cours...");
                    QMessageBox::information(this, "Maintenance ajoutée",
                                             "✅ Maintenance urgente ajoutée et affectée à :\n" +
                                                 employeNom + "\n📧 Email : " + employeEmail);

                } else {
                    qDebug() << "❌ Échec d'ajout de la maintenance.";
                }

            });

            openai->analyserMaintenance(description);
        });


        connect(arduino, &Arduino::donneesRecues, this, [](const QString &msg) {
            qDebug() << "🧪 Arduino a envoyé :" << msg;
        });



        connect(arduino, &Arduino::gazDetecte, this, [=]() {
            this->setStyleSheet("background-color: red;");
            player->play();  // 🔥 Jouer le son
            ui->btn_stop_alerte->setEnabled(true);
        });

    }

    connect(ui->btn_stop_alerte, &QPushButton::clicked, this, [=]() {
        bool ok;
        QString code = QInputDialog::getText(this, "Code d'arrêt",
                                             "Entrez le code pour arrêter l'alerte :",
                                             QLineEdit::Password,
                                             "", &ok);
        if (ok && code == "1234") {  // ✅ Bon code
            player->stop();          // 🛑 Stopper le son
            this->setStyleSheet(""); // Fond normal
            arduino->envoyerCommande("STOP");  // Envoyer stop vers Arduino
            ui->btn_stop_alerte->setEnabled(false);
            QMessageBox::information(this, "Succès", "✅ Alerte arrêtée !");
        } else {
            QMessageBox::critical(this, "Erreur", "❌ Code incorrect !");
        }
    });*/










    //_____________insaf______________



    networkManager = new QNetworkAccessManager(this);

    // Initialize the QCompleter
    completer = new QCompleter(this);
    completer = new QCompleter(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_Nom_espace->setCompleter(completer);
    // Connect UI actions (buttons)
    ui->tabWidget_espace->tabBar()->hide();

    connect(ui->liste_espace, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_espace->setCurrentIndex(1);
           afficherEspaces();
    });
    connect(ui->ajout_espace, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_espace->setCurrentIndex(0);
    });
    connect(ui->stat_espace, &QPushButton::clicked, this, [=]() {
        ui->tabWidget_espace->setCurrentIndex(2);
        on_stat_2_clicked();
    });
  connect(ui->searchEspace, &QLineEdit::textChanged, this, &MainWindow::rechercherEspaces);
    ui->searchEspace->setPlaceholderText("🔎 Rechercher ...");
    connect(ui->map, &QPushButton::clicked, this, &MainWindow::on_map_clicked);
    connect(ui->lineEdit_id_locataire_espace, &QLineEdit::editingFinished, this, &MainWindow::on_id_locataire_entered);
    connect(ui->lineEdit_id_locataire_2_espace, &QLineEdit::editingFinished,
            this, &MainWindow::on_lineEdit_id_locataire_2_espace_editingFinished);
    afficherEspaces();


    //____________________HAMZA_______________




    afficherLocataires();
        // ✅ Appliquer une police moderne globalement
        QFont defaultFont("Segoe UI", 10);  // ou "Arial", "Calibri"...
        QApplication::setFont(defaultFont);

        // Connexions des boutons
        connect(ui->tri_loc, &QPushButton::clicked, this, &MainWindow::trierlocataire);
        connect(ui->pdf_loc, &QPushButton::clicked, this, &MainWindow::exporterPDF_locataire);
        connect(ui->chmprech, &QLineEdit::textChanged, this, &MainWindow::rechercherLocataire);
        connect(ui->upload, &QPushButton::clicked, this, &MainWindow::on_upload_clicked);
        connect(ui->btnAfficherStatistiques, &QPushButton::clicked, this, &MainWindow::afficherStatistiques_locataire);
        connect(ui->btnGenererContrat, &QPushButton::clicked, this, &MainWindow::genererContratDepuisOpenAI);

        connect(ui->email_locataire, &QLineEdit::textChanged, this, &MainWindow::verifierEmail);

        // Connexion avec lambda pour extraction OpenAI
        connect(ui->pushButton_extraire, &QPushButton::clicked, this, [this]() {
            if (cheminContrat.isEmpty()) {
                QMessageBox::warning(this, "Erreur", "Aucun contrat sélectionné !");
                return;
            }
            envoyerImageAOpenAI(cheminContrat);
        });

        // Onglet masqué et navigation entre les onglets
        ui->tabWidget_loc->tabBar()->hide();

        connect(ui->liste_loc, &QPushButton::clicked, this, [=]() {
            ui->tabWidget_loc->setCurrentIndex(1);
            qDebug() << "Passage à l'onglet Liste";
            afficherLocataires();
        });

        connect(ui->ajout_loc, &QPushButton::clicked, this, [=]() {
            ui->tabWidget_loc->setCurrentIndex(0);
            qDebug() << "Retour à l'onglet Ajout";
        });

        connect(ui->stat_loc, &QPushButton::clicked, this, [=]() {
            ui->tabWidget_loc->setCurrentIndex(2);
            qDebug() << "Passage à l'onglet Statistiques";
            afficherStatistiques_locataire();
        });

        // Optionnel : afficher les locataires directement au démarrage
        afficherLocataires();
        ui->chmprech->setPlaceholderText("🔎 Rechercher un locataire...");





}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_chatManager; // <-- Libération de la mémoire

}





//________________yassmine_______________








void MainWindow::afficherEmployes()
{
    disconnect(ui->tableView, &QTableView::clicked, this, &MainWindow::onPhotoClicked);

    QSqlQuery query("SELECT pdp, id, nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, statut FROM employe");

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "Photo", "ID", "Nom", "Prénom", "Date embauche",
        "Poste", "Salaire", "Email", "Sexe", "Téléphone", "Statut"
    });

    while (query.next()) {
        QList<QStandardItem*> rowItems;

        QStandardItem *photoItem = new QStandardItem();
        QByteArray imageData = query.value("pdp").toByteArray();

        qDebug() << "🔄 Chargement image : Taille =" << imageData.size();

        QVariantMap empData;
        empData["pdp"] = imageData;
        empData["id"] = query.value("id");
        empData["nom"] = query.value("nom");
        empData["prenom"] = query.value("prenom");
        empData["date_embauche"] = query.value("date_embauche");
        empData["poste"] = query.value("poste");
        empData["salaire"] = query.value("salaire");
        empData["email"] = query.value("email");
        empData["sexe"] = query.value("sexe");
        empData["telephone"] = query.value("telephone");
        empData["statut"] = query.value("statut");

        photoItem->setData(empData, Qt::UserRole + 1);

        if (!imageData.isEmpty()) {
            QPixmap pix;
            if (pix.loadFromData(imageData)) {
                pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QPixmap rounded = createRoundedPixmap(pix, 48);
                photoItem->setIcon(QIcon(rounded));
            } else {
                qDebug() << "⚠ Erreur : Impossible de charger l'image.";
            }
        } else {
            QPixmap def(":/img/default-user.png");
            if (!def.isNull()) {
                QPixmap rounded = createRoundedPixmap(def, 48);
                photoItem->setIcon(QIcon(rounded));
            }
        }

        photoItem->setText("");
        rowItems << photoItem;

        for (int i = 1; i < query.record().count(); ++i) {
            rowItems << new QStandardItem(query.value(i).toString());
        }

        model->appendRow(rowItems);
    }

    // 🔁 Appliquer modèle et design
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setIconSize(QSize(64, 64));

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

        qDebug() << "✅ Liste des employés chargée avec succès.";
    } else {
        qDebug() << "❌ Erreur lors du chargement des employés.";
    }

    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::onPhotoClicked);
}

void MainWindow::on_pushButton_analysePDP_clicked() {
    static QPixmap originalImage;
    static QString fileName;

    // Récupérer l'image du bouton pushButton_pdp
    QIcon icon = ui->pushButton_pdp->icon();
    QPixmap pixmap = icon.pixmap(200, 200);

    if (pixmap.isNull()) {
        // Si aucune image n'est trouvée sur le bouton, ouvrir l'explorateur de fichiers
        fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une photo"), "", tr("Images (*.png *.jpg *.jpeg)"));
        if (fileName.isEmpty()) return;
        originalImage = QPixmap(fileName);
    } else {
        originalImage = pixmap;
    }

    if (originalImage.isNull()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image"));
        return;
    }

    QPixmap scaledImage = originalImage.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pushButton_pdp->setIcon(scaledImage);
    ui->pushButton_pdp2->setIcon(scaledImage);

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    originalImage.save(&buffer, "PNG");
    QString base64Image = imageData.toBase64();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://api.clarifai.com/v2/models/face-detection/outputs"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Key " + CLARIFAI_API_KEY.toUtf8());

    QJsonObject inputs;
    inputs["data"] = QJsonObject{{"image", QJsonObject{{"base64", base64Image}}}};
    QJsonObject requestBody;
    requestBody["inputs"] = QJsonArray{inputs};

    QJsonDocument doc(requestBody);
    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->post(request, data);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Analyse de l'image");
    dialog->setMinimumWidth(600);
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *imageLabel = new QLabel(dialog);
    imageLabel->setPixmap(originalImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout->addWidget(imageLabel);

    QLabel *analysisLabel = new QLabel("Analyse en cours...", dialog);
    layout->addWidget(analysisLabel);

    QPushButton *selectButton = new QPushButton("Utiliser cette image", dialog);
    QPushButton *chooseAnotherButton = new QPushButton("Choisir une autre image", dialog);
    connect(selectButton, &QPushButton::clicked, [=]() {
        ui->pushButton_pdp->setIcon(scaledImage);
        ui->pushButton_pdp2->setIcon(scaledImage);
        dialog->accept();
    });
    connect(chooseAnotherButton, &QPushButton::clicked, [=]() {
        dialog->reject();
        QString newFileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une photo"), "", tr("Images (*.png *.jpg *.jpeg)"));
        if (!newFileName.isEmpty()) {
            fileName = newFileName;
            // Mettre à jour le bouton avec la nouvelle image avant de relancer l'analyse
            originalImage = QPixmap(fileName);
            QPixmap scaledImage = originalImage.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->pushButton_pdp->setIcon(scaledImage);
            ui->pushButton_pdp2->setIcon(scaledImage);
            on_pushButton_analysePDP_clicked(); // Relancer l'analyse
        }
    });
    layout->addWidget(selectButton);
    layout->addWidget(chooseAnotherButton);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            QJsonArray outputs = response.object()["outputs"].toArray();
            if (!outputs.isEmpty()) {
                QJsonArray regions = outputs[0].toObject()["data"].toObject()["regions"].toArray();
                int faceCount = regions.size();

                if (faceCount > 0) {
                    QPixmap tempImage = originalImage;
                    QPainter painter(&tempImage);
                    painter.setPen(QPen(Qt::red, 3));

                    for (const auto &region : regions) {
                        QJsonObject box = region.toObject()["region_info"].toObject()["bounding_box"].toObject();
                        double top = box["top_row"].toDouble() * tempImage.height();
                        double left = box["left_col"].toDouble() * tempImage.width();
                        double bottom = box["bottom_row"].toDouble() * tempImage.height();
                        double right = box["right_col"].toDouble() * tempImage.width();
                        QRect rect(QPoint(left, top), QPoint(right, bottom));
                        painter.drawRect(rect);
                    }
                    painter.end();
                    imageLabel->setPixmap(tempImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }

                if (faceCount == 1) {
                    analysisLabel->setText("✅ Un seul visage détecté. Photo validée.");
                } else if (faceCount > 1) {
                    analysisLabel->setText("⚠️ Plusieurs visages détectés. Photo non validée.");
                } else {
                    analysisLabel->setText("❌ Aucun visage détecté. Photo non validée.");
                }
            } else {
                analysisLabel->setText("❌ Erreur d'analyse Clarifai.");
            }
        } else {
            analysisLabel->setText("❌ Erreur réseau : " + reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });

    dialog->exec();
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    qDebug() << "Tentative d'ajout d'un employé...";

    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QDate date_embauche = ui->date_embauche->date();
    QString poste = ui->poste->currentText();
    QString email = ui->email->text();
    QString sexe = ui->sexe->currentText();
    QString telephone = ui->telephone->text();
    QString statut = ui->statut->currentText();

    // Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis.");
        return;
    }

    bool ok;
    int salaire = ui->salaire->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Erreur", "Le salaire doit être un nombre valide.");
        return;
    }

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

    // Si c'est un chef technicien, vérifier le FaceID
    if (poste == "chef technicien" && faceIdTemp.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "L'enregistrement du FaceID est obligatoire pour un chef technicien.");
        return;
    }

    // Création de l'objet employé avec face_id et empreinte
    Employe employe(nom, prenom, date_embauche, poste, salaire, email, sexe, telephone, imageData, statut, faceIdTemp, empreinteTemp);

    // Tentative d'ajout de l'employé
    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");

        // Clear tous les champs
        ui->nom->clear();
        ui->prenom->clear();
        ui->date_embauche->setDate(QDate::currentDate());
        ui->poste->setCurrentIndex(0);
        ui->email->clear();
        ui->sexe->setCurrentIndex(0);
        ui->telephone->clear();
        ui->statut->setCurrentIndex(0);
        ui->salaire->clear();

        // Réinitialiser les boutons de photo de profil
        ui->pushButton_pdp->setIcon(QIcon());  // Enlever l'icône
        ui->pushButton_pdp2->setIcon(QIcon()); // Enlever l'icône
        ui->pushButton_pdp->setStyleSheet("");  // Réinitialiser le style
        ui->pushButton_pdp2->setStyleSheet(""); // Réinitialiser le style

        // Réinitialiser le chemin de l'image
        cheminImagePDP.clear();

        // Réinitialiser le Face ID temporaire
        faceIdTemp.clear();

        // Rafraîchir le tableau des employés
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
    }
}
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

void MainWindow::on_pushButton_supprimer_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int id = selection.first().siblingAtColumn(1).data(Qt::DisplayRole).toInt();
    qDebug() << "ID sélectionné :" << id;

    if (id <= 0) {
        QMessageBox::critical(this, "Erreur", "ID invalide (" + QString::number(id) + ") !");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Êtes-vous sûr de vouloir supprimer cet employé ?",
                                                              QMessageBox::Yes | QMessageBox::No);
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

void MainWindow::on_pushButton_exporterPDF_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "liste_employes.pdf", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(120);
    writer.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    QAbstractItemModel *model = ui->tableView->model();
    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    int y = 100;
    int x = 50;
    const int rowHeight = 200;
    const int photoSize = 100;

    // 🔤 Normalisation des en-têtes
    auto normalize = [](const QString &text) -> QString {
        QRegularExpression re("[\\x{0300}-\\x{036F}]");
        QString cleaned = text.normalized(QString::NormalizationForm_D);
        return cleaned.remove(re).toLower().simplified();
    };

    // 📦 Détection des colonnes
    QMap<QString, int> cols;
    for (int c = 0; c < colCount; ++c) {
        QString head = normalize(model->headerData(c, Qt::Horizontal).toString());
        if (head.contains("photo") || head.contains("pdp")) cols["photo"] = c;
        else if (head.contains("id")) cols["id"] = c;
        else if (head == "nom") cols["nom"] = c;
        else if (head == "prenom") cols["prenom"] = c;
        else if (head.contains("date")) cols["date_embauche"] = c;
        else if (head.contains("poste")) cols["poste"] = c;
        else if (head.contains("salaire")) cols["salaire"] = c;
        else if (head.contains("email")) cols["email"] = c;
        else if (head.contains("sexe")) cols["sexe"] = c;
        else if (head.contains("telephone")) cols["telephone"] = c;
        else if (head.contains("statut")) cols["statut"] = c;
    }

    if (cols.size() < 10) {
        painter.end();
        QMessageBox::critical(this, "Erreur", "Certaines colonnes sont introuvables !");
        return;
    }

    // 📝 Titre stylisé du document
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(220, 230, 250));  // Fond doux pour le titre
    painter.drawRoundedRect(QRect(30, 20, writer.width() - 60, 60), 10, 10);

    // Texte centré
    painter.setFont(QFont("Georgia", 20, QFont::Bold));
    painter.setPen(QColor(25, 50, 120));
    painter.drawText(QRect(30, 20, writer.width() - 60, 60), Qt::AlignCenter, "📋 Liste des Employés");
    painter.setFont(QFont("Arial", 11));


    // 📄 Parcours des employés
    for (int row = 0; row < rowCount; ++row) {
        int posX = x;

        // 🎨 Fond alterné
        QColor bg = (row % 2 == 0) ? QColor(250, 250, 255) : QColor(240, 245, 255);
        painter.fillRect(QRect(30, y - 20, writer.width() - 60, rowHeight + 20), bg);

        // 🖼️ Photo
        QVariant cellData = model->data(model->index(row, cols["photo"]), Qt::UserRole + 1);
        QPixmap pixmap;
        if (cellData.canConvert<QVariantMap>()) {
            QByteArray imgData = cellData.toMap()["pdp"].toByteArray();
            pixmap.loadFromData(imgData);
        }

        if (!pixmap.isNull()) {
            QPixmap circle(photoSize, photoSize);
            circle.fill(Qt::transparent);
            QPainter circlePainter(&circle);
            circlePainter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, photoSize, photoSize);
            circlePainter.setClipPath(path);
            circlePainter.drawPixmap(0, 0, pixmap.scaled(photoSize, photoSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            painter.drawPixmap(posX, y, circle);
        }

        posX += photoSize + 30;

        // 📋 Données avec icônes
        QString rawDate = model->data(model->index(row, cols["date_embauche"])).toString();
        QDate date = QDate::fromString(rawDate.left(10), "yyyy-MM-dd");
        QString dateStr = date.isValid() ? date.toString("yyyy-MM.dd") : rawDate;

        QStringList lignes;
        lignes << QString("🆔 ID : %1").arg(model->data(model->index(row, cols["id"])).toString());
        lignes << QString("👤 Nom : %1 %2")
                      .arg(model->data(model->index(row, cols["prenom"])).toString())
                      .arg(model->data(model->index(row, cols["nom"])).toString());
        lignes << QString("📅 Embauche : %1").arg(dateStr);
        lignes << QString("💼 Poste : %1").arg(model->data(model->index(row, cols["poste"])).toString());
        lignes << QString("💰 Salaire : %1 DT")
                      .arg(QString::number(model->data(model->index(row, cols["salaire"])).toDouble(), 'f', 2));
        lignes << QString("📧 Email : %1").arg(model->data(model->index(row, cols["email"])).toString());
        lignes << QString("📞 Téléphone : %1").arg(model->data(model->index(row, cols["telephone"])).toString());
        lignes << QString("🔘 Sexe : %1").arg(model->data(model->index(row, cols["sexe"])).toString());
        lignes << QString("📌 Statut : %1").arg(model->data(model->index(row, cols["statut"])).toString());

        painter.setPen(Qt::black);
        QFont lineFont("Arial", 10);
        painter.setFont(lineFont);
        for (int i = 0; i < lignes.size(); ++i) {
            painter.drawText(posX, y + 20 + i * 16, lignes[i]);
        }

        y += rowHeight;

        // Nouvelle page
        if (y > writer.height() - 180) {
            writer.newPage();
            y = 100;
        }
    }

    // 🦶 Pied de page
    painter.setPen(QColor(80, 80, 80));
    painter.setFont(QFont("Arial", 9));
    QString footer = QString("📄 Généré le %1  |  Employés : %2")
                         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
                         .arg(rowCount);
    painter.drawText(QRect(0, writer.height() - 40, writer.width(), 20), Qt::AlignCenter, footer);

    painter.end();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    QMessageBox::information(this, "PDF généré", "🎉 La liste des employés a été exportée avec succès !");
}
void MainWindow::trierEmployes() {
    int columnIndex = ui->tableView->currentIndex().column();

    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return;
    }

    // Récupérer le modèle source (StandardItemModel) même si on a un proxy model
    QStandardItemModel *sourceModel = nullptr;
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

    if (proxyModel) {
        sourceModel = qobject_cast<QStandardItemModel*>(proxyModel->sourceModel());
    } else {
        sourceModel = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    }

    if (!sourceModel) {
        QMessageBox::warning(this, "Erreur", "Impossible de trier ce type de modèle");
        return;
    }

    // Si on n'a pas de proxy model, en créer un nouveau
    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(sourceModel);
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    }

    // Configurer le proxy model pour le tri numérique
    proxyModel->setSortRole(Qt::UserRole);

    // Pour chaque ligne, définir la valeur numérique dans le rôle UserRole
    for (int row = 0; row < sourceModel->rowCount(); ++row) {
        QStandardItem *item = sourceModel->item(row, columnIndex);
        if (item) {
            QString text = item->text();
            bool ok;
            double numericValue = text.toDouble(&ok);
            if (ok) {
                item->setData(numericValue, Qt::UserRole);
            } else {
                item->setData(text, Qt::UserRole);
            }
        }
    }

    // Appliquer le tri
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    ui->tableView->setModel(proxyModel);
}
void MainWindow::afficherStatistiques() {
    // Création de la scène
    QGraphicsScene *scene = new QGraphicsScene(this);
    int chartWidth = 900;  // Augmenter la largeur totale

    // Grand titre
    QLabel *titleLabel = new QLabel("📊 Statistiques des employés");
    titleLabel->setStyleSheet("font-size: 28pt; font-weight: bold; color: #2c3e50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedWidth(chartWidth);
    QGraphicsProxyWidget *titleProxy = scene->addWidget(titleLabel);
    titleProxy->setPos(0, 0);

    // =================== Haute Section : Répartition par Poste ===================
    QSqlQuery query1("SELECT poste, COUNT(*) AS effectif FROM employe GROUP BY poste");
    QPieSeries *posteSeries = new QPieSeries();
    int totalEmployes = 0;

    while (query1.next()) {
        int count = query1.value(1).toInt();
        totalEmployes += count;
        posteSeries->append(query1.value(0).toString(), count);
    }

    for (QPieSlice *slice : posteSeries->slices()) {
        slice->setExploded(true);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *posteChart = new QChart();
    posteChart->addSeries(posteSeries);
    posteChart->setTitle("📋 Répartition par poste");
    posteChart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    posteChart->setAnimationOptions(QChart::SeriesAnimations);
    posteChart->legend()->setAlignment(Qt::AlignBottom);
    posteChart->legend()->setBackgroundVisible(false);
    posteChart->setBackgroundBrush(QColor("#ecf0f1"));

    QChartView *posteChartView = new QChartView(posteChart);
    posteChartView->setRenderHint(QPainter::Antialiasing);
    posteChartView->setFixedSize(450, 350); // Ajustement de taille

    // Position pour le graphique par poste
    QGraphicsProxyWidget *posteProxy = scene->addWidget(posteChartView);
    posteProxy->setPos(0, 100); // Position en haut à gauche

    // =================== Basse Section : Répartition par Sexe ===================
    QSqlQuery query2("SELECT sexe FROM employe");
    int hommes = 0, femmes = 0;

    while (query2.next()) {
        QString sexe = query2.value(0).toString().toLower();
        if (sexe.contains("homme")) {
            hommes++;
        } else if (sexe.contains("femme")) {
            femmes++;
        }
    }

    QPieSeries *sexeSeries = new QPieSeries();
    sexeSeries->append("👨 Hommes", hommes);
    sexeSeries->append("👩 Femmes", femmes);

    for (QPieSlice *slice : sexeSeries->slices()) {
        slice->setExploded(true);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChart *sexeChart = new QChart();
    sexeChart->addSeries(sexeSeries);
    sexeChart->setTitle("👥 Répartition par sexe");
    sexeChart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    sexeChart->setAnimationOptions(QChart::SeriesAnimations);
    sexeChart->legend()->setAlignment(Qt::AlignBottom);
    sexeChart->legend()->setBackgroundVisible(false);
    sexeChart->setBackgroundBrush(QColor("#ecf0f1"));

    QChartView *sexeChartView = new QChartView(sexeChart);
    sexeChartView->setRenderHint(QPainter::Antialiasing);
    sexeChartView->setFixedSize(450, 350);

    // Position pour le graphique par sexe
    QGraphicsProxyWidget *sexeProxy = scene->addWidget(sexeChartView);
    sexeProxy->setPos(460, 100); // Position à droite sous le titre

    // Appliquer la scène au QGraphicsView
    scene->setSceneRect(0, 0, chartWidth + 20, 500);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setSceneRect(scene->sceneRect());
    // Animation d'apparition : ajouter une animation de fondu
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->graphicsView);
    ui->graphicsView->setGraphicsEffect(effect);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(400);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
void MainWindow::rechercherEmployes()
{
    QString searchText = ui->searchText->text().trimmed();

    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterRole(Qt::DisplayRole);
        ui->tableView->setModel(proxyModel);
    }
    proxyModel->setFilterKeyColumn(-1);  // Toutes colonnes
    QRegularExpression regex(searchText, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);
}
void MainWindow::setEmploye(const Employe &employe)
{
    this->employe = employe;

    ui->nom_2->setText(employe.getNom());
    ui->prenom_2->setText(employe.getPrenom());
    ui->date_embauche_2->setDate(employe.getDateEmbauche());
    ui->poste_2->setCurrentText(employe.getPoste());
    ui->salaire_2->setText(QString::number(employe.getSalaire()));
    ui->email_2->setText(employe.getEmail());
    ui->sexe_2->setCurrentText(employe.getSexe());
    ui->telephone_2->setText(employe.getTelephone());
    ui->statut_2->setCurrentText(employe.getStatut());

    if (!employe.getPdp().isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(employe.getPdp());

        if (!pixmap.isNull()) {
            ui->pushButton_pdp_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp_2->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2_2->setIconSize(QSize(50, 50));
        } else {
            qDebug() << "❌ Erreur : Impossible de charger l'image depuis les données binaires.";
        }
    } else {
        qDebug() << "⚠️ Aucune donnée d'image disponible.";
    }

    ui->tabWidget->setCurrentIndex(3);
}

Employe MainWindow::getEmployeModifie() const
{
    Employe employe;

    employe.setNom(ui->nom_2->text());
    employe.setPrenom(ui->prenom_2->text());
    employe.setDateEmbauche(ui->date_embauche_2->date());
    employe.setPoste(ui->poste_2->currentText());
    employe.setSalaire(ui->salaire_2->text().toDouble());
    employe.setEmail(ui->email_2->text());
    employe.setSexe(ui->sexe_2->currentText());
    employe.setTelephone(ui->telephone_2->text());
    employe.setStatut(ui->statut_2->currentText());

    employe.setPdp(this->employe.getPdp());
    employe.setId(this->employe.getId());

    return employe;
}

void MainWindow::on_pushButton_modifierPhoto_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            ui->pushButton_pdp_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp_2->setIconSize(QSize(150, 150));
            ui->pushButton_pdp2_2->setIcon(QIcon(pixmap));
            ui->pushButton_pdp2_2->setIconSize(QSize(50, 50));
            ui->pushButton_pdp2_2->setStyleSheet("border-radius: 25px; border: none;");

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");
            employe.setPdp(byteArray);
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image sélectionnée.");
        }
    }
}

void MainWindow::on_pushButton_enregistrer_clicked()
{
    if (ui->nom_2->text().isEmpty() || ui->prenom_2->text().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Tous les champs obligatoires doivent être remplis !");
        return;
    }

    Employe e = getEmployeModifie();

    if (e.modifier(e.getId())) {
        QMessageBox::information(this, "Succès", "✅ Modification enregistrée !");
        afficherEmployes();
        ui->tabWidget->setCurrentIndex(1);
    } else {
        QMessageBox::critical(this, "Erreur", "❌ La modification a échoué.");
    }
}

void MainWindow::on_pushButton_appliquerModif_clicked()
{
    if (ui->nom_2->text().isEmpty() || ui->prenom_2->text().isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    Employe updated;
    updated.setId(employe.getId());
    updated.setNom(ui->nom_2->text());
    updated.setPrenom(ui->prenom_2->text());
    updated.setDateEmbauche(ui->date_embauche_2->date());
    updated.setPoste(ui->poste_2->currentText());
    updated.setSalaire(ui->salaire_2->text().toDouble());
    updated.setEmail(ui->email_2->text());
    updated.setSexe(ui->sexe_2->currentText());
    updated.setTelephone(ui->telephone_2->text());
    updated.setStatut(ui->statut_2->currentText());
    updated.setPdp(employe.getPdp());

    QString ancienStatut = employe.getStatut();
    QString nouveauStatut = updated.getStatut();

    if (updated.modifier(updated.getId())) {
        QMessageBox::information(this, "Succès", "✅ Employé modifié avec succès !");
        afficherEmployes();
        ui->tabWidget->setCurrentIndex(1);

        // ✉️ Envoi de mail si le statut a changé
        if (ancienStatut != nouveauStatut) {
            envoyerEmailSelonStatut(
                updated.getNom(),
                updated.getPrenom(),
                updated.getEmail(),
                nouveauStatut
                );
        }

    } else {
        QMessageBox::critical(this, "Erreur", "❌ La mise à jour a échoué !");
    }
}

QPixmap createRoundedPixmap(const QPixmap &src, int size)
{
    QPixmap dest(size, size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, size, size, src);
    return dest;
}

void MainWindow::afficherDetailsEmploye(const QModelIndex &index)
{
    if (index.column() != 0) return;

    QVariant empData = index.data(Qt::UserRole + 1);
    if (!empData.isValid()) {
        qDebug() << "❌ Aucune donnée employé trouvée";
        return;
    }

    QVariantMap employee = empData.toMap();
    qDebug() << "📸 Affichage des détails de l'employé:" << employee;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Détails de l'employé");
    dialog->setFixedSize(350, 500);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // 📷 Affichage de la photo
    QLabel *photoLabel = new QLabel();
    QPixmap photo;
    QByteArray photoData = employee["pdp"].toByteArray();

    qDebug() << "Taille des données photo:" << photoData.size();

    if (!photoData.isEmpty() && photo.loadFromData(photoData)) {
        photo = photo.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        photo.load(":/img/default-user.png");
        photo = photo.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    photoLabel->setPixmap(photo);
    photoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(photoLabel);

    // 🔹 Infos employé
    QFrame *infoFrame = new QFrame();
    QFormLayout *formLayout = new QFormLayout(infoFrame);
    formLayout->setSpacing(5);

    // Correction des clés pour correspondre aux données stockées
    QStringList fields = {"ID", "Nom", "Prénom", "Poste", "Salaire", "Email", "Téléphone"};
    QStringList keys = {"id", "nom", "prenom", "poste", "salaire", "email", "telephone"};

    for (int i = 0; i < fields.size(); ++i) {
        QLabel *label = new QLabel(employee[keys[i]].toString());
        label->setStyleSheet("font: 10pt; padding: 2px;");

        QLabel *title = new QLabel(fields[i] + ":");
        title->setStyleSheet("font-weight: bold; font: 10pt; color: #2c3e50;");

        formLayout->addRow(title, label);
    }

    infoFrame->setStyleSheet("background: #f8f9fa; border-radius: 10px; padding: 10px;");
    layout->addWidget(infoFrame);

    // 📄 Ajout d'un bouton pour générer le PDF
    QPushButton *pdfButton = new QPushButton("Exporter en PDF");
    layout->addWidget(pdfButton);

    connect(pdfButton, &QPushButton::clicked, this, [this, employee]() {
        genererPDF(employee);
    });

    dialog->exec();
    delete dialog;
}

void MainWindow::genererPDF(const QVariantMap &employee)
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Enregistrer PDF", "details_employe.pdf", "Fichiers PDF (*.pdf)");
    if (filename.isEmpty()) return;

    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);
    writer.setPageMargins(QMarginsF(40, 40, 40, 40));

    QPainter painter;
    if (!painter.begin(&writer)) return;

    int pageWidth = writer.width();
    int pageHeight = writer.height();
    int margin = 100;

    // Arrière-plan
    painter.fillRect(0, 0, pageWidth, pageHeight, QColor(252, 252, 252));
    painter.fillRect(0, 0, pageWidth, 350, QColor(245, 245, 245));

    // PHOTO À GAUCHE
    int photoX = margin;
    int photoY = 50;
    int photoSize = 280;

    QByteArray photoData = employee["pdp"].toByteArray();
    if (!photoData.isEmpty()) {
        QPixmap photo;
        if (photo.loadFromData(photoData)) {
            QPixmap rounded(photoSize, photoSize);
            rounded.fill(Qt::transparent);

            QPainter roundPainter(&rounded);
            roundPainter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, photoSize, photoSize);
            roundPainter.setClipPath(path);

            QPixmap scaled = photo.scaled(photoSize, photoSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            roundPainter.drawPixmap(0, 0, scaled);
            roundPainter.end();

            painter.drawPixmap(photoX, photoY, rounded);
        }
    }

    // Titre à droite
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(QRect(photoX + photoSize + 50, 140, pageWidth - photoSize - 200, 80), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("Détails de l'employé"));

    // Carte Informations
    int infoStartY = 370;
    int cardWidth = pageWidth - (2 * margin);
    int cardHeight = 1000;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(220, 220, 220, 100));
    painter.drawRoundedRect(margin + 5, infoStartY + 5, cardWidth, cardHeight, 15, 15);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(margin, infoStartY, cardWidth, cardHeight, 15, 15);

    // Détails
    int x = margin + 80;
    int y = infoStartY + 80;
    int lineHeight = 135; // ✅ Espacement augmenté ici

    QStringList fields = {
        QStringLiteral("ID"), QStringLiteral("Nom"), QStringLiteral("Prénom"),
        QStringLiteral("Poste"), QStringLiteral("Salaire"), QStringLiteral("Email"),
        QStringLiteral("Tél")
    };
    QStringList keys = {"id", "nom", "prenom", "poste", "salaire", "email", "telephone"};

    for (int i = 0; i < fields.size(); ++i) {
        if (employee.contains(keys[i])) {
            // Label
            painter.setPen(QColor(50, 50, 50));
            painter.setFont(QFont("Arial", 18, QFont::Bold));
            QRect labelRect(x, y, 300, 60);
            painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, fields[i] + " :");

            // Valeur
            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 18));
            QString value = employee[keys[i]].toString();
            if (fields[i] == QStringLiteral("Salaire"))
                value += QStringLiteral(" DT");

            QRect valueRect(x + 320, y, cardWidth - 400, 60);
            painter.drawText(valueRect, Qt::AlignLeft | Qt::AlignVCenter, value);

            // Ligne de séparation
            if (i < fields.size() - 1) {
                painter.setPen(QPen(QColor(200, 200, 200), 2));
                painter.drawLine(x - 30, y + lineHeight - 35, x + cardWidth - 100, y + lineHeight - 35);
            }

            y += lineHeight;
        }
    }

    // Pied de page
    painter.setPen(QColor(90, 90, 90));
    painter.setFont(QFont("Arial", 14));
    QString dateStr = "Document généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm");

    // ... pied de page ...
    painter.drawText(QRect(margin, pageHeight - 60, cardWidth, 40), Qt::AlignCenter, dateStr);

    // 🔚 Fin du dessin
    painter.end();

    // 🚀 Ouvre automatiquement le fichier PDF généré
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

    QMessageBox::information(nullptr, "Succès", "✅ Le PDF a été généré avec succès !");

}


void MainWindow::saveImageToDatabase(const QPixmap &pixmap) {
    if (pixmap.isNull()) {
        qDebug() << "❌ Image invalide";
        return;
    }

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    if (!pixmap.save(&buffer, "PNG")) {
        qDebug() << "❌ Échec de la conversion de l'image";
        return;
    }
    buffer.close();

    QSqlQuery query;
    query.prepare("UPDATE employe SET pdp = :imageData WHERE id = :id");
    query.bindValue(":imageData", imageData);
    query.bindValue(":id", employe.getId());

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la mise à jour de l'image:" << query.lastError().text();
    } else {
        qDebug() << "✅ Image mise à jour avec succès";
    }
}

void MainWindow::onPhotoClicked(const QModelIndex &index) {
    if (index.column() != 0) return;

    QVariant empData = index.data(Qt::UserRole + 1);
    if (!empData.isValid()) {
        qDebug() << "❌ Aucune donnée employé trouvée";
        return;
    }

    QVariantMap employee = empData.toMap();
    qDebug() << "📸 Affichage des détails de l'employé:" << employee;

    afficherDetailsEmploye(index);
}
void MainWindow::chargementTodoList()
{
    if (ui->tableTodo->model())
        delete ui->tableTodo->model();

    QSqlQuery query(R"(
        SELECT
            m.id,
            m.type,
            m.description,
            m.statut,
            m.date_resolution AS date_resolution
        FROM maintenance m
    )");

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ "✔", "🔧 Type", "📝 Description", "📌 Statut", "📅 Résolution" });

    while (query.next()) {
        QList<QStandardItem *> row;

        // Case à cocher (colonne 0)
        QStandardItem *checkItem = new QStandardItem();
        checkItem->setCheckable(true);
        QString statut = query.value("statut").toString();
        checkItem->setCheckState(statut.toLower() == "traitée" ? Qt::Checked : Qt::Unchecked);
        checkItem->setData(query.value("id").toInt()); // pour update
        row << checkItem;

        // Type, description
        row << new QStandardItem(query.value("type").toString());
        row << new QStandardItem(query.value("description").toString());

        // Statut (badge coloré)
        QStandardItem *statutItem = new QStandardItem(statut);
        QString couleur = "#ffffff";
        if (statut.toLower() == "non traitée") couleur = "#ffeaa7";
        else if (statut.toLower() == "en cours") couleur = "#d6eaff";
        else if (statut.toLower() == "traitée") couleur = "#d4edda";
        statutItem->setBackground(QColor(couleur));
        row << statutItem;

        // Date résolution
        row << new QStandardItem(query.value("date_resolution").toString());

        model->appendRow(row);
    }

    ui->tableTodo->setModel(model);

    for (int i = 0; i < model->columnCount(); ++i)
        ui->tableTodo->setColumnWidth(i, 160);

    // Style
    ui->tableTodo->setStyleSheet(R"(
        QTableView {
            background-color: #f4f6f8;
            font-size: 14px;
            alternate-background-color: #ffffff;
            selection-background-color: #aee1f9;
            border: 1px solid #ccc;
            border-radius: 8px;
        }
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            font-weight: bold;
            font-size: 14px;
            padding: 6px;
        }
        QTableView::item {
            padding: 5px;
        }
        QTableView::item:hover {
            background-color: #eaf4fb;
        }
    )");

    ui->tableTodo->horizontalHeader()->setStretchLastSection(true);
    ui->tableTodo->setAlternatingRowColors(true);
    ui->tableTodo->verticalHeader()->setVisible(false);
    ui->tableTodo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTodo->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Animation à l'apparition
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->tableTodo);
    ui->tableTodo->setGraphicsEffect(effect);
    QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity");
    fade->setDuration(500);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    // Mise à jour du statut en BDD quand on coche
    connect(model, &QStandardItemModel::itemChanged, this, [=](QStandardItem *item) {
        if (item->column() == 0) {
            int id = item->data().toInt();
            bool isChecked = item->checkState() == Qt::Checked;
            QString newStatus = isChecked ? "traitée" : "en cours";

            QSqlQuery update;
            update.prepare("UPDATE maintenance SET statut = :statut WHERE id = :id");
            update.bindValue(":statut", newStatus);
            update.bindValue(":id", id);

            if (!update.exec()) {
                qDebug() << "❌ Erreur update maintenance :" << update.lastError().text();
            } else {
                model->setData(model->index(item->row(), 3), newStatus);
                // Optionnel : changer la couleur du badge
                QColor color = (newStatus == "traitée") ? QColor("#d4edda") : QColor("#d6eaff");
                model->item(item->row(), 3)->setBackground(color);
            }
        }
    });

    qDebug() << "✅ To-Do list chargée avec badge et checkbox.";
}

void MainWindow::toutMarquerCommeTermine()
{
    QSqlQuery update("UPDATE maintenance SET statut = 'traitée' WHERE statut <> 'traitée' OR statut IS NULL");
    if (!update.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de tout marquer comme terminé !");
    } else {
        chargementTodoList();
    }
}

void MainWindow::on_btnAccederTodo_clicked()
{
    FaceLoginDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->tabWidget->setCurrentIndex(5);  // Index de l'onglet Todo List
        chargementTodoList();
    }
}
void MainWindow::on_pushButton_faceId_clicked()
{
    // Récupérer le dernier ID inséré dans la table employe
    QSqlQuery query;
    query.prepare("SELECT MAX(id) FROM employe");

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer l'ID de l'employé");
        return;
    }

    QString employeeId = query.value(0).toString();
    if (employeeId.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord ajouter l'employé");
        return;
    }

    // Appeler la fonction d'enregistrement du FaceID
    if (FaceLoginDialog::registerFaceID(this, employeeId)) {
        QMessageBox::information(this, "Succès", "FaceID enregistré avec succès");
        // Mettre à jour l'icône du bouton pour indiquer que le FaceID est enregistré
        ui->pushButton_faceId->setStyleSheet(ui->pushButton_faceId->styleSheet() + "\nbackground-color: #4CAF50;");
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'enregistrement du FaceID");
    }
}
void MainWindow::on_btnVerifier_clicked()
{
    QString nomSaisi = ui->lineEdit_nom->text().trimmed();
    QSqlQuery query;
    query.prepare("SELECT id, nom, face_id FROM employes WHERE nom = :nom");
    query.bindValue(":nom", nomSaisi);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

}
void MainWindow::on_cvButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir un CV", "", "Fichiers texte (*.txt)");
    if (filePath.isEmpty()) return;

    QString extractedText;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        extractedText = in.readAll();
        file.close();
    }

    if (extractedText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le fichier est vide ou non lisible.");
        return;
    }

    // === Extraction des infos ===
    QString nom, prenom, email, tel, sexe, poste;

    // 🔍 Email & téléphone
    QRegularExpression reEmail(R"(([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");
    QRegularExpression rePhone(R"((\+?\d[\d\s\-]{6,}\d))");

    QRegularExpressionMatch matchEmail = reEmail.match(extractedText);
    if (matchEmail.hasMatch()) email = matchEmail.captured(1);

    QRegularExpressionMatch matchPhone = rePhone.match(extractedText);
    if (matchPhone.hasMatch()) tel = matchPhone.captured(1);

    // 🔤 Nom / prénom (simple heuristique)
    QStringList lines = extractedText.split("\n", Qt::SkipEmptyParts);
    if (!lines.isEmpty()) {
        QStringList names = lines.first().split(" ", Qt::SkipEmptyParts);
        if (names.size() >= 2) {
            prenom = names.first();
            nom = names.last();
        }
    }

    // 🔮 Détection sexe
    QString lowerText = extractedText.toLower();
    if (lowerText.contains("née le") || lowerText.contains("madame") || lowerText.contains("elle")) {
        sexe = "Femme";
    } else if (lowerText.contains("né le") || lowerText.contains("monsieur") || lowerText.contains("il")) {
        sexe = "Homme";
    }

    // 🎯 Détection poste (keywords)
    QStringList postes = {
        "développeur", "developpeur", "ingénieur", "ingenieur",
        "technicien", "chef de projet", "designer", "analyste",
        "data scientist", "administrateur", "manager"
    };

    for (const QString &mot : postes) {
        if (lowerText.contains(mot)) {
            poste = mot.left(1).toUpper() + mot.mid(1);  // Mettre en maj la première lettre
            break;
        }
    }

    // ✅ Remplir les champs
    ui->nom->setText(nom);
    ui->prenom->setText(prenom);
    ui->email->setText(email);
    ui->telephone->setText(tel);
    ui->sexe->setCurrentText(sexe);
    ui->poste->setCurrentText(poste);


    // ✅ Message OK
    QMessageBox::information(this, "Extraction réussie", "✅ Données extraites avec succès !");
}


//empreinte

void MainWindow::on_btnAjouterEmpreinte_clicked()
{
    // 1. Récupérer le dernier ID employé inséré
    QSqlQuery query;
    if (!query.exec("SELECT id FROM employe ORDER BY id DESC LIMIT 1")) {
        QString errorMsg = "Impossible de récupérer le dernier ID employé:\n" + query.lastError().text();

        // Solution alternative pour les drivers qui ne supportent pas LIMIT
        if (!query.exec("SELECT MAX(id) FROM employe")) {
            QMessageBox::critical(this, "Erreur SQL", errorMsg);
            return;
        }
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Avertissement", "Aucun employé trouvé dans la base de données");
        return;
    }

    int dernierId = query.value(0).toInt();
    if (dernierId <= 0) {
        QMessageBox::warning(this, "Avertissement", "ID employé invalide");
        return;
    }

    // 2. Vérifier la connexion Arduino
    if (!arduino->estConnecte()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Arduino non connecté",
            "Le capteur d'empreintes n'est pas connecté. Voulez-vous essayer de vous reconnecter ?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes && !arduino->connecter()) {
            QMessageBox::critical(this, "Erreur", "Échec de la connexion au capteur");
            return;
        }
        else if (reply == QMessageBox::No) {
            return;
        }
    }

    // 3. Préparer l'interface utilisateur
    ui->btnAjouterEmpreinte->setEnabled(false);
    ui->btnAjouterEmpreinte->setText("Enregistrement en cours...");
    QApplication::processEvents(); // Forcer la mise à jour de l'UI

    // 4. Afficher les instructions
    QMessageBox::information(this,
                             "Enregistrement d'empreinte",
                             QString("Prêt à enregistrer l'empreinte pour l'employé ID: %1\n\n"
                                     "Veuillez placer votre doigt sur le capteur.\n"
                                     "Vous devrez le placer deux fois pour une bonne capture.").arg(dernierId));

    // 5. Envoyer la commande à l'Arduino
    arduino->demarrerEnregistrement(dernierId); // Utiliser la méthode correcte

    // Le reste est géré par:
    // - onEmpreinteEnregistree() en cas de succès
    // - onErreurArduino() en cas d'échec
}
void MainWindow::onEmpreinteEnregistree(int idEmploye)
{
    // 1. Vérifier que l'ID correspond au dernier ID dans la base de données
    QSqlQuery verifQuery;
    verifQuery.prepare("SELECT MAX(id) FROM employe");

    if (!verifQuery.exec()) {
        QMessageBox::critical(this, "Erreur BD",
                              QString("Erreur lors de l'exécution de la requête SELECT MAX(id): %1")
                                  .arg(verifQuery.lastError().text()));
        return;
    }

    if (!verifQuery.next()) {
        QMessageBox::critical(this, "Erreur BD",
                              "Impossible de lire la réponse de la base de données.");
        return;
    }

    QVariant maxIdVariant = verifQuery.value(0);
    if (maxIdVariant.isNull()) {
        QMessageBox::warning(this, "BD vide", "Aucun employé trouvé dans la base.");
        return;
    }

    int currentMaxId = maxIdVariant.toInt();
    if (idEmploye != currentMaxId) {
        QMessageBox::warning(this, "Avertissement",
                             QString("L'ID de l'empreinte (%1) ne correspond pas au dernier employé (%2)")
                                 .arg(idEmploye).arg(currentMaxId));
        return; // Arrêter l'exécution si l'ID est incorrect
    }

    // 2. Mettre à jour l'attribut 'empreinte' ET 'id_empreinte_capteur'
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE employe SET empreinte = ?, id_empreinte_capteur = ? WHERE id = ?");

    QByteArray dummyData = QByteArray::fromHex("00"); // Donnée factice pour test, à remplacer par la vraie empreinte si dispo

    // Supposons pour l'exemple qu'on veut utiliser idEmploye comme id_empreinte_capteur :
    int idEmpreinteCapteur = idEmploye; // à adapter selon ton vrai ID du capteur, si tu l'as dans ta logique

    updateQuery.addBindValue(dummyData);           // Ajouter la valeur BLOB
    updateQuery.addBindValue(idEmpreinteCapteur);  // Ajouter l'ID depuis le capteur
    updateQuery.addBindValue(idEmploye);           // Ajouter l'ID

    if (updateQuery.exec()) {
        QMessageBox::information(this, "Succès",
                                 QString("Empreinte enregistrée avec succès pour l'employé #%1").arg(idEmploye));

        // Mettre à jour l'affichage si besoin
        afficherEmployes();
    } else {
        QMessageBox::critical(this, "Erreur BD",
                              QString("Erreur lors de la mise à jour: %1").arg(updateQuery.lastError().text()));
    }

    // 3. Réinitialiser l'interface
    ui->btnAjouterEmpreinte->setEnabled(true);
    ui->btnAjouterEmpreinte->setText("Ajout avec succés");
}
void MainWindow::onErreurArduino(const QString &message)
{
    // 1. Afficher le message d'erreur dans la barre de statut
    ui->statusbar->showMessage("Erreur Arduino: " + message, 10000); // 10 secondes

    // 2. Journalisation de l'erreur
    qCritical() << "Erreur Arduino:" << message;

    // 3. Feedback visuel sur le bouton
    QString originalStyle = ui->btnAjouterEmpreinte->styleSheet();
    QString originalText = ui->btnAjouterEmpreinte->text();

    // Changement visuel temporaire
    ui->btnAjouterEmpreinte->setStyleSheet("background-color: #ffcccc; color: #cc0000;");
    ui->btnAjouterEmpreinte->setText("Erreur - Cliquez pour réessayer");
    ui->btnAjouterEmpreinte->setEnabled(true);

    // 4. Gestion des erreurs spécifiques
    if(message.contains("capteur", Qt::CaseInsensitive)) {
        QMessageBox::critical(this, "Erreur Capteur",
                              "Problème de communication avec le capteur d'empreintes.\n"
                              "Veuillez vérifier:\n"
                              "1. Que le capteur est bien branché\n"
                              "2. Qu'aucun autre programme n'utilise le port série\n"
                              "3. Que le câble USB est en bon état");
    }
    else if(message.contains("timeout", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Timeout",
                             "Temps d'attente dépassé. Veuillez réessayer.");
    }
    else {
        QMessageBox::warning(this, "Erreur", message);
    }

    // 5. Réinitialisation après 5 secondes
    QTimer::singleShot(5000, this, [this, originalStyle, originalText]() {
        ui->btnAjouterEmpreinte->setStyleSheet(originalStyle);
        ui->btnAjouterEmpreinte->setText(originalText);
        ui->statusbar->clearMessage();
    });

    // 6. Tentative automatique de reconnexion pour certaines erreurs
    if(message.contains("connexion", Qt::CaseInsensitive)) {
        QTimer::singleShot(3000, this, [this]() {
            if(!arduino->estConnecte()) {
                ui->statusbar->showMessage("Tentative de reconnexion...", 3000);
                arduino->connecter();
            }
        });
    }
}
void MainWindow::on_btnCheckEmpreinte_clicked()
{
    if (!arduino->estConnecte()) {
        QMessageBox::warning(this,"Erreur","Arduino non connecté.");
        return;
    }
    arduino->demarrerVerification(); // Envoie "VERIFY" à Arduino, qui fait la détection/matching
}
void MainWindow::slotEmpreinteReconnue(int idCapteur)
{
    // 1 → Rechercher l'employé ayant id_empreinte_capteur = idCapteur
    QSqlQuery q;
    q.prepare("SELECT id, nom, prenom FROM employe WHERE id_empreinte_capteur = :cid"); // adapte le champ si besoin
    q.bindValue(":cid", idCapteur);
    if (q.exec() && q.next()) {
        int idEmploye = q.value(0).toInt(); // l’id réel de la table ‘employe’ (clé primaire)
        QString nom = q.value(1).toString();
        QString prenom = q.value(2).toString();
        // 2 → Affiche la notification (id “humain” employé !)
        QMessageBox::information(this, "Succès",
                                 QString("Empreinte reconnue pour %1 %2 (id: %3)").arg(prenom, nom).arg(idEmploye));
        ui->tabWidget->setCurrentIndex(5);
        chargementTodoList(); // On charge la todoList de cet employé, via la vraie id BDD !
    } else {
        QMessageBox::critical(this, "Erreur", "Employé non trouvé pour cette empreinte capteur !");
    }
}
void MainWindow::slotEmpreinteNonTrouvee()
{
    QMessageBox::critical(this, "Accès refusé", "Empreinte inconnue, accès impossible.");
    // Tu peux aussi logger, bloquer, etc.
}
void MainWindow::genererBadgePDF(const QVariantMap &employee)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le badge", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A6));
    writer.setPageOrientation(QPageLayout::Portrait);  // Plus adapté à un badge
    writer.setResolution(300);
    QPainter painter(&writer);

    // Marges
    const int margin = 30;
    QRectF pageRect(margin, margin, writer.width() - 2 * margin, writer.height() - 2 * margin);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fond
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#e3f2fd"));
    painter.drawRoundedRect(pageRect, 20, 20);

    int y = static_cast<int>(pageRect.top()) + 20;

    // PHOTO
    QByteArray photoData = employee.value("pdp").toByteArray();
    QPixmap photo = !photoData.isEmpty() ? QPixmap::fromImage(QImage::fromData(photoData))
                                         : QPixmap(":/img/default-user.png");

    QPixmap roundPhoto(100, 100);
    roundPhoto.fill(Qt::transparent);
    {
        QPainter p(&roundPhoto);
        QPainterPath path;
        path.addEllipse(0, 0, 100, 100);
        p.setRenderHint(QPainter::Antialiasing);
        p.setClipPath(path);
        p.drawPixmap(0, 0, photo.scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }

    int photoX = pageRect.left() + (pageRect.width() - 100) / 2;
    painter.drawPixmap(photoX, y, roundPhoto);
    y += 110; // espace après photo

    // TITRE
    QFont titleFont("Arial", 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor("#0a3871"));
    QRectF titleRect(pageRect.left(), y, pageRect.width(), 30);
    painter.drawText(titleRect, Qt::AlignCenter, "BADGE EMPLOYÉ");
    y += 35;

    // NOM
    QFont nameFont("Arial", 15, QFont::Bold);
    painter.setFont(nameFont);
    painter.setPen(Qt::black);
    QRectF nameRect(pageRect.left(), y, pageRect.width(), 25);
    painter.drawText(nameRect, Qt::AlignCenter, employee.value("nom").toString() + " " + employee.value("prenom").toString());
    y += 30;

    // POSTE
    QFont posteFont("Arial", 12, QFont::StyleItalic);
    painter.setFont(posteFont);
    painter.setPen(QColor("#1766b0"));
    QRectF posteRect(pageRect.left(), y, pageRect.width(), 20);
    painter.drawText(posteRect, Qt::AlignCenter, employee.value("poste").toString());
    y += 30;

    // ESPACE avant code-barres
    y += 15;

    // CODE-BARRES
    QString idTxt = employee.value("id").toString();
    QString codeData = "*" + idTxt + "*";

    int fontId = QFontDatabase::addApplicationFont(":/fonts/code128.ttf");
    QString family = (fontId != -1 && !QFontDatabase::applicationFontFamilies(fontId).isEmpty())
                         ? QFontDatabase::applicationFontFamilies(fontId).at(0)
                         : "Courier"; // police de repli

    QFont codeFont(family, 32);
    painter.setFont(codeFont);
    painter.setPen(Qt::black);

    QRectF barcodeRect(pageRect.left() + 40, y, pageRect.width() - 80, 40);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(barcodeRect, 6, 6);
    painter.setPen(Qt::black);
    painter.drawText(barcodeRect, Qt::AlignCenter, codeData);
    y += 50;

    // ID
    QFont idFont("Arial", 10);
    painter.setFont(idFont);
    QRectF idRect(barcodeRect.left(), y, barcodeRect.width(), 20);
    painter.drawText(idRect, Qt::AlignCenter, "ID : " + idTxt);

    // FIN RECTO
    painter.end();

    QMessageBox::information(this, "PDF généré", "✅ Le badge a été généré proprement.");
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}


void MainWindow::suggestEmailsByIA(const QString& prenom, const QString& nom)
{
    // ✅ Prompt IA : conventions françaises + domaine esprit.tn
    QString prompt = QString(
                         "En te basant sur les conventions réelles utilisées en France et dans les écoles d’ingénieurs comme ESPRIT, "
                         "devine 6 adresses email plausibles pour une personne nommée \"%1 %2\". "
                         "Utilise des formats réalistes (ex : nomprenom, prénom.nom, nom.prénom, etc.) comme on en trouve sur les CV, réseaux sociaux et dans les écoles. "
                         "Utilise uniquement les domaines suivants : gmail.com, hotmail.com, outlook.com, esprit.tn. "
                         "Réponds uniquement par les emails séparés par une virgule, sans aucun texte ni ponctuation superflue."
                         ).arg(prenom.trimmed(), nom.trimmed());

    // 🧠 Debug du prompt généré
    qDebug() << "[GPT PROMPT]" << prompt;

    // 🔐 Requête OpenAI
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 🧩 Structure du message
    QJsonArray messages = QJsonArray{
        QJsonObject{{"role", "system"},
                    {"content", "Tu es un générateur intelligent d'adresses email basées sur des conventions professionnelles en France et dans les écoles comme ESPRIT."}},
        QJsonObject{{"role", "user"}, {"content", prompt}}
    };

    QJsonObject reqBody{
        {"model", "gpt-3.5-turbo"},
        {"messages", messages},
        {"max_tokens", 150},
        {"temperature", 0.4}
    };

    QJsonDocument doc(reqBody);

    // 📤 Envoi requête
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, doc.toJson());

    // 🔄 Callback à la réception
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        reply->deleteLater();
        manager->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(this, "Erreur IA", reply->errorString());
            qDebug() << "[ERREUR]" << reply->errorString();
            return;
        }

        qDebug() << "[GPT RAW RESPONSE]" << response;

        // 🧹 Parsing JSON
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QStringList emails;

        if (jsonDoc.isObject()) {
            QJsonArray choices = jsonDoc.object()["choices"].toArray();
            if (!choices.isEmpty()) {
                QString content = choices.first().toObject()["message"].toObject()["content"].toString();
                QStringList rawList = content.split(QRegularExpression("[,;\\n]"), Qt::SkipEmptyParts);
                for (QString email : rawList) {
                    email = email.trimmed();
                    if (email.contains("@") && email.length() < 100) {
                        emails << email;
                    }
                }
            }
        }

        // 🧠 Affichage Debug
        qDebug() << "[EMAILS SUGGÉRÉS]" << emails;

        // ✅ Application au champ email (autocomplete)
        if (!emails.isEmpty()) {
            QCompleter *completer = new QCompleter(emails, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            completer->setCompletionMode(QCompleter::PopupCompletion);
            ui->email->setCompleter(completer); // 🔁 Remplace par ton vrai champ
            ui->email->setPlaceholderText("Suggestion IA…");
        } else {
            QMessageBox::information(this, "Suggestion IA", "Aucune suggestion disponible.");
        }
    });
}
void MainWindow::envoyerEmailSelonStatut(const QString& nom, const QString& prenom, const QString& email, const QString& statut)
{
    qDebug() << "[INFO] Fonction envoyerEmailSelonStatut appelée pour :" << prenom << nom << "Email:" << email << "Statut:" << statut;

    if (statut != "congé" && statut != "inactif") {
        qDebug() << "[INFO] Aucun email à envoyer pour ce statut.";
        return;
    }

    QString sujet;
    QString prompt;

    if (statut == "congé") {
        sujet = "Confirmation de congé";
        prompt = QString("Rédige un email professionnel à %1 %2 pour l'informer qu’il bénéficie de 3 jours de congé. "
                         "Utilise un ton respectueux et positif. Commence par 'Bonjour' et termine par 'Cordialement, Direction RH'.")
                     .arg(prenom, nom);
    } else if (statut == "inactif") {
        sujet = "Mise en inactivité";
        prompt = QString("Rédige un email professionnel à %1 %2 pour l'informer que son statut est passé à 'Inactif'. "
                         "Invite-le à contacter les RH pour plus de détails. Sois formel. Commence par 'Bonjour' et termine par 'Cordialement, Direction RH'.")
                     .arg(prenom, nom);
    }

    // Étape 1 : Générer le contenu avec OpenAI
    QNetworkRequest req(QUrl("https://api.openai.com/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonArray messages = {
        QJsonObject{{"role", "system"}, {"content", "Tu es un assistant RH qui rédige des emails professionnels."}},
        QJsonObject{{"role", "user"}, {"content", prompt}}
    };

    QJsonObject reqBody;
    reqBody["model"] = "gpt-3.5-turbo";
    reqBody["messages"] = messages;
    reqBody["max_tokens"] = 250;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(req, QJsonDocument(reqBody).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "[ERREUR] OpenAI : " << reply->errorString();
            QMessageBox::critical(this, "Erreur OpenAI", reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);
        QString contenu;

        if (json.object().contains("choices")) {
            contenu = json["choices"].toArray().first().toObject()["message"].toObject()["content"].toString();
            qDebug() << "[DEBUG] Contenu généré : " << contenu;
        } else {
            qDebug() << "[ERREUR] Réponse JSON inattendue";
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        // Étape 2 : Envoyer par SMTP Gmail directement
        QSslSocket* socket = new QSslSocket(this);
        socket->connectToHostEncrypted("smtp.gmail.com", 465);

        if (!socket->waitForConnected(5000)) {
            qDebug() << "[ERREUR] Connexion SMTP échouée.";
            return;
        }

        QTextStream stream(socket);
        stream.setAutoDetectUnicode(true);

        auto waitForResponse = [&]() {
            if (!socket->waitForReadyRead(5000)) {
                qDebug() << "[ERREUR] Pas de réponse serveur.";
                return false;
            }
            QByteArray response = socket->readAll();
            qDebug() << "[SMTP]" << response;
            return true;
        };

        stream << "EHLO smtp.gmail.com\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "AUTH LOGIN\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << QByteArray("yassmine.nouisser@esprit.tn").toBase64() << "\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << QByteArray("231JFT3921").toBase64() << "\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "MAIL FROM:<espritsmm@gmail.com>\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "RCPT TO:<" << email << ">\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "DATA\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "Subject: " << sujet << "\r\n";
        stream << "From: espritsmm@gmail.com\r\n";
        stream << "To: " << email << "\r\n";
        stream << "Content-Type: text/plain; charset=UTF-8\r\n";
        stream << "\r\n" << contenu << "\r\n.\r\n";
        stream.flush();
        if (!waitForResponse()) return;

        stream << "QUIT\r\n";
        stream.flush();
        waitForResponse();

        qDebug() << "[INFO] Email envoyé avec succès.";

        socket->disconnectFromHost();
        socket->deleteLater();

        reply->deleteLater();
        manager->deleteLater();
    });
}





//__________________youssef____________________








void MainWindow::updateChatSenderName(int index) {
    if (!m_chatWindow) return;

    const QVector<QString> names = {"youssef", "yassmine", "insaf", "malek"};

    if (index >= 0 && index < names.size()) {
        m_chatWindow->setSenderName(names[index]);
    }
}


void MainWindow::openChatWindow() {
    if (!m_chatManager) {
        QMessageBox::warning(this, "Erreur", "Service de chat indisponible");
        return;
    }

    if (!m_chatWindow) {
        // Initialise avec le nom correspondant à l'onglet actuel
        int currentTab = ui->tabWidget_srv->currentIndex();
        const QString initialName = currentTab >= 0 && currentTab < 4 ?
                                        QVector<QString>{"youssef", "yassmine", "insaf", "malek"}[currentTab] :
                                        "invité";

        m_chatWindow = new chatwindow(initialName, m_chatManager, this);
        connect(m_chatWindow.data(), &::chatwindow::windowRole,
                [this]() { m_chatWindow.clear(); });
        // Connectez le changement d'onglet
        connect(ui->tabWidget_srv, &QTabWidget::currentChanged,
                m_chatWindow.data(), [this](int index) {
                    updateChatSenderName(index);
                });
    }

    m_chatWindow->show();
    m_chatWindow->raise();
    m_chatWindow->activateWindow();
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
            ui->textEdit_Description_srv->insertPlainText(QString(description[*index]));
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
    QString nom = ui->lineEdit_Nom_srv->text().trimmed();

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
    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du chargement des données : " + model->lastError().text());
        delete model;
        return;
    }

    ui->tableView_srv->setModel(model);
    qDebug() << "Résultats affichés avec succès.";

    // Appliquer un style visuel
    ui->tableView_srv->setStyleSheet(
        "QtableView {"
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

void MainWindow::on_ajouter_srv_clicked()
{
    // Récupérer les valeurs des champs
    QString nom = ui->lineEdit_Nom_srv->text().trimmed();
    QString description = ui->textEdit_Description_srv->toPlainText().trimmed();
    QString statut = ui->comboBox_Statut_srv->currentText();
    QString frequence = ui->comboBox_Frequence_srv->currentText();
    QDate dateDebut = ui->dateEdit_Debut_srv->date();
    QDate dateFin = ui->dateEdit_Fin_srv->date();

    // Vérifier si le champ coût est vide
    QString coutStr = ui->lineEdit_Cout_srv->text().trimmed().replace(",", ".");
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
    QString idText = ui->lineEdit_id_join->text().trimmed();
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
        ui->lineEdit_Nom_srv->clear();
        ui->textEdit_Description_srv->clear();
        ui->lineEdit_Cout_srv->clear();
        ui->comboBox_Statut_srv->setCurrentIndex(0);
        ui->comboBox_Frequence_srv->setCurrentIndex(0);
        ui->dateEdit_Debut_srv->setDate(QDate::currentDate());
        ui->dateEdit_Fin_srv->setDate(QDate::currentDate());
        ui->lineEdit_id_join->clear();

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du service !");
    }
}


void MainWindow::afficherServices() {
    Service service;
    QSqlQueryModel* model = service.afficher();

    if (model) {
        ui->tableView_srv->setModel(model);
        ui->tableView_srv->setStyleSheet(
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
    QItemSelectionModel *select = ui->tableView_srv->selectionModel();
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
    // Configuration du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "Liste_Services.pdf", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // Variables de mise en page
    int currentPage = 1;
    const int leftMargin = 40;
    const int topMargin = 100;
    const int rightMargin = 40;
    int yPosition = topMargin;

    // Style du document
    const QColor headerColor(58, 80, 107);   // Bleu foncé
    const QColor titleColor(40, 116, 166);   // Bleu moyen
    const QColor rowColor1(255, 255, 255);  // Blanc
    const QColor rowColor2(240, 245, 249);  // Bleu très clair
    const QColor borderColor(210, 210, 210); // Gris clair

    // Ajout du logo (remplacer par votre chemin)
    QPixmap logo(":/../img/logo_final-removebg-preview.png");
    if (!logo.isNull()) {
        logo = logo.scaledToWidth(70, Qt::SmoothTransformation);
        painter.drawPixmap(leftMargin, 20, logo);
    }

    // En-tête du document
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.setPen(titleColor);
    painter.drawText(QRect(leftMargin + 80, 30, pdfWriter.width() - leftMargin - rightMargin - 80, 40),
                     Qt::AlignLeft | Qt::AlignVCenter, "LISTE DES SERVICES");

    // Date et heure de génération
    painter.setFont(QFont("Arial", 7));
    painter.setPen(Qt::darkGray);
    QString generationDate = QDateTime::currentDateTime().toString("dd/MM/yyyy - HH:mm");
    painter.drawText(QRect(leftMargin + 80, 65, pdfWriter.width() - leftMargin - rightMargin - 80, 20),
                     Qt::AlignLeft | Qt::AlignVCenter, "Généré le " + generationDate);

    // Ligne de séparation
    painter.setPen(QPen(Qt::lightGray, 1));
    painter.drawLine(leftMargin, 90, pdfWriter.width() - rightMargin, 90);

    // Configuration des colonnes à afficher
    const QStringList columnsToShow = {"ID", "Nom", "Description", "Coût", "Fréquence", "Statut"};
    QMap<QString, int> columnWidths = {
        {"ID", 40},        // 40 points
        {"Nom", 100},      // 100 points
        {"Description", 200}, // 200 points
        {"Coût", 60},      // 60 points
        {"Fréquence", 80}, // 80 points
        {"Statut", 80}     // 80 points
    };

    // Ajustement automatique des largeurs
    int totalRequestedWidth = 0;
    for (int width : columnWidths) {
        totalRequestedWidth += width;
    }
    float scalingFactor = (pdfWriter.width() - leftMargin - rightMargin) / (float)totalRequestedWidth;
    for (auto& width : columnWidths) {
        width = qRound(width * scalingFactor);
    }

    // En-têtes de colonnes
    const int rowHeight = 32;
    QLinearGradient headerGradient(0, 0, 0, rowHeight);
    headerGradient.setColorAt(0, headerColor);
    headerGradient.setColorAt(1, headerColor.darker(110));

    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.setBrush(headerGradient);
    painter.setPen(QPen(borderColor, 0.7));

    int xPosition = leftMargin;
    for (const QString& column : columnsToShow) {
        QRect headerRect(xPosition, yPosition, columnWidths[column], rowHeight);
        painter.drawRect(headerRect);
        painter.setPen(Qt::white);
        painter.drawText(headerRect, Qt::AlignCenter, column);
        xPosition += columnWidths[column];
    }
    yPosition += rowHeight;

    // Remplissage des données
    painter.setFont(QFont("Arial", 8));
    QAbstractItemModel* model = ui->tableView_srv->model();

    for (int row = 0; row < model->rowCount(); ++row) {
        xPosition = leftMargin;
        QColor rowColor = (row % 2 == 0) ? rowColor1 : rowColor2;

        for (const QString& column : columnsToShow) {
            QRect cellRect(xPosition, yPosition, columnWidths[column], rowHeight);

            // Fond de la cellule
            painter.setPen(QPen(borderColor, 0.5));
            painter.setBrush(rowColor);
            painter.drawRect(cellRect);

            // Texte de la cellule
            int modelColumn = -1;
            for (int i = 0; i < model->columnCount(); ++i) {
                if (model->headerData(i, Qt::Horizontal).toString() == column) {
                    modelColumn = i;
                    break;
                }
            }

            if (modelColumn != -1) {
                QString cellText = model->data(model->index(row, modelColumn)).toString();
                painter.setPen(Qt::black);

                // Alignement différent pour les colonnes numériques
                Qt::Alignment alignment = (column == "ID" || column == "Coût")
                                              ? (Qt::AlignRight | Qt::AlignVCenter)
                                              : (Qt::AlignLeft | Qt::AlignVCenter);

                painter.drawText(cellRect.adjusted(5, 0, -5, 0), cellText, alignment);
            }

            xPosition += columnWidths[column];
        }

        yPosition += rowHeight;

        // Gestion des sauts de page
        if (yPosition > pdfWriter.height() - 50) {
            // Pied de page
            painter.setFont(QFont("Arial", 7));
            painter.setPen(Qt::gray);
            painter.drawText(QRect(leftMargin, pdfWriter.height() - 30,
                                   pdfWriter.width() - leftMargin - rightMargin, 20),
                             Qt::AlignRight, QString("Page %1").arg(currentPage++));

            // Nouvelle page
            pdfWriter.newPage();
            yPosition = 40;

            // Réafficher les en-têtes
            xPosition = leftMargin;
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            painter.setBrush(headerGradient);
            painter.setPen(QPen(borderColor, 0.7));

            for (const QString& column : columnsToShow) {
                QRect headerRect(xPosition, yPosition, columnWidths[column], rowHeight);
                painter.drawRect(headerRect);
                painter.setPen(Qt::white);
                painter.drawText(headerRect, Qt::AlignCenter, column);
                xPosition += columnWidths[column];
            }
            yPosition += rowHeight;

            painter.setFont(QFont("Arial", 8));
        }
    }

    // Pied de page final
    painter.setFont(QFont("Arial", 7));
    painter.setPen(Qt::gray);
    painter.drawText(QRect(leftMargin, pdfWriter.height() - 30,
                           pdfWriter.width() - leftMargin - rightMargin, 20),
                     Qt::AlignRight, QString("Page %1").arg(currentPage));

    painter.end();

    // Ouverture du PDF généré
    if (QFile::exists(filePath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "La génération du PDF a échoué.");
    }
}


void MainWindow::afficherStatistiquesServ() {
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
    QWidget *statistiquesTab = ui->tabWidget_srv->widget(2);



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
        QPixmap pixmap = ui->tabWidget_srv->widget(2)->grab();

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
    QList<QPushButton*> boutons = {ui->stat_srv, ui->form_srv, ui->liste_srv, ui->save_stat}; // Ajoute tous tes boutons ici

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
/*void MainWindow::setUpNavigationButtons()
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
}*/
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
void MainWindow::on_modifier_srv_clicked()
{
    QItemSelectionModel *select = ui->tableView_srv->selectionModel();
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
    ui->tabWidget_srv->setCurrentIndex(3);
    loadServiceDetails(idService);


}

void MainWindow::loadServiceDetails(int id)
{
    Service service;
    if (service.loadById(id)) {  // Charger les détails du service sélectionné
        ui->lineEdit_Nom_srv_mod->setText(service.getNom());
        ui->textEdit_Description_srv_mod->setPlainText(service.getDescription());
        ui->lineEdit_Cout_srv_mod->setText(QString::number(service.getCout()));
        ui->comboBox_Frequence_srv_mod->setCurrentText(service.getFrequence());
        ui->comboBox_Statut_srv_mod->setCurrentText(service.getStatut());
        ui->dateEdit_Debut_srv_mod->setDate(QDate::fromString(service.getDateDebut(), "DD-MM-YY"));
        ui->dateEdit_Fin_srv_mod->setDate(QDate::fromString(service.getDateFin(), "DD-MM-YY"));
        ui->lineEdit_id_join_mod->setText(QString::number(service.getIdEspace())); // Ajout de l'ID espace
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les détails du service.");
    }
}



void MainWindow::on_save_srv_clicked()
{
    Service service;
    QString nom = ui->lineEdit_Nom_srv_mod->text().trimmed();
    QString description = ui->textEdit_Description_srv_mod->toPlainText().trimmed();
    QString statut = ui->comboBox_Statut_srv_mod->currentText();
    QString frequence = ui->comboBox_Frequence_srv_mod->currentText();
    QDate dateDebut = ui->dateEdit_Debut_srv_mod->date();
    QDate dateFin = ui->dateEdit_Fin_srv_mod->date();

    // Récupérer et nettoyer le champ coût
    QString coutStr = ui->lineEdit_Cout_srv_mod->text().trimmed().replace(",", ".");
    coutStr.remove(QChar(0x200E)); // Supprimer les caractères invisibles

    // Récupérer et nettoyer l'ID espace
    QString idEspaceStr = ui->lineEdit_id_join_mod->text().trimmed();
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
        ui->tabWidget_srv->setCurrentIndex(0);
        afficherServices();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du service.");
    }
    QString nouveauStatut = ui->comboBox_Statut_srv_mod->currentText(); // Supposons que c'est un QComboBox



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
    int columnIndex = ui->tableView_srv->currentIndex().column();

    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return;
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView_srv->model());
    if (!proxyModel) {
        // Création du proxyModel si nécessaire
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_srv->model()); // Assurez-vous que `serviceModel` est le modèle de la table des services
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView_srv->setModel(proxyModel);
    }

    // Appliquer le tri sur la colonne sélectionnée
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    qDebug() << "Tri effectué sur la colonne : " << columnIndex;
}


void MainWindow::rechercherService(const QString &searchText) {
    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView_srv->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_srv->model());  // Assure-toi que `serviceModel` est le modèle principal
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView_srv->setModel(proxyModel);
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

    QString idStr = ui->lineEdit_id_join->text().trimmed().remove(QChar(0x200E)); // Nettoyage ID
    qDebug() << "ID Saisi après nettoyage:" << idStr;

    bool ok;
    int id_espace = idStr.toInt(&ok);
    qDebug() << "ID converti :" << id_espace << ", Conversion réussie ?" << ok;

    if (!ok || id_espace <= 0) {
        qDebug() << "ID invalide";
        ui->nom_espace_srv->setText("ID inexistant");
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
        ui->nom_espace_srv->setText(nomEspace);
    } else {
        qDebug() << "Aucun espace trouvé pour cet ID.";
        ui->nom_espace_srv->setText("ID inexistant");
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
            ui->nom_espace_srv_mod->setText(nomEspace);
        } else {
            qDebug() << "Aucun espace trouvé pour cet ID.";
            ui->nom_espace_srv_mod->setText("ID inexistant");
        }
    } else {
        qDebug() << "ID invalide";
        ui->nom_espace_srv_mod->setText("ID inexistant");
    }
}



//_______________________MALEK_________________________________







void MainWindow::on_ajouter_button_ev_clicked()
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


void MainWindow::on_pushButton_choisirImage_event_clicked()
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
        ui->tableView_ev->setModel(model);
        ui->tableView_ev->setStyleSheet(
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
    copyTableViewToTableWidget(ui->tableView_ev,tablewidget);

}


void MainWindow::on_supprimer_event_clicked()
{
    QModelIndex index = ui->tableView_ev->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un événement à supprimer.");
        return;
    }

    // Récupérer l'ID depuis la première colonne (colonne 0)
    int id = ui->tableView_ev->model()->data(ui->tableView_ev->model()->index(index.row(), 0)).toInt();

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
        ui->modif_titre_ev->setText(event.getTitre());
        ui->modif_type_ev->setText(event.getType());
        ui->modif_capacite_ev->setText(QString::number(event.getCapacite())); // ✅ CORRECT
        ui->modif_prix_ev->setValue(event.getPrix());
        ui->modif_description_ev->setPlainText(event.getDescription());
        ui->modif_dateDebut_ev->setDate(event.getDateDebut());
        ui->modif_dateFin_ev->setDate(event.getDateFin());
        ui->modif_categorie_ev->setText(event.getCategorie());
        ui->modif_statut_ev->setCurrentText(event.getStatut());
        ui->modif_organisateur_ev->setText(event.getOrganisateur());
        ui->modif_lieu_ev->setText(QString::number(event.getid_espace()));

        QString affichePath = event.getAffiche();
        if (!affichePath.isEmpty()) {
            QPixmap pixmap;
            if (pixmap.load(affichePath)) {
                ui->pushButton_affiche->setIcon(QIcon(pixmap));
                ui->pushButton_affiche->setIconSize(QSize(150, 150));            }
        }

        idEvenementAModifier = eventId;
        ui->tabWidget_event->setCurrentIndex(4); // aller à la page 5
    }
}

void MainWindow::on_pushButton_save_modif_ev_clicked()
{
    if (idEvenementAModifier == -1) return;

    Evenement evenement;
    int capacite = ui->modif_capacite_ev->text().toDouble();
    double prix = ui->modif_prix_ev->value();
    int lieu = ui->modif_lieu_ev->text().toInt();

    if (ui->modif_dateFin_ev->date() <= ui->modif_dateDebut_ev->date()) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        return;
    }

    if (evenement.modifier(idEvenementAModifier,
                           ui->modif_titre_ev->text(),
                           ui->modif_type_ev->text(),
                           capacite,
                           prix,
                           ui->modif_description_ev->toPlainText(),
                           ui->modif_dateDebut_ev->date().toString("dd-MM-yy"),
                           ui->modif_dateFin_ev->date().toString("dd-MM-yy"),
                           ui->modif_categorie_ev->text(),
                           ui->modif_statut_ev->currentText(),
                           ui->modif_organisateur_ev->text(),
                           lieu)) {
        ui->tabWidget_event->setCurrentIndex(1);
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
        } else {
            QMessageBox::warning(this, "Erreur", "Image invalide.");
        }
    }
}

void MainWindow::on_btnUpdate_ev_clicked()
{
    QItemSelectionModel *select = ui->tableView_ev->selectionModel();
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






void MainWindow::afficherStatistiquesEvent() {
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
    QWidget *statistiquesTab = ui->tabWidget_event->widget(2);

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
    int columnIndex = ui->tableView_ev->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    //2. Vérifier si aucune colonne n’est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return; // Arrêter la fonction si aucune colonne n'est sélectionnée
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    //Créer un QSortFilterProxyModel si besoin
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->tableView_ev->model());
    if (!proxyModel) {
        // Si aucun proxyModel n'existe, on en crée un
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_ev->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView_ev->setModel(proxyModel); // Appliquer le proxyModel à tableView
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
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView_ev->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_ev->model());  // Assure-toi que serviceModel est le modèle principal
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterRole(Qt::DisplayRole);
        ui->tableView_ev->setModel(proxyModel);
    }

    // Obtenir les index des colonnes visibles ID et Titre
    int colId = ui->tableView_ev->horizontalHeader()->logicalIndex(0);
    int colTitre = ui->tableView_ev->horizontalHeader()->logicalIndex(1);

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
    ui->titrel_4->clear();
    ui->desl_4->clear();
    ui->ddl_4->clear();
    ui->dfl_4->clear();
    ui->capl_4->clear();
    ui->prixl_4->clear();
    ui->catl_4->clear();
    ui->typl_4->clear();
    ui->orgl_4->clear();
    ui->lieul_4->clear();

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
                ui->ddl_4->setDate(date);
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
                ui->titrel_4->setText(query.value("TITRE").toString());
                ui->desl_4->setPlainText(query.value("DESCRIPTION").toString());
                ui->ddl_4->setDate(query.value("DATE_DEB").toDate());
                ui->dfl_4->setDate(query.value("DATE_FIN").toDate());
                ui->capl_4->setText(query.value("CAPACITE").toString());
                ui->prixl_4->setValue(query.value("PRIX").toDouble());
                ui->statusl_4->setCurrentText(query.value("STATUT").toString());
                ui->catl_4->setText(query.value("CATEGORIE").toString());
                ui->typl_4->setText(query.value("TYPE").toString());
                ui->orgl_4->setText(query.value("ORGANISATEUR").toString());
                ui->lieul_4->setText(query.value("ID_ESPACE").toString());
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
    QString titre = ui->titrel_4->text();
    QString description = ui->desl_4->toPlainText(); // Utiliser toPlainText() pour QTextEdit
    QDate dateDebut = ui->ddl_4->date();
    QDate dateFin = ui->dfl_4->date();
    int capacite = ui->capl_4->text().toInt(); // Utiliser text() et convertir en entier
    double prix = ui->prixl_4->value();
    QString statut = ui->statusl_4->currentText();
    QString categorie = ui->catl_4->text(); // Utiliser text() pour QLineEdit
    QString type = ui->typl_4->text();  // Utiliser text() pour QLineEdit
    QString organisateur = ui->orgl_4->text();
    int id_espace = ui->lieul_4->text().toInt();


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
    ui->tabWidget_event->setCurrentIndex(5);
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
    todoModelEvent->appendRow(rowItems);

    ui->write_todo->clear();
}

void MainWindow::on_table_todo_ev_clicked(const QModelIndex &index)
{
    if (index.column() == 0) {
        QStandardItem *checkItem = todoModelEvent->item(index.row(), 0);
        QStandardItem *statusItem = todoModelEvent->item(index.row(), 2);

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




void MainWindow::on_ticket_clicked()
{
    QModelIndex currentIndex = ui->tableView_ev->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un événement.");
        return;
    }

    int row = currentIndex.row();
    QAbstractItemModel* model = ui->tableView_ev->model();

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
    QModelIndexList selectedRows = ui->tableView_ev->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un événement dans la table.");
        return;
    }

    int selectedRow = selectedRows.first().row();

    // Récupérer les données de la ligne sélectionnée - CORRECTION: colonne 0 pour le TITRE
    QString titre = ui->tableView_ev->model()->data(ui->tableView_ev->model()->index(selectedRow, 1)).toString(); // colonne 1 : TITRE

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




//________________________Ahmed___________________





//Ajout
void MainWindow::validerChamp(QLineEdit* champ, QLabel* labelErreur, const QString& message, bool estValide) {
    if (estValide) {
        champ->setStyleSheet("border: 2px solid green;");
        labelErreur->setStyleSheet("color: green;");
        labelErreur->setText(message);
    } else {
        champ->setStyleSheet("border: 2px solid red;");
        labelErreur->setStyleSheet("color: red;");
        labelErreur->setText(message);
    }
}

void MainWindow::validerChamp2(QWidget* champ, QLabel* labelErreur, const QString& message, bool estValide) {
    if (estValide) {
        champ->setStyleSheet("border: 2px solid green;");
        labelErreur->setStyleSheet("color: green;");
    } else {
        champ->setStyleSheet("border: 2px solid red;");
        labelErreur->setStyleSheet("color: red;");
    }
    labelErreur->setText(message);
}


void MainWindow::on_ajouter_maintenance_clicked() {
    qDebug() << "Tentative d'ajout d'une maintenance...";

    ui->er_description->clear();
    ui->er_cout->clear();
    ui->er_id_employee->clear();
    ui->er_id_espace->clear();

    QString type = ui->type_maintenance->currentText();
    QString description = ui->description_maintenance->toPlainText();
    QString statut = ui->statut_maintenance->currentText();
    QDate date_resolution = ui->date_resolution->date();
    QString id_espace = ui->id_espace_maintenance->text();
    QString coutStr = ui->cout_maintenance->text();
    QString id_employe = ui->id_employe_maintenance->text();

    bool valid = true;
    bool ok;

    if (description.isEmpty()) {
        ui->er_description->setText("La description ne peut pas être vide.");
        valid = false;
    }

    double cout = coutStr.toDouble(&ok);
    if (!ok) {
        ui->er_cout->setText("Le coût doit être un nombre valide.");
        valid = false;
    }

    int id_espace1 = id_espace.toInt(&ok);
    if (!ok || !Maintenance::espaceExiste(id_espace1)) {
        ui->er_id_espace->setText("L'ID espace n'existe pas.");
        valid = false;
    }

    if (!valid) {
        QMessageBox::critical(this, "Erreur", "Veuillez corriger les champs invalides avant d'ajouter la maintenance !");
        return;
    }

    int employeAutoId = -1;
    QString employeNom, employeEmail, employeTel;

    bool estAjoutManuel = !id_employe.isEmpty();

    if (estAjoutManuel) {
        // 👤 Mode manuel
        int id_employe_int = id_employe.toInt(&ok);
        if (!ok || !Maintenance::employeeExiste(id_employe_int)) {
            ui->er_id_employee->setText("L'ID employé n'existe pas.");
            return;
        }

        QMap<QString, QString> employeData = Maintenance::getEmployeById(id_employe_int);
        employeAutoId = id_employe_int;
        employeNom = employeData["nom"] + " " + employeData["prenom"];
        employeEmail = employeData["email"];
        employeTel = employeData["telephone"];

    } else {
        // 🤖 Mode automatique via analyse IA
        QString analyseNettoyee = normaliserTexte(analyseMaintenanceResult);
        static const QRegularExpression regex("\\b(electricien|plombier|technicien|informaticien|menuisier|nettoyeur)\\b");
        QRegularExpressionMatch match = regex.match(analyseNettoyee);

        if (!match.hasMatch()) {
            QMessageBox::warning(this, "Affectation", "Aucun métier reconnu dans l'analyse IA !");
            return;
        }

        QString metier = match.captured(1);
        qDebug() << "✅ Métier extrait de l’analyse (nettoyée) :" << metier;

        QMap<QString, QString> employeData = Maintenance::getEmployeInactifByPoste(metier);
        if (employeData.isEmpty()) {
            QMessageBox::warning(this, "Affectation", "Aucun employé inactif trouvé avec le poste : " + metier);
            return;
        }

        employeAutoId = employeData["id"].toInt();
        employeNom = employeData["nom"] + " " + employeData["prenom"];
        employeEmail = employeData["email"];
        employeTel = employeData["telephone"];

        Maintenance::updateEmployeStatut(employeAutoId, "actif");
    }

    // 🛠 Création de la maintenance avec l’employé sélectionné
    Maintenance maintenance(type, description, cout, statut, date_resolution.toString("yyyy-MM-dd"), employeAutoId, id_espace1);
    maintenance.setAnalyse(analyseMaintenanceResult);

    if (maintenance.ajouter()) {
        QMessageBox::information(this, "Succès", "Maintenance ajoutée et affectée à : " + employeNom);

        // ✉️ Envoi de mail via MailJet
        MailJetSender *sender = new MailJetSender(this);
        sender->envoyerMail(
            "SMM",
            "ayadia819@gmail.com",
            employeEmail,
            "Nouvelle maintenance",
            "Bonjour " + employeNom + ",\n"
                                      "Vous avez été affecté à une nouvelle maintenance.\n\n"
                                      "Type: " + type + "\n"
                         "Description: " + description + "\n"
                                "Espace: " + id_espace + "\n"
                              "Analyse IA:\n" + analyseMaintenanceResult
            );

        // 📲 Envoi SMS
        QString telephone = employeTel;
        if (!telephone.startsWith("+")) {
            telephone = "+216" + telephone;
        }

        QString smsText = "Maintenance affectée à vous :\n"
                          "Type: " + type + "\n"
                                   "Description: " + description + "\n"
                                          "Espace ID: " + id_espace + "\n"
                                        "Analyse: " + analyseMaintenanceResult.left(140); // pour éviter dépassement

        sendSMS(telephone, smsText);

        ui->tabWidget_3->setCurrentIndex(1);
        afficher_maintenance();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la maintenance !");
    }
}

void MainWindow::afficher_maintenance()
{
    Maintenance m;
    QSqlQueryModel* model = m.afficher();

    if (model) {
        ui->tab_maintenance->setModel(model);
        ui->tab_maintenance->setColumnHidden(8, true);  // Colonne "analyse" (index 8)
        ui->tab_maintenance->setStyleSheet(
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
        ui->tab_maintenance->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des maintenances.");
    }
}

void MainWindow::afficherDescriptionComplete(const QModelIndex &index)
{
    if (index.column() != 2) return; // Vérifie si c'est la colonne description

    QString id = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toString();

    QSqlQuery query;
    query.prepare("SELECT description FROM maintenance WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
        qDebug() << "Erreur lors de la récupération de la description:" << query.lastError().text();
        return;
    }

    QString fullDesc = query.value(0).toString();

    // Création de la fenêtre
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Description complète");
    dialog->setMinimumSize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QTextEdit *text = new QTextEdit();
    text->setText(fullDesc);
    text->setReadOnly(true);
    text->setStyleSheet("font: 12pt; padding: 10px;");
    layout->addWidget(text);

    QPushButton *fermerButton = new QPushButton("Fermer");
    connect(fermerButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(fermerButton);

    dialog->exec();
    delete dialog;
}


void MainWindow::afficherDetailsEmployeMain(const QModelIndex &index)
{
    // Vérifier si on a cliqué sur la bonne colonne (ajuste si nécessaire)
    if (index.column() != 6) return;

    // Récupérer l'ID de l'employé depuis la cellule
    int employeId = index.data().toInt();
    if (employeId <= 0) {
        qDebug() << "ID employé invalide";
        return;
    }

    // Requête SQL pour récupérer les informations de l'employé
    QSqlQuery query;
    query.prepare("SELECT id, nom, prenom, poste, salaire, email, telephone, pdp FROM employe WHERE id = :id");
    query.bindValue(":id", employeId);

    if (!query.exec() || !query.next()) {
        qDebug() << "Erreur récupération employé:" << query.lastError().text();
        return;
    }

    // Récupérer les données de l'employé
    QVariantMap employee;
    employee["ID"] = query.value("id");
    employee["Nom"] = query.value("nom");
    employee["Prénom"] = query.value("prenom");
    employee["Poste"] = query.value("poste");
    employee["Salaire"] = query.value("salaire").toDouble();
    employee["Email"] = query.value("email");
    employee["Téléphone"] = query.value("telephone");
    QByteArray photoData = query.value("pdp").toByteArray();

    qDebug() << "Données employé:" << employee; // Pour débogage

    // Création de la fenêtre de dialogue
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Détails Employé"));
    dialog->setFixedSize(400, 700);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Photo (centrée)
    QLabel *photoLabel = new QLabel();
    QPixmap photo;
    if (!photoData.isEmpty() && photo.loadFromData(photoData)) {
        photo = photo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        photo.load(":/img/default-user.png");
        photo = photo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    photoLabel->setPixmap(photo);
    photoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(photoLabel);

    // Informations employé
    QFrame *infoFrame = new QFrame();
    QFormLayout *formLayout = new QFormLayout(infoFrame);
    formLayout->setSpacing(10);

    QStringList fields = { "ID", "Nom", "Prénom", "Poste", "Salaire", "Email", "Téléphone" };
    for (const QString &field : fields) {
        QLabel *label = new QLabel(employee[field].toString());
        label->setStyleSheet("font: 12pt; padding: 5px;");

        QLabel *title = new QLabel(field + ":");
        title->setStyleSheet("font-weight: bold; font: 12pt; color: #2c3e50;");

        formLayout->addRow(title, label);
    }

    infoFrame->setStyleSheet("background: #f8f9fa; border-radius: 10px; padding: 15px;");
    layout->addWidget(infoFrame);
    layout->setContentsMargins(20, 20, 20, 20);

    // Bouton de fermeture
    QPushButton *closeButton = new QPushButton(tr("Fermer"));
    closeButton->setStyleSheet("background-color: #2c3e50; color: white; padding: 8px; border-radius: 5px;");
    QObject::connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Affichage du dialogue
    dialog->exec();
}

void MainWindow::afficherDetailsEspace(const QModelIndex &index)
{
    if (index.column() != 7) return; // Vérifier si on clique sur la colonne id_espace

    QString id_espace = index.data().toString();
    if (id_espace.isEmpty()) {
        qDebug() << "Aucun ID espace trouvé";
        return;
    }

    // Exécuter la requête SQL pour récupérer les détails de l'espace
    QSqlQuery query;
    query.prepare("SELECT * FROM espace WHERE id = :id_espace");
    query.bindValue(":id_espace", id_espace);

    if (!query.exec() || !query.next()) {
        qDebug() << "Erreur lors de la récupération des données de l'espace :" << query.lastError().text();
        return;
    }

    // Création de la fenêtre
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Détails Espace");
    dialog->setFixedSize(400, 700);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Informations sous forme de formulaire
    QFrame *infoFrame = new QFrame();
    QFormLayout *formLayout = new QFormLayout(infoFrame);
    formLayout->setSpacing(10);

    QStringList fields = {
        "ID", "Nom", "Type", "Superficie",
        "Emplacement", "Statut", "Loyer Mensuel",
        "Date Début Contrat", "ID Locataire"
    };

    QStringList dbFields = {
        "id", "nom", "type", "superficie",
        "emplacement", "statut", "loyer_m",
        "date_deb_contrat", "id_locataire"
    };

    for (int i = 0; i < fields.size(); i++) {
        QLabel *label = new QLabel(query.value(dbFields[i]).toString());
        label->setStyleSheet("font: 12pt; padding: 5px;");

        QLabel *title = new QLabel(fields[i] + ":");
        title->setStyleSheet("font-weight: bold; font: 12pt; color: #2c3e50;");

        formLayout->addRow(title, label);
    }

    infoFrame->setStyleSheet("background: #f8f9fa; border-radius: 10px; padding: 15px;");
    layout->addWidget(infoFrame);
    layout->setContentsMargins(20, 20, 20, 20);

    // Bouton de fermeture
    QPushButton *fermerButton = new QPushButton("Fermer");
    connect(fermerButton, &QPushButton::clicked, dialog, &QDialog::close);
    layout->addWidget(fermerButton);

    dialog->exec();
    delete dialog;
}

void MainWindow::handleTableClick(const QModelIndex &index)
{
    if (index.column() == 7) { // Colonne id_espace
        afficherDetailsEspace(index);
    }
}






void MainWindow::on_tabWidget_3_tabBarClicked(int index)
{
    afficher_maintenance();
}

void MainWindow::on_supprimer_maintenance_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner une maintenance à supprimer.");
        return;
    }

    int id = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cette maintenance ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Maintenance maintenance;
        if (maintenance.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Maintenance supprimée avec succès.");
            afficher_maintenance(); // Met à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}

void MainWindow::on_modifier_maintenance_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Modification", "Veuillez sélectionner une maintenance à modifier.");
        return;
    }
    int id = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toInt();
    Maintenance maintenance;
    maintenance.afficherDetails(id, ui);
}

void MainWindow::on_update_maintenance_clicked() {
    int id = ui->up_id->text().toInt();
    QString type = ui->up_type->currentText();
    QString description = ui->up_description->text();
    QString statut = ui->up_statut_maintenance->currentText();
    QDate date_resolution = ui->up_date_resolution->date();
    QString id_employe = ui->up_id_employe_maintenance->text();
    QString id_espace = ui->up_id_espace_maintenance->text();
    QString coutStr = ui->up_cout->text();

    bool valid = true;
    if (description.isEmpty()) {
        ui->er_description_up->setText("La description ne peut pas être vide.");
        valid = false;
    }
    bool ok;
    double cout = coutStr.toDouble(&ok);
    if (!ok) {
        ui->er_cout_up->setText("Le coût doit être un nombre valide.");
        valid = false;
    }
    int id_employe1 = id_employe.toInt(&ok);
    if (!ok || !Maintenance::employeeExiste(id_employe1)) {
        ui->er_id_employee_up->setText("L'ID employé n'existe pas.");
        valid = false;
    }
    int id_espace1 = id_espace.toInt(&ok);
    if (!ok || !Maintenance::espaceExiste(id_espace1)) {
        ui->er_id_espace_up->setText("L'ID espace n'existe pas.");
        valid = false;
    }
    if (!valid) return;
    Maintenance maintenance;
    if (maintenance.update(id, type, description, cout, statut, date_resolution.toString("yyyy-MM-dd"), id_employe1, id_espace1)) {
        QMessageBox::information(this, "Succès", "Maintenance mise à jour avec succès.");
        ui->tabWidget_3->setCurrentIndex(1);
        afficher_maintenance();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour.");
    }
}



void MainWindow::on_liste_maintenance_clicked()
{
    afficher_maintenance();
}


void MainWindow::on_ajouter_maintenance_2_clicked()
{
    afficher_maintenance();
}


void MainWindow::on_exporter_maintenance_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "Erreur", "Impossible d’écrire dans le fichier PDF.");
        return;
    }

    // 🎨 Définition des styles
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont contentFont("Arial", 10);

    QColor headerBackground(44, 62, 80); // Bleu foncé
    QColor evenRowColor(245, 245, 245); // Gris clair

    int marginLeft = 0, marginTop = 190;
    int colWidth = 290, rowHeight = 70;
    int spacing = 10; // 🆕 Espacement entre les colonnes et lignes
    int pageWidth = writer.width(), pageHeight = writer.height();

    // 📌 Dessiner le titre
    painter.setFont(titleFont);
    painter.drawText(QRect(marginLeft, 30, pageWidth - 2 * marginLeft, 120), Qt::AlignCenter, "📋 Liste des Maintenances");

    // 📅 Ajouter la date
    painter.setFont(contentFont);
    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(pageWidth - 250, marginTop + 30, "📅 " + date);

    // 🔍 Récupération des données
    QTableView* table = ui->tab_maintenance;
    QAbstractItemModel* model = table->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }

    // 🏷️ Dessiner l’en-tête du tableau
    painter.setFont(headerFont);
    painter.setBrush(headerBackground);
    painter.setPen(Qt::white);

    int x = marginLeft, y = marginTop + 80;
    for (int column = 0; column < model->columnCount(); column++) {
        QString header = model->headerData(column, Qt::Horizontal).toString();
        QRect cellRect(x, y, colWidth, rowHeight);
        painter.fillRect(cellRect, headerBackground);
        painter.drawText(cellRect.adjusted(spacing, 0, -spacing, 0), Qt::AlignCenter, header);
        x += colWidth + spacing; // 🆕 Ajout du spacing entre colonnes
    }

    // 📊 Dessiner les données
    painter.setFont(contentFont);
    y += rowHeight + spacing; // 🆕 Ajout du spacing entre l'en-tête et les données

    int pageNumber = 1;
    for (int row = 0; row < model->rowCount(); ++row) {
        x = marginLeft;

        // Alternance des couleurs de ligne
        if (row % 2 == 0)
            painter.setBrush(evenRowColor);
        else
            painter.setBrush(evenRowColor);

        for (int column = 0; column < model->columnCount(); ++column) {
            QString data = model->data(model->index(row, column)).toString();
            QRect cellRect(x, y, colWidth, rowHeight);
            painter.fillRect(cellRect, painter.brush());
            painter.setPen(Qt::black);
            painter.drawText(cellRect.adjusted(spacing, 0, -spacing, 0), Qt::AlignCenter, data);
            x += colWidth + spacing; // 🆕 Ajout du spacing entre colonnes
        }
        y += rowHeight + spacing; // 🆕 Ajout du spacing entre lignes

        // 🆕 Nouvelle page si nécessaire
        if (y > pageHeight - 100) {
            writer.newPage();
            pageNumber++;
            y = marginTop + 80;
        }
    }

    // 📌 Pied de page avec numéro
    painter.drawText(pageWidth / 2 - 40, pageHeight - 40, QString("📄 Page %1").arg(pageNumber));

    painter.end();
    QMessageBox::information(this, "Succès", "✅ PDF Exporté avec succès !");
}

//recherche




void MainWindow::rechercherMaintenance(const QString &searchText) {
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tab_maintenance->model());

    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tab_maintenance->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tab_maintenance->setModel(proxyModel);
    }

    // Activer le filtre sur toutes les colonnes
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->setFilterRegularExpression(QRegularExpression(searchText, QRegularExpression::CaseInsensitiveOption));
}




//stat

void MainWindow::afficherStatistiquesMaintenance() {
    // 🟢 Accéder à l'onglet Statistiques (Page 4 dans tabWidget_3)
    QWidget *statistiquesTab = ui->tabWidget_3->widget(3);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(statistiquesTab->layout());

    // Si le layout n'existe pas, on le crée
    if (!layout) {
        layout = new QVBoxLayout(statistiquesTab);
        statistiquesTab->setLayout(layout);
    }

    // Supprimer tous les widgets existants dans le layout
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Créer un QSplitter horizontal
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    layout->addWidget(splitter);

    // 🔄 Récupérer les données depuis la classe Maintenance
    Maintenance m;
    QMap<QString, double> couts = m.getCoutsParTypeEtStatut();

    double instTraite    = couts["Instantané Traité"];
    double instNonTraite = couts["Instantané Non Traité"];
    double predTraite    = couts["Prédictive Traité"];
    double predNonTraite = couts["Prédictive Non Traité"];

    // ======================== Histogramme ========================
    QBarSeries *barSeries = new QBarSeries();
    QStringList categories = {"Instantané Traité", "Instantané Non Traité", "Prédictive Traité", "Prédictive Non Traité"};

    QBarSet *set = new QBarSet("Coût");
    *set << instTraite << instNonTraite << predTraite << predNonTraite;
    set->setColor(QColor::fromHsv(220, 255, 255)); // Couleur bleue

    barSeries->append(set);

    QChart *chart1 = new QChart();
    chart1->addSeries(barSeries);
    chart1->setTitle("Coût par Type et Statut");
    chart1->setAnimationOptions(QChart::SeriesAnimations);
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignBottom);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart1->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Coût (en DT)");
    chart1->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setRenderHint(QPainter::Antialiasing);

    QWidget *barChartWidget = new QWidget();
    QVBoxLayout *barLayout = new QVBoxLayout(barChartWidget);
    barLayout->addWidget(chartView1);
    splitter->addWidget(barChartWidget);

    // ======================== Diagramme Circulaire ========================
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Instantané Traité", instTraite);
    pieSeries->append("Instantané Non Traité", instNonTraite);
    pieSeries->append("Prédictive Traité", predTraite);
    pieSeries->append("Prédictive Non Traité", predNonTraite);

    QPieSlice *s1 = pieSeries->slices().at(0);
    s1->setBrush(Qt::blue);
    s1->setLabel(QString("Instantané Traité: %1 DT").arg(instTraite));
    s1->setLabelVisible(true);

    QPieSlice *s2 = pieSeries->slices().at(1);
    s2->setBrush(Qt::cyan);
    s2->setLabel(QString("Instantané Non Traité: %1 DT").arg(instNonTraite));
    s2->setLabelVisible(true);

    QPieSlice *s3 = pieSeries->slices().at(2);
    s3->setBrush(Qt::green);
    s3->setLabel(QString("Prédictive Traité: %1 DT").arg(predTraite));
    s3->setLabelVisible(true);

    QPieSlice *s4 = pieSeries->slices().at(3);
    s4->setBrush(Qt::yellow);
    s4->setLabel(QString("Prédictive Non Traité: %1 DT").arg(predNonTraite));
    s4->setLabelVisible(true);

    QChart *chart2 = new QChart();
    chart2->addSeries(pieSeries);
    chart2->setTitle("Répartition des coûts par Type et Statut");
    chart2->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);

    QWidget *pieChartWidget = new QWidget();
    QVBoxLayout *pieLayout = new QVBoxLayout(pieChartWidget);
    pieLayout->addWidget(chartView2);
    splitter->addWidget(pieChartWidget);

    splitter->setSizes(QList<int>() << 400 << 400);
}



void MainWindow::on_stat_maintenance_clicked()
{
    // Appeler la fonction pour afficher les statistiques avec animation
    afficherStatistiquesMaintenance();
}


//tri
void MainWindow::trierMaintenance()
{
    // Récupérer l'index de la colonne sélectionnée dans tab_maintenance
    int columnIndex = ui->tab_maintenance->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez sélectionner une colonne à trier !");
        return;
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxy model existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->tab_maintenance->model());

    if (!proxyModel) {
        // Créer un nouveau proxy model si besoin
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tab_maintenance->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tab_maintenance->setModel(proxyModel);
    }

    // Appliquer le tri sur la colonne sélectionnée
    proxyModel->sort(columnIndex, Qt::AscendingOrder);

    qDebug() << "Tri effectué sur la colonne : " << columnIndex;
}


//analyse
void MainWindow::on_analyse_maintenance_clicked() {
    QString description = ui->description_maintenance->toPlainText();
    if (description.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "La description est vide !");
        return;
    }

    OpenAIHelper *openai = new OpenAIHelper(this);
    connect(openai, &OpenAIHelper::analysisReady, this, [=](const QString &result) {
        analyseMaintenanceResult = result;  // ✅ stocke l’analyse dans la variable

        // Générer le PDF
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer l'analyse", "", "PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            QPainter painter(&writer);
            painter.setFont(QFont("Arial", 12));
            painter.drawText(QRect(100, 100, writer.width() - 200, writer.height() - 200), Qt::TextWordWrap, result);
            painter.end();
        }

        QMessageBox::information(this, "Analyse prête", "L'analyse est prête et enregistrée. Vous pouvez maintenant ajouter la maintenance.");
    });

    openai->analyserMaintenance(description);
}

QString MainWindow::nettoyerTexte(const QString& texte) {
    QString t = texte.normalized(QString::NormalizationForm_D);
    t.remove(QRegularExpression("[\\u0300-\\u036f]")); // Enlève les accents
    return t.toLower().trimmed();
}

QString MainWindow::normaliserTexte(const QString &texte) {
    QString result = texte.normalized(QString::NormalizationForm_D);
    result.remove(QRegularExpression(R"([\u0300-\u036f])"));  // Supprime accents
    result.replace(QRegularExpression("[^a-zA-Z0-9\\s]"), ""); // Supprime caractères bizarres
    return result.toLower().trimmed();
}

//SMS
void MainWindow::sendSMS(const QString& phoneNumber, const QString& message) {


    QUrl url(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSID));
    QNetworkRequest request(url);

    QString credentials = QString("%1:%2").arg(accountSID).arg(authToken);
    QByteArray authData = credentials.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", "Basic " + authData);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString formattedPhone = phoneNumber;
    if (!formattedPhone.startsWith("+"))
        formattedPhone = "+216" + formattedPhone.mid(1); // 🇹🇳

    QUrlQuery params;
    params.addQueryItem("To", formattedPhone);
    params.addQueryItem("From", twilioNumber);
    params.addQueryItem("Body", message);

    QNetworkReply* reply = manager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "✅ SMS envoyé avec succès:" << reply->readAll();
        } else {
            qDebug() << "❌ Échec SMS:" << reply->errorString();
            qDebug() << "Réponse Twilio:" << reply->readAll();
        }
        reply->deleteLater();
    });
}

//badge

void MainWindow::on_badge_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne dans le tableau.");
        return;
    }

    int row = index.row();
    QString id_maintenance = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(row, 0)).toString();

    QSqlQuery query;
    query.prepare(R"(
        SELECT m.id, m.date_resolution, e.nom AS espace_nom,
               emp.nom, emp.prenom, emp.poste, emp.pdp
        FROM maintenance m
        JOIN employe emp ON m.id_employe = emp.id
        JOIN espace e ON m.id_espace = e.id
        WHERE m.id = :id
    )");
    query.bindValue(":id", id_maintenance);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données pour le badge.");
        return;
    }

    QString date_resolution = query.value("date_resolution").toString();
    QString espace_nom = query.value("espace_nom").toString();
    QString nom = query.value("nom").toString();
    QString prenom = query.value("prenom").toString();
    QString poste = query.value("poste").toString();
    QByteArray photoData = query.value("pdp").toByteArray();

    QImage photo;
    photo.loadFromData(photoData, "JPEG");
    if (photo.isNull()) {
        QMessageBox::warning(this, "Photo", "Erreur de lecture de la photo (BLOB)");
        return;
    }

    // QR code simulé : image avec texte
    QString qrText = QString(
                         "Nom: %1\n"
                         "Prénom: %2\n"
                         "Poste: %3\n"
                         "Maintenance ID: %4\n"
                         "Espace: %5\n"
                         "Date: %6"
                         ).arg(nom, prenom, poste, id_maintenance, espace_nom, date_resolution);

    QImage qrImg(200, 200, QImage::Format_ARGB32);
    qrImg.fill(Qt::white);
    QPainter pqr(&qrImg);
    pqr.setPen(Qt::black);
    pqr.setFont(QFont("Courier", 8));
    pqr.drawText(qrImg.rect(), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, qrText);
    pqr.end();

    // Choix fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le badge", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    // Créer PDF : 85mm x 54mm à 300 dpi = 1000x637 approx
    QPdfWriter writer(fileName);
    writer.setResolution(300);
    //writer.setPageSizeMM(QSizeF(85, 54));
    writer.setPageOrientation(QPageLayout::Portrait);

    QPainter painter(&writer);
    painter.fillRect(writer.pageLayout().fullRectPoints(), QColor("#ffffff"));

    // Photo centrée en haut
    QRect photoRect((writer.width() - 200) / 2, 20, 200, 200);
    painter.drawImage(photoRect, photo.scaled(photoRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Infos
    int textStartY = photoRect.bottom() + 20;
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12));
    int lineHeight = 30;
    int xLeft = 30;
    painter.drawText(xLeft, textStartY, "Nom : " + nom);               textStartY += lineHeight;
    painter.drawText(xLeft, textStartY, "Prénom : " + prenom);        textStartY += lineHeight;
    painter.drawText(xLeft, textStartY, "Poste : " + poste);          textStartY += lineHeight;
    painter.drawText(xLeft, textStartY, "ID : " + id_maintenance);    textStartY += lineHeight;
    painter.drawText(xLeft, textStartY, "Espace : " + espace_nom);    textStartY += lineHeight;
    painter.drawText(xLeft, textStartY, "Date : " + date_resolution);

    // QR code en bas à droite
    QRect qrRect(writer.width() - 180, writer.height() - 180, 150, 150);
    painter.drawImage(qrRect, qrImg);

    painter.end();

    QMessageBox::information(this, "Succès", "✅ Badge PDF 85x54mm généré avec succès !");
}


//badge
void MainWindow::on_badge_maintenance_clicked() {
    QModelIndex index = ui->tab_maintenance->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne dans le tableau.");
        return;
    }

    QString id_maintenance = ui->tab_maintenance->model()->data(ui->tab_maintenance->model()->index(index.row(), 0)).toString();

    QSqlQuery query;
    query.prepare(R"(
        SELECT m.id, m.id_employe, m.type, m.description, m.date_resolution,
               emp.nom, emp.prenom, emp.poste, emp.pdp
        FROM maintenance m
        JOIN employe emp ON m.id_employe = emp.id
        WHERE m.id = :id
    )");
    query.bindValue(":id", id_maintenance);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Erreur", "Données introuvables pour cette maintenance.");
        return;
    }

    QString id_employe = query.value("id_employe").toString();
    QString type = query.value("type").toString();
    QString description = query.value("description").toString();
    QString date_res = query.value("date_resolution").toString();
    QString nom = query.value("nom").toString();
    QString prenom = query.value("prenom").toString();
    QString poste = query.value("poste").toString();
    QByteArray photoData = query.value("pdp").toByteArray();

    // Charger la photo
    QImage photo;
    photo.loadFromData(photoData, "JPEG");

    // Construire le contenu du QR code
    QString qrText = QString(
                         "Maintenance ID: %1\n"
                         "Nom: %2\n"
                         "Prénom: %3\n"
                         "Poste: %4\n"
                         "Type: %5\n"
                         "Date: %6\n"
                         "Description: %7"
                         ).arg(id_maintenance, nom, prenom, poste, type, date_res, description);

    // QR code simulé
    QImage qrImg(150, 150, QImage::Format_ARGB32);
    qrImg.fill(Qt::white);
    QPainter qrp(&qrImg);
    qrp.setPen(Qt::black);
    qrp.setFont(QFont("Courier", 8));
    qrp.drawText(qrImg.rect(), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, qrText);
    qrp.end();

    // Sauvegarde PDF
    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer le badge", "", "PDF (*.pdf)");
    if (filename.isEmpty()) return;

    QPdfWriter writer(filename);
    writer.setResolution(300);
    writer.setPageSize(QPageSize(QSize(1000, 600), QPageSize::Point));
    writer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // 🎨 En-tête violet
    QRect headerRect(0, 0, writer.width(), 80);
    painter.fillRect(headerRect, QColor("#2E247C"));
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(headerRect, Qt::AlignCenter, "IDENTIFICATION EMPLOYÉ");

    // 📸 Photo
    QRect photoRect(40, 100, 180, 180);
    painter.drawImage(photoRect, photo.scaled(photoRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Infos à droite
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    int infoX = 250, infoY = 110, lh = 35;

    painter.drawText(infoX, infoY, "N° D’IDENTIFICATION");
    painter.setFont(QFont("Arial", 11));
    painter.drawText(infoX, infoY + 20, id_employe);

    infoY += lh;
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.drawText(infoX, infoY, "NOM");
    painter.setFont(QFont("Arial", 11));
    painter.drawText(infoX, infoY + 20, nom + " " + prenom);

    infoY += lh;
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.drawText(infoX, infoY, "POSTE / EMPLOI");
    painter.setFont(QFont("Arial", 11));
    painter.drawText(infoX, infoY + 20, poste);

    // 🔳 QR code à droite
    QRect qrRect(writer.width() - 190, 120, 150, 150);
    painter.drawImage(qrRect, qrImg);

    // Footer violet
    QRect footerRect(0, writer.height() - 70, writer.width(), 70);
    painter.fillRect(footerRect, QColor("#A59EDC"));
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.setPen(Qt::black);
    painter.drawText(footerRect, Qt::AlignCenter, "SMM");

    painter.end();

    QMessageBox::information(this, "Succès", "✅ Badge généré avec succès !");
}



//_________________insaf_______________


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
        ui->tableView_3->setModel(model);
        ui->tableView_3->setStyleSheet(
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
        ui->tableView_3->resizeColumnsToContents();
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

void MainWindow::on_id_locataire_entered()
{
    QString idLocataireStr = ui->lineEdit_id_locataire_espace->text().trimmed();

    if (idLocataireStr.isEmpty()) {
        ui->lineEdit_Nom_3_espace->clear();
        return;
    }

    bool ok;
    int idLocataire = idLocataireStr.toInt(&ok);
    if (!ok || idLocataire <= 0) {
        ui->lineEdit_Nom_3_espace->setText("ID invalide !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM LOCATAIRE WHERE ID = :id");
    query.bindValue(":id", idLocataire);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        ui->lineEdit_Nom_3_espace->setText(nom + " " + prenom);
    } else {
        ui->lineEdit_Nom_3_espace->setText("Aucun locataire trouvé avec cet ID");
    }
}

// Function to delete an Espace
void MainWindow::on_supprimer_espace_clicked()
{
    // Get the selected row from the table view
    QModelIndexList selectedRows = ui->tableView_3->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un espace à supprimer !");
        return;
    }

    // Get the ID of the selected space (assumes ID is in the first column)
    int selectedRow = selectedRows.first().row();
    int espaceID = ui->tableView_3->model()->data(ui->tableView_3->model()->index(selectedRow, 0)).toInt();

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
void MainWindow::on_modifier_espace_clicked()
{
    QModelIndexList selectedRows = ui->tableView_3->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un espace à modifier !");
        return;
    }

    int selectedRow = selectedRows.first().row();
    espaceID_a_modifier = ui->tableView_3->model()->data(ui->tableView_3->model()->index(selectedRow, 0)).toInt();

    chargerDonneesEspace(espaceID_a_modifier);
    ui->tabWidget_espace->setCurrentIndex(4); // Index 5 = page 5 (0-based)
}

void MainWindow::chargerDonneesEspace(int id)
{
    QSqlQuery query;
    query.prepare("SELECT e.*, l.NOM AS NOM_LOC "
                  "FROM \"YOUSSEF\".\"ESPACE\" e "
                  "LEFT JOIN \"YOUSSEF\".\"LOCATAIRE\" l ON e.\"ID_LOCATAIRE\" = l.\"ID\" "
                  "WHERE e.\"ID\" = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Données principales
        ui->lineEdit_Nom_mod_espace->setText(query.value("NOM").toString());
        ui->lineEdit_Type_mod_espace->setText(query.value("TYPE").toString());
        ui->spinBox_Superficie_mod_espace->setValue(query.value("SUPERFICIE").toDouble());
        ui->lineEdit_Emplacement_mod_espace->setText(query.value("EMPLACEMENT").toString());
        ui->comboBox_Statut_mod_espace->setCurrentText(query.value("STATUT").toString());
        ui->lineEdit_Loyer_mod_espace->setText(QString::number(query.value("LOYER_M").toDouble(), 'f', 2));

        // Dates
        QDate dateDebut = QDate::fromString(query.value("DATE_DEB_CONTRAT").toString(), "yyyy-MM-dd");
        QDate dateFin = QDate::fromString(query.value("DATE_FIN").toString(), "yyyy-MM-dd");
        ui->dateEdit_Debut_mod->setDate(dateDebut.isValid() ? dateDebut : QDate::currentDate());
        ui->dateEdit_Fin_mod->setDate(dateFin.isValid() ? dateFin : QDate::currentDate().addYears(1));

        // Locataire
        ui->lineEdit_id_locataire_2_espace->setText(query.value("ID_LOCATAIRE").toString());
        ui->lineEdit_Nom_4_espace->setText(query.value("NOM_LOC").toString());
    } else {
        QMessageBox::warning(this, "Erreur", "Chargement des données échoué: " + query.lastError().text());
    }
}
void MainWindow::on_enregistrer_mod_espace_clicked()
{
    if (espaceID_a_modifier == -1) {
        QMessageBox::warning(this, "Erreur", "Aucun espace sélectionné pour modification !");
        return;
    }

    // Récupération des valeurs
    QString nom = ui->lineEdit_Nom_mod_espace->text().trimmed();
    QString type = ui->lineEdit_Type_mod_espace->text().trimmed();
    QString emplacement = ui->lineEdit_Emplacement_mod_espace->text().trimmed();
    QString statut = ui->comboBox_Statut_mod_espace->currentText();
    double superficie = ui->spinBox_Superficie_mod_espace->value();

    // Validation des champs obligatoires
    if (nom.isEmpty() || type.isEmpty() || emplacement.isEmpty() || statut.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // Validation du loyer
    QString loyerStr = ui->lineEdit_Loyer_mod_espace->text().replace(",", ".");
    bool ok;
    double loyer = loyerStr.toDouble(&ok);
    if (!ok || loyer <= 0) {
        QMessageBox::warning(this, "Erreur", "Le loyer doit être un nombre valide et supérieur à 0 !");
        return;
    }

    // Validation des dates
    QDate dateDebut = ui->dateEdit_Debut_mod->date();
    QDate dateFin = ui->dateEdit_Fin_mod->date();
    if (dateDebut >= dateFin) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être après la date de début !");
        return;
    }

    // Récupération des infos du locataire
    QString idLocataireStr = ui->lineEdit_id_locataire_2_espace->text().trimmed();
    QString nomLocataire = ui->lineEdit_Nom_4_espace->text().trimmed();

    // Validation du locataire
    int idLocataire = 0;
    if (!idLocataireStr.isEmpty()) {
        idLocataire = idLocataireStr.toInt(&ok);
        if (!ok || idLocataire <= 0) {
            QMessageBox::warning(this, "Erreur", "ID locataire invalide !");
            return;
        }

        // Vérification que le locataire existe et récupération du nom
        QSqlQuery query;
        query.prepare("SELECT NOM, PRENOM FROM LOCATAIRE WHERE ID = :id");
        query.bindValue(":id", idLocataire);

        if (query.exec() && query.next()) {
            nomLocataire = query.value(0).toString() + " " + query.value(1).toString();
            ui->lineEdit_Nom_4_espace->setText(nomLocataire); // Mise à jour automatique du nom
        } else {
            QMessageBox::warning(this, "Erreur", "Locataire introuvable !");
            return;
        }
    }

    // Modification de l'espace
    Espace espace;
    if (espace.modifier(espaceID_a_modifier, nom, type, superficie, emplacement,
                        statut, loyer, dateDebut, dateFin, idLocataire)) {
        QMessageBox::information(this, "Succès", "Modification enregistrée avec succès !");
        ui->tabWidget_espace->setCurrentIndex(1);
        afficherEspaces();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + espace.lastError());
    }
}void MainWindow::on_exporterPDF_clicked()
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

    int colCount = ui->tableView_3->model()->columnCount();

    // Exclure la colonne "pdp" si elle existe
    int photoIndex = -1;
    for (int i = 0; i < colCount; ++i) {
        if (ui->tableView_3->model()->headerData(i, Qt::Horizontal).toString().toLower().contains("pdp")) {
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
                         Qt::AlignCenter, ui->tableView_3->model()->headerData(i, Qt::Horizontal).toString());
        x += colWidth + 10;
        j++;
    }

    y += rowHeight + 10;
    x = marginLeft;

    // Contenu du tableau (alternance de couleurs)
    painter.setFont(QFont("Helvetica", 10));
    painter.setPen(Qt::black);
    int rowCount = ui->tableView_3->model()->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QColor rowColor = (row % 2 == 0) ? QColor(245, 245, 245) : QColor(255, 255, 255);
        painter.setBrush(rowColor);

        for (int col = 0, j = 0; col < colCount; ++col) {
            if (col == photoIndex) continue;
            if (j >= colsToShow) break;

            // Bordure nette avec une couleur foncée
            painter.setPen(QColor(150, 150, 150));
            painter.drawRect(x, y, colWidth, rowHeight);

            QString data = ui->tableView_3->model()->data(ui->tableView_3->model()->index(row, col)).toString();

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
                                 Qt::AlignCenter, ui->tableView_3->model()->headerData(i, Qt::Horizontal).toString());
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

void MainWindow::rechercherEspaces() {
    QString searchText = ui->searchEspace->text().trimmed(); // Texte à rechercher

    // Vérifier si un QSortFilterProxyModel est déjà appliqué
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->tableView_3->model());

    if (!proxyModel) {
        // Créer un nouveau proxyModel si ce n’est pas encore le cas
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_3->model());  // Remplacer par ton modèle de base si différent
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterRole(Qt::DisplayRole);
        ui->tableView_3->setModel(proxyModel);
    }

    // Appliquer le filtre sur toutes les colonnes
    proxyModel->setFilterKeyColumn(-1);  // -1 = toutes les colonnes

    // Appliquer le filtre avec une expression régulière
    QRegularExpression regex(searchText, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);
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
    ui->tabWidget_espace->setCurrentIndex(2);  // Assurez-vous que l'index de l'onglet est bien 2
    QWidget *tab = ui->tabWidget_espace->widget(2);

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
    for (int i = 0; i < ui->tabWidget_espace->count(); ++i) {
        if (ui->tabWidget_espace->tabText(i) == "Carte") {
            qDebug() << "Onglet carte trouvé, passage à l'onglet";
            ui->tabWidget_espace->setCurrentIndex(i);
            return;
        }
    }

    qDebug() << "Création d'un nouvel onglet carte";
    showMapView();
}


void MainWindow::showMapView()
{
    qDebug() << "Initialisation de la vue carte";

    // Création du widget et layout principal
    QWidget *mapTab = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mapTab);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // --- Barre de contrôle supérieure ---
    QHBoxLayout *controlLayout = new QHBoxLayout();


    // ComboBox de recherche
    QComboBox *spaceSelector = new QComboBox();
    spaceSelector->setMinimumWidth(300);
    spaceSelector->setStyleSheet("QComboBox { padding: 8px; font-size: 14px; }");
    spaceSelector->addItem("🔍 Rechercher un espace...");

    // Bouton rafraîchissement
    QPushButton *refreshBtn = new QPushButton("🔄 Actualiser");
    refreshBtn->setStyleSheet("QPushButton { padding: 5px 10px; }");

    controlLayout->addWidget(spaceSelector);
    controlLayout->addStretch();
    controlLayout->addWidget(refreshBtn);
    mainLayout->addLayout(controlLayout);

    // --- Scène graphique ---
    m_scene = new QGraphicsScene(this); // Utilisez le membre de classe    scene->setSceneRect(0, 0, 1500, 1000);  // Taille de la carte

    // Configuration de la vue
    QGraphicsView *view = new QGraphicsView(m_scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QColor(240, 248, 255));  // Fond bleu clair
    view->setDragMode(QGraphicsView::ScrollHandDrag);  // Navigation par glissement

    // --- Barre d'outils de zoom ---
    QHBoxLayout *zoomLayout = new QHBoxLayout();

    QPushButton *zoomInBtn = new QPushButton("+ Zoom");
    QPushButton *zoomOutBtn = new QPushButton("- Zoom");
    QPushButton *resetBtn = new QPushButton("Reset");

    zoomLayout->addWidget(zoomOutBtn);
    zoomLayout->addWidget(resetBtn);
    zoomLayout->addWidget(zoomInBtn);
    mainLayout->addLayout(zoomLayout);
    mainLayout->addWidget(view);
    // --- Légende à droite ---
    QHBoxLayout *mainMapLayout = new QHBoxLayout();
    mainMapLayout->addWidget(view, 4);  // 80% de l'espace pour la carte

    // Création du panneau de légende
    QWidget *legendPanel = new QWidget();
    legendPanel->setFixedWidth(200);  // Largeur fixe pour la légende
    QVBoxLayout *legendLayout = new QVBoxLayout(legendPanel);
    legendLayout->setAlignment(Qt::AlignTop);

    // Titre de la légende
    QLabel *legendTitle = new QLabel("Légende des espaces");
    legendTitle->setStyleSheet("font-weight: bold; font-size: 14px;");
    legendLayout->addWidget(legendTitle);
    // Après avoir créé la scène
    m_scene->installEventFilter(this);
    // Ajout des éléments de légende
    const QMap<QString, QColor> typeColors ={
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

    for (const auto &[type, color] : typeColors.asKeyValueRange()) {
        QHBoxLayout *itemLayout = new QHBoxLayout();

        // Carré de couleur
        QLabel *colorLabel = new QLabel();
        colorLabel->setFixedSize(20, 20);
        colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border: 1px solid black;")
                                      .arg(color.red()).arg(color.green()).arg(color.blue()));

        // Nom du type
        QLabel *typeLabel = new QLabel(type);
        typeLabel->setStyleSheet("font-size: 12px;");

        itemLayout->addWidget(colorLabel);
        itemLayout->addWidget(typeLabel);
        itemLayout->addStretch();

        legendLayout->addLayout(itemLayout);
    }

    mainMapLayout->addWidget(legendPanel, 1);  // 20% pour la légende
    mainLayout->addLayout(mainMapLayout);
    // --- Connexions des signaux ---
    connect(zoomInBtn, &QPushButton::clicked, [view]() { view->scale(1.2, 1.2); });
    connect(zoomOutBtn, &QPushButton::clicked, [view]() { view->scale(0.8, 0.8); });
    // Au lieu de capturer m_scene directement, utilisez this->
    connect(resetBtn, &QPushButton::clicked, [view, this]() {
        view->resetTransform();
        view->fitInView(this->m_scene->sceneRect(), Qt::KeepAspectRatio);
    });

    connect(refreshBtn, &QPushButton::clicked, this, [this]() {
        showMapView();  // Recrée la vue
    });

    // --- Création du contenu de la carte ---
    createMapZones(m_scene);  // Dessine les zones A, B, C, D
    loadSpacesFromDatabase(m_scene, spaceSelector, view);  // Charge les espaces depuis la DB

    // --- Ajout de l'onglet ---
    int tabIndex = ui->tabWidget_espace->addTab(mapTab, "🗺️ Carte");
    ui->tabWidget_espace->setCurrentIndex(tabIndex);
}

void MainWindow::createMapZones(QGraphicsScene* scene)
{
    if (!scene) {
        qWarning() << "Scene is null in createMapZones";
        return;
    }

    // Configuration des zones
    struct Zone {
        QString name;
        QRectF rect;
        QColor color;
        QString description = "";  // Initialisation par défaut ajoutée ici
    };

    QVector<Zone> zones = {
        {"A", QRectF(100, 100, 600, 400), QColor(230, 230, 250, 80),},
        {"B", QRectF(750, 100, 600, 400), QColor(255, 228, 225, 80),},
        {"C", QRectF(100, 550, 600, 400), QColor(240, 255, 240, 80), },
        {"D", QRectF(750, 550, 600, 400), QColor(255, 250, 205, 80), }
    };

    // Création des zones
    for (const Zone& zone : zones) {
        try {
            // Rectangle de zone
            QGraphicsRectItem* zoneRect = new QGraphicsRectItem(zone.rect);
            zoneRect->setBrush(QBrush(zone.color));
            zoneRect->setPen(QPen(Qt::black, 2, Qt::DashLine));
            scene->addItem(zoneRect);

            // Texte de la zone
            QGraphicsTextItem* zoneText = new QGraphicsTextItem("Zone " + zone.name + "\n" + zone.description);
            zoneText->setFont(QFont("Arial", 12, QFont::Bold));
            zoneText->setDefaultTextColor(Qt::darkBlue);
            zoneText->setPos(zone.rect.x() + 10, zone.rect.y() + 10);
            scene->addItem(zoneText);

        } catch (const std::exception& e) {
            qCritical() << "Error creating zone" << zone.name << ":" << e.what();
        }
    }
}
void MainWindow::loadSpacesFromDatabase(QGraphicsScene* scene, QComboBox* spaceSelector, QGraphicsView* view)
{
    if (!scene || !spaceSelector || !view) {
        qWarning() << "Paramètres invalides dans loadSpacesFromDatabase";
        return;
    }

    // Vider le sélecteur d'espaces
    spaceSelector->clear();
    spaceSelector->addItem("🔍 Rechercher un espace...");

    // Couleurs par type d'espace
    const QMap<QString, QColor> typeColors = {
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

    try {
        QSqlQuery query;
        if (!query.exec("SELECT ID, NOM, TYPE, STATUT, EMPLACEMENT FROM ESPACE")) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        QMap<QString, QGraphicsItemGroup*> spaceItems; // Utilisation d'un groupe pour tous les éléments
        QMap<QString, int> spaceCountPerZone;

        while (query.next()) {
            // Extraction des données
            //int id = query.value("ID").toInt();
            QString name = query.value("NOM").toString();
            QString type = query.value("TYPE").toString();
            QString status = query.value("STATUT").toString();
            QString zone = query.value("EMPLACEMENT").toString().toUpper();

            // Couleur et statut
            QColor color = typeColors.value(type, Qt::gray);
            bool isOccupied = (status.toLower() == "occupé");
            if (isOccupied) {
                color = color.darker(150);
            }

            // Positionnement dans la zone
            int count = spaceCountPerZone[zone]++;
            static const QMap<QString, QPointF> zoneBases = {
                {"A", QPointF(100, 100)},
                {"B", QPointF(750, 100)},
                {"C", QPointF(100, 550)},
                {"D", QPointF(750, 550)}
            };

            QPointF base = zoneBases.value(zone, QPointF(100, 100));
            int col = count % 4;
            int row = count / 4;

            qreal x = base.x() + 20 + col * 150;
            qreal y = base.y() + 50 + row * 120;

            // Création d'un groupe pour contenir tous les éléments de l'espace
            QGraphicsItemGroup* spaceGroup = new QGraphicsItemGroup();
            scene->addItem(spaceGroup);

            // Rectangle principal
            QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, 140, 100);
            rect->setPos(0, 0);
            rect->setBrush(color);
            rect->setPen(QPen(Qt::black, 1));
            spaceGroup->addToGroup(rect);

            // Nom de l'espace (tronqué si trop long)
            QString displayedName = name.length() > 15 ? name.left(12) + "..." : name;
            QGraphicsTextItem* nameText = new QGraphicsTextItem(displayedName);
            nameText->setFont(QFont("Arial", 8, QFont::Bold));
            nameText->setDefaultTextColor(Qt::black);
            nameText->setPos(5, 5);
            spaceGroup->addToGroup(nameText);



            // Stockez le nom de l'espace dans l'item
            rect->setData(0, name);

            // Activez les événements de souris
            rect->setFlag(QGraphicsItem::ItemIsSelectable);
            rect->setAcceptHoverEvents(true);
            rect->setCursor(Qt::PointingHandCursor);

            scene->addItem(rect);

            // Indicateur de statut
            QGraphicsRectItem* statusRect = new QGraphicsRectItem(0, 0, 50, 20);
            statusRect->setPos(90, 80);
            statusRect->setBrush(isOccupied ? Qt::red : Qt::green);
            statusRect->setPen(QPen(Qt::black, 1));
            spaceGroup->addToGroup(statusRect);

            // Texte du statut
            QGraphicsTextItem* statusText = new QGraphicsTextItem(isOccupied ? "Occupé" : "Libre");
            statusText->setFont(QFont("Arial", 7));
            statusText->setDefaultTextColor(Qt::white);
            statusText->setPos(95, 80);
            spaceGroup->addToGroup(statusText);

            // Positionnement du groupe
            spaceGroup->setPos(x, y);

            // Ajout au sélecteur
            spaceSelector->addItem(QString("%1 (Zone %2 - %3)").arg(name).arg(zone).arg(isOccupied ? "Occupé" : "Libre"));
            spaceItems[name] = spaceGroup;
        }

        // Connexion du sélecteur
        connect(spaceSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [this, view, spaceItems, spaceSelector](int index) {
                    if (index > 0) {
                        QString name = spaceSelector->itemText(index).split(" (Zone").first();
                        if (spaceItems.contains(name)) {
                            view->centerOn(spaceItems[name]);

                            // Mise en surbrillance
                            for (auto& item : spaceItems) {
                                QPen pen = (item == spaceItems[name]) ? QPen(Qt::blue, 3) : QPen(Qt::black, 1);
                                QGraphicsItemGroup* group = item;
                                for (QGraphicsItem* child : group->childItems()) {
                                    if (auto* rectItem = dynamic_cast<QGraphicsRectItem*>(child)) {
                                        rectItem->setPen(pen);
                                    }
                                }
                            }
                        }
                    }
                });
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors du chargement des espaces:\n" + QString::fromStdString(e.what()));
    }
}



/*
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
*/
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
        QGraphicsItem* item = m_scene->itemAt(mouseEvent->scenePos(), QTransform());

        if (item && item->data(0).isValid()) {
            // Vérification du type et conversion
            if (QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
                QString spaceName = item->data(0).toString();

                // Feedback visuel
                rectItem->setPen(QPen(Qt::blue, 2));
                QTimer::singleShot(200, [rectItem]() {
                    rectItem->setPen(QPen(Qt::black, 1));
                });

                //show3DView(spaceName);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_tri_espaces_clicked()
{
        // Obtenir l'index de la colonne sélectionnée
        int columnIndex = ui->tableView_3->currentIndex().column();

        if (columnIndex < 0) {
            QMessageBox::warning(this, "Tri impossible", "Veuillez sélectionner une colonne pour trier.");
            return;
        }

        // Obtenir le nom de la colonne à partir du modèle actuel
        QString columnName = ui->tableView_3->model()->headerData(columnIndex, Qt::Horizontal).toString();
        qDebug() << "Colonne sélectionnée pour le tri :" << columnName;

        // Construire la requête SQL dynamiquement avec le nom de colonne
        QString queryStr = QString("SELECT * FROM ESPACE ORDER BY %1 ASC").arg(columnName);

        QSqlQuery query;
        if (!query.exec(queryStr)) {
            QMessageBox::critical(this, "Erreur SQL", "Impossible d'exécuter la requête de tri.");
            return;
        }

        // Créer un modèle pour les résultats
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(query);

        // Vérifier les erreurs
        if (model->lastError().isValid()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors du chargement des données triées.");
            return;
        }

        // Appliquer le modèle au tableau
        ui->tableView_3->setModel(model);

        // Appliquer le style (facultatif)
        ui->tableView_3->setStyleSheet(
            "QTableView {"
            "   background-color: #f5f5f5;"
            "   border: 1px solid #ccc;"
            "   selection-background-color: #A3C1DA;"
            "   font: bold 12px;"
            "}"
            "QTableView::item:selected {"
            "   background-color: #6fa3ef;"
            "}"
            "QHeaderView::section {"
            "   background-color: #2e3d4e;"
            "   color: white;"
            "   border: 1px solid #aaa;"
            "}"
            );

        ui->tableView_3->resizeColumnsToContents();

        // Confirmation
        QMessageBox::information(this, "Tri effectué", QString("Tri effectué par la colonne '%1'.").arg(columnName));


}



void MainWindow::on_ajouterespace_clicked()
{

        // Get values from the form
        QString nom = ui->lineEdit_Nom_espace->text();
        QString type = ui->comboBox_Type_espace->currentText();
        int superficie = ui->spinBox_Superficie_espace->value();
        QString emplacement = ui->lineEdit_Emplacement_espace->text();
        QString statut = ui->comboBox_espace->currentText();
        QString dateDebut = ui->dateEdit_espace->date().toString("yyyy-MM-dd");
        QString dateFin = ui->dateEdit_2_espace->date().toString("yyyy-MM-dd");

        // Get locataire ID
        QString idLocataireStr = ui->lineEdit_id_locataire_espace->text().trimmed();
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
        QString loyerStr = ui->lineEdit_loyer_espace->text().replace(",", ".");
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
        QDate startDate = ui->dateEdit_espace->date();
        QDate endDate = ui->dateEdit_2_espace->date();
        if (startDate >= endDate) {
            QMessageBox::warning(this, "Erreur", "La date de fin doit être après la date de début !");
            return;
        }

        // Create Espace and add to DB
        Espace Espace(nom, type, superficie, emplacement, statut, loyer, startDate, endDate, idLocataire);
        if (Espace.ajouter()) {
            QMessageBox::information(this, "Succès", "Espace ajouté avec succès !");

            // Clear form
            ui->lineEdit_Nom_espace->clear();
            ui->comboBox_Type_espace->setCurrentIndex(0);
            ui->spinBox_Superficie_espace->setValue(1);
            ui->lineEdit_Emplacement_espace->clear();
            ui->comboBox_espace->setCurrentIndex(0);
            ui->lineEdit_loyer_espace->clear();
            ui->dateEdit_espace->setDate(QDate::currentDate());
            ui->dateEdit_2_espace->setDate(QDate::currentDate().addYears(1));
            ui->lineEdit_id_locataire_espace->clear();
            ui->lineEdit_Nom_3_espace->clear();

            // Switch to list tab
            ui->tabWidget_espace->setCurrentIndex(1);
            afficherEspaces();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'espace !");
        }
}
void MainWindow::on_lineEdit_id_locataire_2_espace_editingFinished()
{
    QString idStr = ui->lineEdit_id_locataire_2_espace->text().trimmed();

    if (idStr.isEmpty()) {
        ui->lineEdit_Nom_4_espace->clear();
        return;
    }

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        ui->lineEdit_Nom_4_espace->setText("ID invalide");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM LOCATAIRE WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString nomComplet = query.value(0).toString() + " " + query.value(1).toString();
        ui->lineEdit_Nom_4_espace->setText(nomComplet);
    } else {
        ui->lineEdit_Nom_4_espace->setText("Locataire introuvable");
    }
}




//____________________HAMZA_____________________


#include <QRegularExpression> // <-- ajout ici

void MainWindow::on_btnAjouterLocataire_clicked()
{
    qDebug() << "Tentative d'ajout d'un locataire...";

    QString nom = ui->nom_locataire->text().trimmed();
    QString prenom = ui->prenom_locataire->text().trimmed();
    QString typeact = ui->typeac_locataire->text().trimmed();
    QString email = ui->email_locataire->text().trimmed();
    QString telephoneStr = ui->tel_locataire->text().trimmed();
    QString duree = ui->duree_locataire->text().trimmed();
    QString paiement = ui->paiement_locataire->text().trimmed();
    QDate datedeb = ui->datedeb_locataire->date();
    QDate datefin = ui->datefin_locataire->date();

    // Vérifications simples
    if (nom.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Le champ 'Nom' est vide !");
        return;
    }

    if (prenom.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Le champ 'Prénom' est vide !");
        return;
    }

    if (typeact.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Le champ 'Type d'activité' est vide !");
        return;
    }

    if (email.isEmpty() || !(email.contains('@') && email.contains('.'))) {
        QMessageBox::critical(this, "Erreur", "L'adresse email est invalide !");
        return;
    }

    if (telephoneStr.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone est vide !");
        return;
    }

    // Vérification téléphone : que des chiffres et longueur correcte
    bool telephoneOk = true;
    for (int i = 0; i < telephoneStr.length(); ++i) {
        if (!telephoneStr[i].isDigit()) {
            telephoneOk = false;
            break;
        }
    }
    if (!telephoneOk || telephoneStr.length() < 8 || telephoneStr.length() > 15) {
        QMessageBox::critical(this, "Erreur", "Le numéro de téléphone doit contenir uniquement entre 8 et 15 chiffres.");
        return;
    }
    int telephone = telephoneStr.toInt(); // sûr ici

    if (duree.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "La durée est vide !");
        return;
    }

    // Vérification durée : que des chiffres
    bool dureeOk = true;
    for (int i = 0; i < duree.length(); ++i) {
        if (!duree[i].isDigit()) {
            dureeOk = false;
            break;
        }
    }
    if (!dureeOk) {
        QMessageBox::critical(this, "Erreur", "La durée doit être un nombre valide !");
        return;
    }

    if (datedeb > datefin) {
        QMessageBox::critical(this, "Erreur", "La date de début doit être avant la date de fin !");
        return;
    }

    if (paiement.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Le mode de paiement est vide !");
        return;
    }

    // Chargement du contrat
    QByteArray contratData;
    if (!cheminContrat.isEmpty()) {
        QFile file(cheminContrat);
        if (file.open(QIODevice::ReadOnly)) {
            contratData = file.readAll();
            file.close();
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier contrat.");
            return;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Veuillez sélectionner un fichier de contrat !");
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
        ui->tableView_loc->setModel(model);
        ui->tableView_loc->setStyleSheet(
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
    QModelIndexList selection = ui->tableView_loc->selectionModel()->selectedRows();

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
    QItemSelectionModel *select = ui->tableView_loc->selectionModel();  // Récupérer le modèle de sélection
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
    int columnIndex = ui->tableView_loc->currentIndex().column();

    // Vérifier si aucune colonne n'est sélectionnée
    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez choisir une colonne à trier !");
        return; // Arrêter la fonction si aucune colonne n'est sélectionnée
    }

    qDebug() << "Tri en cours sur la colonne : " << columnIndex;

    // Vérifier si un proxyModel existe déjà
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView_loc->model());
    if (!proxyModel) {
        // Si aucun proxyModel n'existe, on en crée un
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView_loc->model());
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        ui->tableView_loc->setModel(proxyModel); // Appliquer le proxyModel à tableView
    }

    // Appliquer le tri sur la colonne sélectionnée
    proxyModel->sort(columnIndex, Qt::AscendingOrder);
    qDebug() << "Tri effectué sur la colonne : " << columnIndex;
}
void MainWindow::exporterPDF_locataire() {

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
        QAbstractItemModel *model = ui->tableView_loc->model();
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
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView_loc->model());

        if (!proxyModel) {
            proxyModel = new QSortFilterProxyModel(this);
            proxyModel->setSourceModel(ui->tableView_loc->model());  // Assurez-vous que le modèle de locataires est le modèle source
            proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
            ui->tableView_loc->setModel(proxyModel);
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
        ui->contrat_locataire->setText(chemin);

        // Option 2 : Afficher l'image dans un QLabel
        QPixmap image(chemin);
        ui->contrat_locataire->setPixmap(image.scaled(ui->contrat_locataire->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
        {"text", "Ce document est un contrat de location. Peux-tu extraire les informations utiles pour un système de gestion locative ? Les données recherchées sont : nom complet, adresse email, date de naissance, adresse, type d'activité, téléphone, durée du contrat, date de début du contrat et date de fin du contrat. Ce traitement est automatisé dans une application."}
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

    // Préparer la requête HTTP
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

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

    // Regex
    QRegularExpression regexNomComplet("Nom complet\\s*:\\s*([A-Za-zÀ-ÿ\\-\\s]+)");
    QRegularExpression regexEmail("Adresse email\\s*:\\s*([\\w.-]+@[\\w.-]+)");
    QRegularExpression regexTypeActivite("Type d'activité\\s*:\\s*(.+)");
    QRegularExpression regexTelephone("Téléphone\\s*:\\s*(\\+?\\d[\\d\\s\\-]+)");
    QRegularExpression regexDureeContrat("Durée du contrat\\s*:\\s*(\\d+\\s*(?:mois|années?))");
    QRegularExpression regexDateDebut("Date de début\\s*:\\s*(\\d{2}/\\d{2}/\\d{4})");
    QRegularExpression regexDateFin("Date de fin\\s*:\\s*(\\d{2}/\\d{2}/\\d{4})");

    bool extractionReussie = false;
    QRegularExpressionMatch match;

    match = regexNomComplet.match(contenu);
    if (match.hasMatch()) {
        QString nomComplet = match.captured(1).trimmed();
        QStringList noms = nomComplet.split(" ");
        if (noms.size() >= 2) {
            ui->prenom_locataire->setText(noms.first());
            ui->nom_locataire->setText(noms.last());
            extractionReussie = true;
        }
    }

    match = regexEmail.match(contenu);
    if (match.hasMatch()) {
        ui->email_locataire->setText(match.captured(1));
        extractionReussie = true;
    }

    match = regexTypeActivite.match(contenu);
    if (match.hasMatch()) {
        ui->typeac_locataire->setText(match.captured(1).trimmed());
        extractionReussie = true;
    }

    match = regexTelephone.match(contenu);
    if (match.hasMatch()) {
        ui->tel_locataire->setText(match.captured(1).trimmed());
        extractionReussie = true;
    }

    match = regexDureeContrat.match(contenu);
    if (match.hasMatch()) {
        ui->duree_locataire->setText(match.captured(1).trimmed());
        extractionReussie = true;
    }

    match = regexDateDebut.match(contenu);
    if (match.hasMatch()) {
        ui->datedeb_locataire->setDate(QDate::fromString(match.captured(1), "dd/MM/yyyy"));
        extractionReussie = true;
    }

    match = regexDateFin.match(contenu);
    if (match.hasMatch()) {
        ui->datefin_locataire->setDate(QDate::fromString(match.captured(1), "dd/MM/yyyy"));
        extractionReussie = true;
    }

    if (!extractionReussie) {
        QMessageBox::warning(this, "Extraction échouée", "Aucune donnée n’a pu être extraite du texte fourni par l’IA. Vérifiez le format du document ou réessayez.");
    }

    reply->deleteLater();
}


void MainWindow::afficherStatistiques_locataire() {
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
    QWidget *statistiquesTab = ui->tabWidget_loc->widget(2);

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
void MainWindow::genererContratDepuisOpenAI()
{
    QString nom = ui->nom_locataire->text().trimmed();
    QString prenom = ui->prenom_locataire->text().trimmed();
    QString typeact = ui->typeac_locataire->text().trimmed();
    QString email = ui->email_locataire->text().trimmed();
    QString telephoneStr = ui->tel_locataire->text().trimmed();
    QDate datedeb = ui->datedeb_locataire->date();
    QDate datefin = ui->datefin_locataire->date();
    QString duree = ui->duree_locataire->text().trimmed();
    QString paiement = ui->paiement_locataire->text().trimmed();

    QString prompt = QString(
                         "Rédige un contrat de location pour un centre commercial avec les détails suivants :\n"
                         "- Nom du locataire : %1\n"
                         "- Prénom du locataire : %2\n"
                         "- Type d'activité : %3\n"
                         "- Email : %4\n"
                         "- Téléphone : %5\n"
                         "- Date de début : %6\n"
                         "- Date de fin : %7\n"
                         "- Durée du contrat : %8\n"
                         "- Modalités de paiement : %9\n"
                         "\nRédige le contrat de manière formelle avec des sections numérotées."
                         ).arg(nom, prenom, typeact, email, telephoneStr,
                              datedeb.toString("dd/MM/yyyy"), datefin.toString("dd/MM/yyyy"),
                              duree, paiement);

    envoyerPromptAOpenAI(prompt);
}


void MainWindow::envoyerPromptAOpenAI(const QString& prompt)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + API_KEY.toUtf8());

    QJsonObject message;
    message["role"] = "user";
    message["content"] = prompt;

    QJsonArray messagesArray;
    messagesArray.append(message);

    QJsonObject payload;
    payload["model"] = "gpt-4o";
    payload["messages"] = messagesArray;
    payload["temperature"] = 0.5;

    QJsonDocument doc(payload);

    QNetworkReply* reply = manager->post(request, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(this, "Erreur API", reply->errorString());
            reply->deleteLater();
            return;
        }

        QByteArray response_data = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(response_data);
        QString contratText = responseDoc["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();

        // === Préparer le document texte ===
        QTextDocument* document = new QTextDocument(this);

        QString html = QString(R"(
            <html>
            <head>
                <meta charset='utf-8'>
                <style>
                    body {
                        font-family: 'Times New Roman', serif;
                        font-size: 14pt;
                        text-align: justify;
                        margin: 2cm;
                    }
                    h1 {
                        text-align: center;
                        font-size: 24pt;
                        margin-bottom: 20px;
                    }
                    footer {
                        margin-top: 30px;
                        font-size: 10pt;
                        text-align: center;
                        color: gray;
                    }
                </style>
            </head>
            <body>
                <h1>Contrat de Location</h1>
                <p>%1</p>
                <footer>Généré automatiquement le %2</footer>
            </body>
            </html>
        )")
                           .arg(contratText.toHtmlEscaped())
                           .arg(QDate::currentDate().toString("dd/MM/yyyy"));

        document->setHtml(html);

        // === Demander où enregistrer ===
        QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le contrat", "", "PDF Files (*.pdf)");
        if (!filePath.isEmpty()) {
            QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(filePath);
            printer.setPageSize(QPageSize(QPageSize::A4));
            printer.setPageMargins(QMarginsF(15, 15, 15, 15)); // 15 mm de marges

            document->print(&printer);

            QMessageBox::information(this, "Succès", "Contrat généré et sauvegardé !");
        }

        reply->deleteLater();
    });
}
void MainWindow::verifierEmail(const QString &text)
{
    if (text.contains('@') && text.contains('.')) {
        // Email correct → pas de bordure spéciale
        ui->email_locataire->setStyleSheet("");
    } else {
        // Email incorrect → ajouter une bordure rouge fine
        ui->email_locataire->setStyleSheet("border: 2px solid red;");
    }
}
