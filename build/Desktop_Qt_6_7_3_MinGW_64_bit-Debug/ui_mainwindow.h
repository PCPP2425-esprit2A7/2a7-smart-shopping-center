/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QPushButton *pushButton_10;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_3;
    QPushButton *pushButton_7;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton_9;
    QTabWidget *tabWidget;
    QWidget *Ajout;
    QGroupBox *groupBox_5;
    QLabel *label_14;
    QLineEdit *nom;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *prenom;
    QLineEdit *telephone;
    QLineEdit *email;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *statut;
    QDateEdit *date_embauche;
    QLabel *label_21;
    QLabel *label_22;
    QLineEdit *salaire;
    QComboBox *sexe;
    QPushButton *pushButton_pdp2;
    QComboBox *poste;
    QGroupBox *groupBox_6;
    QLabel *label_25;
    QPushButton *pushButton_choisirImage;
    QPushButton *pushButton_analysePDP;
    QPushButton *pushButton_pdp;
    QPushButton *pushButton_ajouter;
    QPushButton *cvButton;
    QWidget *liste_2;
    QTableView *tableView;
    QPushButton *afficher;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_supprimer;
    QLabel *label_5;
    QPushButton *pushButton_exporterPDF;
    QPushButton *statt;
    QPushButton *tri;
    QComboBox *comboRecherchePar;
    QLabel *labelIndication;
    QLineEdit *champRecherche;
    QPushButton *btnRechercher;
    QWidget *stat_2;
    QGraphicsView *graphicsView;
    QWidget *tab;
    QGroupBox *groupBox_7;
    QLabel *label_23;
    QLineEdit *nom_2;
    QLabel *label_24;
    QLabel *label_26;
    QLineEdit *prenom_2;
    QLineEdit *telephone_2;
    QLineEdit *email_2;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLineEdit *poste_2;
    QLineEdit *statut_2;
    QDateEdit *date_embauche_2;
    QLabel *label_31;
    QLabel *label_32;
    QLineEdit *salaire_2;
    QPushButton *pushButton_pdp2_2;
    QComboBox *sexe_2;
    QGroupBox *groupBox_8;
    QLabel *label_33;
    QPushButton *pushButton_pdp_2;
    QPushButton *pushButton_modifierPhoto;
    QPushButton *pushButton_12;
    QPushButton *pushButton_appliquerModif;
    QWidget *tab_2;
    QGroupBox *groupBox_9;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_nom;
    QLabel *label_35;
    QPushButton *pushButton_13;
    QPushButton *btnAccederTodo;
    QPushButton *btnCheckEmpreinte;
    QPushButton *pushButton_8;
    QWidget *tab_3;
    QLabel *label_6;
    QTableView *tableTodo;
    QPushButton *pushButton;
    QWidget *tab_4;
    QGroupBox *groupBox_10;
    QPushButton *pushButton_faceId;
    QLineEdit *lineEdit_3;
    QGroupBox *groupBox_11;
    QPushButton *btnAjouterEmpreinte;
    QLineEdit *lineEdit_4;
    QLabel *label_4;
    QPushButton *liste;
    QPushButton *stat;
    QPushButton *ajout;
    QPushButton *pushButton_11;
    QPushButton *login;
    QPushButton *liste_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1274, 1070);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 0, 1331, 1011));
        groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);"));
        pushButton_10 = new QPushButton(groupBox);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(-30, 220, 101, 61));
        pushButton_10->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/5973800-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_10->setIcon(icon);
        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(-40, 500, 111, 61));
        pushButton_4->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/1623971-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_4->setIcon(icon1);
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(-30, 410, 101, 61));
        pushButton_5->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/9321345-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_5->setIcon(icon2);
        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(-30, 590, 101, 61));
        pushButton_6->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/4986021-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_6->setIcon(icon3);
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(10, 20, 81, 71));
        pushButton_3->setStyleSheet(QString::fromUtf8("background: none; /* Supprime le fond */\n"
"qproperty-iconSize: 70px 70px; /* Augmente la taille de l'ic\303\264ne */\n"
"border: none; /* Supprime la bordure si n\303\251cessaire */\n"
"\n"
""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/logo_final-removebg-preview.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_3->setIcon(icon4);
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(-30, 320, 101, 61));
        pushButton_7->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/3079026-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_7->setIcon(icon5);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 20, 1101, 81));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 25px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(550, 29, 81, 41));
        label_2->setStyleSheet(QString::fromUtf8("background : none ;\n"
"font: 700 22pt \"Arial\";"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(500, 70, 181, 21));
        label_3->setStyleSheet(QString::fromUtf8("background : none ;\n"
"font: 14pt \"Segoe Fluent Icons\";"));
        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(140, 40, 41, 41));
        pushButton_9->setStyleSheet(QString::fromUtf8("background : transparent ;\n"
"background-color: transparent;\n"
"qproperty-iconSize: 30px 30px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon6(QIcon::fromTheme(QIcon::ThemeIcon::SystemLogOut));
        pushButton_9->setIcon(icon6);
        tabWidget = new QTabWidget(groupBox);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(140, 170, 1071, 501));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 25px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        Ajout = new QWidget();
        Ajout->setObjectName("Ajout");
        groupBox_5 = new QGroupBox(Ajout);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setEnabled(true);
        groupBox_5->setGeometry(QRect(90, 30, 401, 401));
        groupBox_5->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_14 = new QLabel(groupBox_5);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(30, 20, 81, 21));
        label_14->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        nom = new QLineEdit(groupBox_5);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(140, 10, 161, 41));
        nom->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_15 = new QLabel(groupBox_5);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(20, 50, 141, 31));
        label_15->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(30, 320, 131, 41));
        label_16->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        prenom = new QLineEdit(groupBox_5);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(140, 50, 151, 41));
        prenom->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        telephone = new QLineEdit(groupBox_5);
        telephone->setObjectName("telephone");
        telephone->setGeometry(QRect(140, 90, 161, 41));
        telephone->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        email = new QLineEdit(groupBox_5);
        email->setObjectName("email");
        email->setGeometry(QRect(140, 130, 151, 41));
        email->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_17 = new QLabel(groupBox_5);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(20, 90, 141, 41));
        label_17->setStyleSheet(QString::fromUtf8("border:  transparent ;\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        label_18 = new QLabel(groupBox_5);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(30, 220, 121, 51));
        label_18->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(10, 270, 271, 51));
        label_19->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(30, 130, 111, 31));
        label_20->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        statut = new QLineEdit(groupBox_5);
        statut->setObjectName("statut");
        statut->setGeometry(QRect(140, 220, 151, 41));
        statut->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        date_embauche = new QDateEdit(groupBox_5);
        date_embauche->setObjectName("date_embauche");
        date_embauche->setGeometry(QRect(140, 270, 181, 41));
        date_embauche->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_21 = new QLabel(groupBox_5);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(30, 180, 91, 31));
        label_21->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_22 = new QLabel(groupBox_5);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(30, 350, 201, 41));
        label_22->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        salaire = new QLineEdit(groupBox_5);
        salaire->setObjectName("salaire");
        salaire->setGeometry(QRect(140, 350, 161, 41));
        salaire->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        sexe = new QComboBox(groupBox_5);
        sexe->addItem(QString());
        sexe->addItem(QString());
        sexe->setObjectName("sexe");
        sexe->setGeometry(QRect(140, 170, 161, 41));
        sexe->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"font: 600 11pt \"Segoe UI\";"));
        pushButton_pdp2 = new QPushButton(groupBox_5);
        pushButton_pdp2->setObjectName("pushButton_pdp2");
        pushButton_pdp2->setGeometry(QRect(320, 10, 61, 61));
        pushButton_pdp2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 80px 80px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
""));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/istockphoto-1474007937-170667a-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdp2->setIcon(icon7);
        poste = new QComboBox(groupBox_5);
        poste->addItem(QString());
        poste->addItem(QString());
        poste->addItem(QString());
        poste->setObjectName("poste");
        poste->setGeometry(QRect(140, 320, 171, 31));
        poste->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"font: 600 11pt \"Segoe UI\";"));
        groupBox_6 = new QGroupBox(Ajout);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(560, 70, 241, 291));
        groupBox_6->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(50, 20, 141, 31));
        label_25->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 13pt \"Arial Rounded MT\";\n"
""));
        pushButton_choisirImage = new QPushButton(groupBox_6);
        pushButton_choisirImage->setObjectName("pushButton_choisirImage");
        pushButton_choisirImage->setGeometry(QRect(30, 220, 171, 41));
        pushButton_choisirImage->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"font: 900 11pt \"Segoe UI\";\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        QIcon icon8(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
        pushButton_choisirImage->setIcon(icon8);
        pushButton_analysePDP = new QPushButton(groupBox_6);
        pushButton_analysePDP->setObjectName("pushButton_analysePDP");
        pushButton_analysePDP->setGeometry(QRect(160, 160, 51, 61));
        pushButton_analysePDP->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 40px 40px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : none ;"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Downloads/1672561.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_analysePDP->setIcon(icon9);
        pushButton_pdp = new QPushButton(groupBox_6);
        pushButton_pdp->setObjectName("pushButton_pdp");
        pushButton_pdp->setGeometry(QRect(40, 50, 161, 141));
        pushButton_pdp->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 100px 100px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/img/add-image-photo-icon.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdp->setIcon(icon10);
        label_25->raise();
        pushButton_choisirImage->raise();
        pushButton_pdp->raise();
        pushButton_analysePDP->raise();
        pushButton_ajouter = new QPushButton(Ajout);
        pushButton_ajouter->setObjectName("pushButton_ajouter");
        pushButton_ajouter->setGeometry(QRect(620, 380, 111, 31));
        pushButton_ajouter->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 12pt \"Arial Rounded MT\";"));
        cvButton = new QPushButton(Ajout);
        cvButton->setObjectName("cvButton");
        cvButton->setGeometry(QRect(840, 140, 111, 151));
        cvButton->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 110px 110px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : none ;"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Downloads/8347432.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        cvButton->setIcon(icon11);
        tabWidget->addTab(Ajout, QString());
        liste_2 = new QWidget();
        liste_2->setObjectName("liste_2");
        tableView = new QTableView(liste_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(30, 50, 821, 311));
        afficher = new QPushButton(liste_2);
        afficher->setObjectName("afficher");
        afficher->setGeometry(QRect(910, 50, 111, 31));
        afficher->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon12(QIcon::fromTheme(QIcon::ThemeIcon::SystemReboot));
        afficher->setIcon(icon12);
        pushButton_modifier = new QPushButton(liste_2);
        pushButton_modifier->setObjectName("pushButton_modifier");
        pushButton_modifier->setGeometry(QRect(910, 100, 111, 31));
        pushButton_modifier->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(250, 146, 75);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon13(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_modifier->setIcon(icon13);
        pushButton_supprimer = new QPushButton(liste_2);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(910, 150, 111, 31));
        pushButton_supprimer->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(255, 0, 0);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon14(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_supprimer->setIcon(icon14);
        label_5 = new QLabel(liste_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(900, 30, 131, 371));
        pushButton_exporterPDF = new QPushButton(liste_2);
        pushButton_exporterPDF->setObjectName("pushButton_exporterPDF");
        pushButton_exporterPDF->setGeometry(QRect(940, 200, 51, 41));
        pushButton_exporterPDF->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/img/pdf-file-type-symbol_icon-icons.com_57657.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_exporterPDF->setIcon(icon15);
        statt = new QPushButton(liste_2);
        statt->setObjectName("statt");
        statt->setGeometry(QRect(940, 260, 51, 41));
        statt->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/img/1170667-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        statt->setIcon(icon16);
        tri = new QPushButton(liste_2);
        tri->setObjectName("tri");
        tri->setGeometry(QRect(940, 320, 51, 41));
        tri->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/img/sort-ascending_icon-icons.com_73158.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        tri->setIcon(icon17);
        comboRecherchePar = new QComboBox(liste_2);
        comboRecherchePar->setObjectName("comboRecherchePar");
        comboRecherchePar->setGeometry(QRect(80, 380, 211, 22));
        comboRecherchePar->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 11pt \"Segoe UI\";\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 10px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 15px 15px; /* Ajuste la taille de l'ic\303\264ne */"));
        labelIndication = new QLabel(liste_2);
        labelIndication->setObjectName("labelIndication");
        labelIndication->setGeometry(QRect(310, 380, 261, 21));
        labelIndication->setStyleSheet(QString::fromUtf8("font: 900 12pt \"Arial\";\n"
"border : none ;"));
        champRecherche = new QLineEdit(liste_2);
        champRecherche->setObjectName("champRecherche");
        champRecherche->setGeometry(QRect(310, 410, 181, 21));
        champRecherche->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 9pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 20px 20px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 10px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */"));
        btnRechercher = new QPushButton(liste_2);
        btnRechercher->setObjectName("btnRechercher");
        btnRechercher->setGeometry(QRect(470, 400, 91, 41));
        btnRechercher->setStyleSheet(QString::fromUtf8("background: none; /* Supprime le fond */\n"
"qproperty-iconSize: 20px 20px; /* Augmente la taille de l'ic\303\264ne */\n"
"border: none; /* Supprime la bordure si n\303\251cessaire */\n"
"\n"
""));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/img/searchmagnifierinterfacesymbol1_79893.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnRechercher->setIcon(icon18);
        tabWidget->addTab(liste_2, QString());
        label_5->raise();
        tableView->raise();
        afficher->raise();
        pushButton_modifier->raise();
        pushButton_supprimer->raise();
        pushButton_exporterPDF->raise();
        statt->raise();
        tri->raise();
        comboRecherchePar->raise();
        labelIndication->raise();
        champRecherche->raise();
        btnRechercher->raise();
        stat_2 = new QWidget();
        stat_2->setObjectName("stat_2");
        graphicsView = new QGraphicsView(stat_2);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 1061, 531));
        tabWidget->addTab(stat_2, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setEnabled(true);
        groupBox_7->setGeometry(QRect(150, 20, 361, 401));
        groupBox_7->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_23 = new QLabel(groupBox_7);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(30, 20, 81, 21));
        label_23->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        nom_2 = new QLineEdit(groupBox_7);
        nom_2->setObjectName("nom_2");
        nom_2->setGeometry(QRect(100, 10, 161, 41));
        nom_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_24 = new QLabel(groupBox_7);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(20, 50, 141, 31));
        label_24->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_26 = new QLabel(groupBox_7);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(30, 280, 131, 41));
        label_26->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        prenom_2 = new QLineEdit(groupBox_7);
        prenom_2->setObjectName("prenom_2");
        prenom_2->setGeometry(QRect(110, 50, 151, 41));
        prenom_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        telephone_2 = new QLineEdit(groupBox_7);
        telephone_2->setObjectName("telephone_2");
        telephone_2->setGeometry(QRect(110, 90, 161, 41));
        telephone_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        email_2 = new QLineEdit(groupBox_7);
        email_2->setObjectName("email_2");
        email_2->setGeometry(QRect(120, 130, 151, 41));
        email_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_27 = new QLabel(groupBox_7);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(20, 90, 141, 41));
        label_27->setStyleSheet(QString::fromUtf8("border:  transparent ;\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(30, 200, 121, 51));
        label_28->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_29 = new QLabel(groupBox_7);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(10, 240, 271, 51));
        label_29->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_30 = new QLabel(groupBox_7);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(30, 130, 111, 31));
        label_30->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        poste_2 = new QLineEdit(groupBox_7);
        poste_2->setObjectName("poste_2");
        poste_2->setGeometry(QRect(120, 280, 161, 41));
        poste_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        statut_2 = new QLineEdit(groupBox_7);
        statut_2->setObjectName("statut_2");
        statut_2->setGeometry(QRect(120, 210, 151, 41));
        statut_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        date_embauche_2 = new QDateEdit(groupBox_7);
        date_embauche_2->setObjectName("date_embauche_2");
        date_embauche_2->setGeometry(QRect(120, 250, 171, 31));
        date_embauche_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_31 = new QLabel(groupBox_7);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(30, 180, 91, 31));
        label_31->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_32 = new QLabel(groupBox_7);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(30, 320, 201, 41));
        label_32->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        salaire_2 = new QLineEdit(groupBox_7);
        salaire_2->setObjectName("salaire_2");
        salaire_2->setGeometry(QRect(120, 320, 161, 41));
        salaire_2->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        pushButton_pdp2_2 = new QPushButton(groupBox_7);
        pushButton_pdp2_2->setObjectName("pushButton_pdp2_2");
        pushButton_pdp2_2->setGeometry(QRect(290, 10, 61, 61));
        pushButton_pdp2_2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 70px 70px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : transparent ;"));
        pushButton_pdp2_2->setIcon(icon7);
        sexe_2 = new QComboBox(groupBox_7);
        sexe_2->addItem(QString());
        sexe_2->addItem(QString());
        sexe_2->setObjectName("sexe_2");
        sexe_2->setGeometry(QRect(130, 180, 151, 31));
        groupBox_8 = new QGroupBox(tab);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(580, 60, 271, 331));
        groupBox_8->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_33 = new QLabel(groupBox_8);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(80, 20, 141, 31));
        label_33->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 13pt \"Arial Rounded MT\";\n"
""));
        pushButton_pdp_2 = new QPushButton(groupBox_8);
        pushButton_pdp_2->setObjectName("pushButton_pdp_2");
        pushButton_pdp_2->setGeometry(QRect(70, 60, 131, 131));
        pushButton_pdp_2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 80px 80px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        pushButton_pdp_2->setIcon(icon10);
        pushButton_modifierPhoto = new QPushButton(groupBox_8);
        pushButton_modifierPhoto->setObjectName("pushButton_modifierPhoto");
        pushButton_modifierPhoto->setGeometry(QRect(70, 210, 141, 41));
        pushButton_modifierPhoto->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"font: 900 10pt \"Segoe UI\";\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        pushButton_modifierPhoto->setIcon(icon8);
        pushButton_12 = new QPushButton(groupBox_8);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(180, 150, 51, 51));
        pushButton_12->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 40px 40px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/img/chatbot-icon-simple-style-vector-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_12->setIcon(icon19);
        pushButton_appliquerModif = new QPushButton(tab);
        pushButton_appliquerModif->setObjectName("pushButton_appliquerModif");
        pushButton_appliquerModif->setGeometry(QRect(690, 420, 91, 31));
        pushButton_appliquerModif->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName("groupBox_9");
        groupBox_9->setEnabled(true);
        groupBox_9->setGeometry(QRect(360, 50, 291, 291));
        groupBox_9->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 3px solid black ; /* Ajoute une bordure */\n"
""));
        pushButton_2 = new QPushButton(groupBox_9);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(90, 10, 121, 91));
        pushButton_2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 90px 90px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
""));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/Downloads/profile-icon-login-head-icon-vector-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_2->setIcon(icon20);
        lineEdit_nom = new QLineEdit(groupBox_9);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(100, 150, 171, 31));
        lineEdit_nom->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 113, 121);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_35 = new QLabel(groupBox_9);
        label_35->setObjectName("label_35");
        label_35->setGeometry(QRect(30, 150, 81, 31));
        label_35->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 15pt \"Arial Rounded MT\";\n"
"background : none;"));
        pushButton_13 = new QPushButton(groupBox_9);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(30, 190, 61, 31));
        pushButton_13->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
"background : none;"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/Downloads/574710-200.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_13->setIcon(icon21);
        btnAccederTodo = new QPushButton(groupBox_9);
        btnAccederTodo->setObjectName("btnAccederTodo");
        btnAccederTodo->setGeometry(QRect(100, 210, 171, 41));
        btnAccederTodo->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 113, 121);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        btnCheckEmpreinte = new QPushButton(groupBox_9);
        btnCheckEmpreinte->setObjectName("btnCheckEmpreinte");
        btnCheckEmpreinte->setGeometry(QRect(30, 230, 61, 31));
        btnCheckEmpreinte->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
"background : none;"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/Downloads/1230982.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnCheckEmpreinte->setIcon(icon22);
        btnAccederTodo->raise();
        pushButton_2->raise();
        lineEdit_nom->raise();
        label_35->raise();
        pushButton_13->raise();
        btnCheckEmpreinte->raise();
        pushButton_8 = new QPushButton(tab_2);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(430, 360, 161, 31));
        pushButton_8->setStyleSheet(QString::fromUtf8("\n"
"font: 700 14pt \"Arial Rounded MT\";\n"
"background-color: rgb(67, 164, 8);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        label_6 = new QLabel(tab_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(40, 20, 991, 81));
        label_6->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 3px solid black ; /* Ajoute une bordure */\n"
"font: 700 30pt \"Segoe UI Variable\";"));
        tableTodo = new QTableView(tab_3);
        tableTodo->setObjectName("tableTodo");
        tableTodo->setGeometry(QRect(35, 120, 1001, 311));
        pushButton = new QPushButton(tab_3);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(380, 30, 141, 51));
        pushButton->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 90px 90px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
"background : none ;"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/Downloads/To-do-list-icon-Graphics-17618184-1-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon23);
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        groupBox_10 = new QGroupBox(tab_4);
        groupBox_10->setObjectName("groupBox_10");
        groupBox_10->setEnabled(true);
        groupBox_10->setGeometry(QRect(210, 100, 181, 191));
        groupBox_10->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 3px solid black ; /* Ajoute une bordure */\n"
""));
        pushButton_faceId = new QPushButton(groupBox_10);
        pushButton_faceId->setObjectName("pushButton_faceId");
        pushButton_faceId->setGeometry(QRect(10, 30, 171, 131));
        pushButton_faceId->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 150px 150px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : none ;\n"
"background : none ;"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/Downloads/pngtree-application-facial-recognition-icon-vector-png-image_7481810-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_faceId->setIcon(icon24);
        lineEdit_3 = new QLineEdit(tab_4);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(200, 320, 231, 31));
        lineEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"font: 700 11pt \"Arial Rounded MT\";\n"
""));
        groupBox_11 = new QGroupBox(tab_4);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setEnabled(true);
        groupBox_11->setGeometry(QRect(590, 100, 161, 181));
        groupBox_11->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 3px solid black ; /* Ajoute une bordure */\n"
""));
        btnAjouterEmpreinte = new QPushButton(groupBox_11);
        btnAjouterEmpreinte->setObjectName("btnAjouterEmpreinte");
        btnAjouterEmpreinte->setGeometry(QRect(20, 30, 131, 121));
        btnAjouterEmpreinte->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 100px 100px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : none ;\n"
"background : none ;"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/Downloads/3076455.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnAjouterEmpreinte->setIcon(icon25);
        lineEdit_4 = new QLineEdit(tab_4);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(560, 320, 231, 31));
        lineEdit_4->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"font: 700 11pt \"Arial Rounded MT\";\n"
""));
        tabWidget->addTab(tab_4, QString());
        lineEdit_3->raise();
        groupBox_10->raise();
        groupBox_11->raise();
        lineEdit_4->raise();
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(940, 40, 291, 41));
        label_4->setStyleSheet(QString::fromUtf8("background : none ;\n"
"font: 700 18pt \"Arial\";"));
        liste = new QPushButton(groupBox);
        liste->setObjectName("liste");
        liste->setGeometry(QRect(800, 120, 101, 41));
        liste->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 11.5pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 20px 20px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/Desktop/img/570170-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        liste->setIcon(icon26);
        stat = new QPushButton(groupBox);
        stat->setObjectName("stat");
        stat->setGeometry(QRect(930, 120, 111, 41));
        stat->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 12pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 20px 20px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/Desktop/img/1170667-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        stat->setIcon(icon27);
        ajout = new QPushButton(groupBox);
        ajout->setObjectName("ajout");
        ajout->setGeometry(QRect(1070, 120, 111, 41));
        ajout->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 10pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 30px 30px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/Desktop/img/18926930-conception-d-icone-de-formulaire-gratuit-vectoriel-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ajout->setIcon(icon28);
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(-30, 130, 131, 61));
        pushButton_11->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/img/employe.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_11->setIcon(icon29);
        login = new QPushButton(groupBox);
        login->setObjectName("login");
        login->setGeometry(QRect(130, 120, 91, 51));
        login->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 60px 60px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: none ; /* Ajoute une bordure */\n"
"background : none;"));
        QIcon icon30;
        icon30.addFile(QString::fromUtf8(":/Downloads/5238442.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        login->setIcon(icon30);
        liste_3 = new QPushButton(groupBox);
        liste_3->setObjectName("liste_3");
        liste_3->setGeometry(QRect(210, 130, 251, 31));
        liste_3->setStyleSheet(QString::fromUtf8("background-color: rgba(240,200,160,255);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"font: 700 11.5pt \"Segoe UI\";"));
        liste_3->setIcon(icon26);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1274, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        pushButton_10->setText(QString());
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        pushButton_3->setText(QString());
        pushButton_7->setText(QString());
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "S.M.M", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "-Smart Mall Manager-", nullptr));
        pushButton_9->setText(QString());
        groupBox_5->setTitle(QString());
        label_14->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone\n"
"", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Date Embauche", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        sexe->setItemText(0, QCoreApplication::translate("MainWindow", "Homme", nullptr));
        sexe->setItemText(1, QCoreApplication::translate("MainWindow", "Femme", nullptr));

        pushButton_pdp2->setText(QString());
        poste->setItemText(0, QCoreApplication::translate("MainWindow", "chef technicien ", nullptr));
        poste->setItemText(1, QCoreApplication::translate("MainWindow", "caissier", nullptr));
        poste->setItemText(2, QCoreApplication::translate("MainWindow", "electricien", nullptr));

        groupBox_6->setTitle(QString());
        label_25->setText(QCoreApplication::translate("MainWindow", "Photo de profil", nullptr));
        pushButton_choisirImage->setText(QCoreApplication::translate("MainWindow", "Importer image ", nullptr));
        pushButton_analysePDP->setText(QString());
        pushButton_pdp->setText(QString());
        pushButton_ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        cvButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(Ajout), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        afficher->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label_5->setText(QString());
        pushButton_exporterPDF->setText(QString());
        statt->setText(QString());
        tri->setText(QString());
        labelIndication->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        btnRechercher->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(liste_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(stat_2), QCoreApplication::translate("MainWindow", "Page", nullptr));
        groupBox_7->setTitle(QString());
        label_23->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone\n"
"", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Date_Embauche", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        pushButton_pdp2_2->setText(QString());
        sexe_2->setItemText(0, QCoreApplication::translate("MainWindow", "Homme", nullptr));
        sexe_2->setItemText(1, QCoreApplication::translate("MainWindow", "Femme", nullptr));

        groupBox_8->setTitle(QString());
        label_33->setText(QCoreApplication::translate("MainWindow", "Photo de profil", nullptr));
        pushButton_pdp_2->setText(QString());
        pushButton_modifierPhoto->setText(QCoreApplication::translate("MainWindow", "Modifier image", nullptr));
        pushButton_12->setText(QString());
        pushButton_appliquerModif->setText(QCoreApplication::translate("MainWindow", "Enregistrer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Page", nullptr));
        groupBox_9->setTitle(QString());
        pushButton_2->setText(QString());
        label_35->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        pushButton_13->setText(QString());
        btnAccederTodo->setText(QCoreApplication::translate("MainWindow", "Authentification ", nullptr));
        btnCheckEmpreinte->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Page", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "                                     TO DO LIST ", nullptr));
        pushButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Page", nullptr));
        groupBox_10->setTitle(QString());
        pushButton_faceId->setText(QString());
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "      Enregistrer votre face id", nullptr));
        groupBox_11->setTitle(QString());
        btnAjouterEmpreinte->setText(QString());
        lineEdit_4->setText(QCoreApplication::translate("MainWindow", "              Enregistrer votre empreinte", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Page", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251s ", nullptr));
        liste->setText(QCoreApplication::translate("MainWindow", "Liste", nullptr));
        stat->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        ajout->setText(QCoreApplication::translate("MainWindow", "Ajouter\n"
" employe", nullptr));
        pushButton_11->setText(QString());
        login->setText(QString());
        liste_3->setText(QCoreApplication::translate("MainWindow", "Login a la liste des maintenances", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
