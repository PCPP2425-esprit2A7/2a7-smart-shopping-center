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
    QLineEdit *poste;
    QLineEdit *statut;
    QDateEdit *date_embauche;
    QLabel *label_21;
    QLabel *label_22;
    QLineEdit *salaire;
    QPushButton *pushButton_pdp2;
    QComboBox *sexe;
    QGroupBox *groupBox_6;
    QLabel *label_25;
    QPushButton *pushButton_pdp;
    QPushButton *pushButton_choisirImage;
    QPushButton *pushButton_ajouter;
    QPushButton *pushButton_8;
    QWidget *liste_2;
    QTableView *tableView;
    QPushButton *afficher;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_supprimer;
    QLabel *label_5;
    QPushButton *pushButton_exporterPDF;
    QPushButton *pushButton_14;
    QPushButton *boutonTrier;
    QWidget *stat_2;
    QLabel *label_4;
    QPushButton *liste;
    QPushButton *stat;
    QPushButton *ajout;
    QComboBox *comboBox;
    QPushButton *pushButton_11;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1005, 717);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 0, 1011, 741));
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
        icon.addFile(QString::fromUtf8(":/Desktop/img/5973800-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
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
        icon1.addFile(QString::fromUtf8(":/Desktop/img/1623971-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
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
        icon2.addFile(QString::fromUtf8(":/Desktop/img/9321345-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
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
        icon3.addFile(QString::fromUtf8(":/Desktop/img/4986021-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
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
        icon4.addFile(QString::fromUtf8(":/Desktop/img/logo_final-removebg-preview.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
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
        icon5.addFile(QString::fromUtf8(":/Desktop/img/3079026-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_7->setIcon(icon5);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 20, 791, 81));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 25px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(450, 29, 81, 41));
        label_2->setStyleSheet(QString::fromUtf8("background : none ;\n"
"font: 700 22pt \"Arial\";"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(410, 70, 181, 21));
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
        tabWidget->setGeometry(QRect(110, 190, 851, 471));
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
        groupBox_5->setGeometry(QRect(50, 30, 361, 401));
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
        nom->setGeometry(QRect(100, 10, 161, 41));
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
        label_16->setGeometry(QRect(30, 280, 131, 41));
        label_16->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        prenom = new QLineEdit(groupBox_5);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(110, 50, 151, 41));
        prenom->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        telephone = new QLineEdit(groupBox_5);
        telephone->setObjectName("telephone");
        telephone->setGeometry(QRect(110, 90, 161, 41));
        telephone->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        email = new QLineEdit(groupBox_5);
        email->setObjectName("email");
        email->setGeometry(QRect(120, 130, 151, 41));
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
        label_18->setGeometry(QRect(30, 200, 121, 51));
        label_18->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(10, 240, 271, 51));
        label_19->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(30, 130, 111, 31));
        label_20->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        poste = new QLineEdit(groupBox_5);
        poste->setObjectName("poste");
        poste->setGeometry(QRect(120, 280, 161, 41));
        poste->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        statut = new QLineEdit(groupBox_5);
        statut->setObjectName("statut");
        statut->setGeometry(QRect(120, 210, 151, 41));
        statut->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        date_embauche = new QDateEdit(groupBox_5);
        date_embauche->setObjectName("date_embauche");
        date_embauche->setGeometry(QRect(120, 250, 171, 31));
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
        label_22->setGeometry(QRect(30, 320, 201, 41));
        label_22->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        salaire = new QLineEdit(groupBox_5);
        salaire->setObjectName("salaire");
        salaire->setGeometry(QRect(120, 320, 161, 41));
        salaire->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        pushButton_pdp2 = new QPushButton(groupBox_5);
        pushButton_pdp2->setObjectName("pushButton_pdp2");
        pushButton_pdp2->setGeometry(QRect(290, 10, 51, 51));
        pushButton_pdp2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 70px 70px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : transparent ;"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Desktop/img/istockphoto-1474007937-170667a-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdp2->setIcon(icon7);
        sexe = new QComboBox(groupBox_5);
        sexe->addItem(QString());
        sexe->addItem(QString());
        sexe->setObjectName("sexe");
        sexe->setGeometry(QRect(130, 180, 151, 31));
        groupBox_6 = new QGroupBox(Ajout);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(450, 60, 281, 331));
        groupBox_6->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(70, 20, 141, 31));
        label_25->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 13pt \"Arial Rounded MT\";\n"
""));
        pushButton_pdp = new QPushButton(groupBox_6);
        pushButton_pdp->setObjectName("pushButton_pdp");
        pushButton_pdp->setGeometry(QRect(70, 60, 121, 121));
        pushButton_pdp->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 80px 80px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
""));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Desktop/img/add-image-photo-icon.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdp->setIcon(icon8);
        pushButton_choisirImage = new QPushButton(groupBox_6);
        pushButton_choisirImage->setObjectName("pushButton_choisirImage");
        pushButton_choisirImage->setGeometry(QRect(70, 200, 141, 41));
        pushButton_choisirImage->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"font: 900 10pt \"Segoe UI\";\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        QIcon icon9(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
        pushButton_choisirImage->setIcon(icon9);
        pushButton_ajouter = new QPushButton(groupBox_6);
        pushButton_ajouter->setObjectName("pushButton_ajouter");
        pushButton_ajouter->setGeometry(QRect(100, 280, 91, 31));
        pushButton_ajouter->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        pushButton_8 = new QPushButton(groupBox_6);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(170, 140, 51, 51));
        pushButton_8->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 40px 40px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Downloads/artificial_intelligence_ai_robot_technology_icon_179494.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_8->setIcon(icon10);
        tabWidget->addTab(Ajout, QString());
        liste_2 = new QWidget();
        liste_2->setObjectName("liste_2");
        tableView = new QTableView(liste_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(20, 20, 621, 401));
        afficher = new QPushButton(liste_2);
        afficher->setObjectName("afficher");
        afficher->setGeometry(QRect(690, 50, 111, 31));
        afficher->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon11(QIcon::fromTheme(QIcon::ThemeIcon::SystemReboot));
        afficher->setIcon(icon11);
        pushButton_modifier = new QPushButton(liste_2);
        pushButton_modifier->setObjectName("pushButton_modifier");
        pushButton_modifier->setGeometry(QRect(690, 100, 111, 31));
        pushButton_modifier->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(250, 146, 75);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon12(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        pushButton_modifier->setIcon(icon12);
        pushButton_supprimer = new QPushButton(liste_2);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(690, 150, 111, 31));
        pushButton_supprimer->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(255, 0, 0);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        QIcon icon13(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton_supprimer->setIcon(icon13);
        label_5 = new QLabel(liste_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(680, 30, 131, 371));
        pushButton_exporterPDF = new QPushButton(liste_2);
        pushButton_exporterPDF->setObjectName("pushButton_exporterPDF");
        pushButton_exporterPDF->setGeometry(QRect(720, 200, 51, 41));
        pushButton_exporterPDF->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/Desktop/img/pdf-file-type-symbol_icon-icons.com_57657.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_exporterPDF->setIcon(icon14);
        pushButton_14 = new QPushButton(liste_2);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(720, 260, 51, 41));
        pushButton_14->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */\n"
""));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/Desktop/img/1170667-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_14->setIcon(icon15);
        boutonTrier = new QPushButton(liste_2);
        boutonTrier->setObjectName("boutonTrier");
        boutonTrier->setGeometry(QRect(720, 320, 51, 41));
        boutonTrier->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 25px 25px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/Desktop/img/sort-ascending_icon-icons.com_73158.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        boutonTrier->setIcon(icon16);
        tabWidget->addTab(liste_2, QString());
        label_5->raise();
        tableView->raise();
        afficher->raise();
        pushButton_modifier->raise();
        pushButton_supprimer->raise();
        pushButton_exporterPDF->raise();
        pushButton_14->raise();
        boutonTrier->raise();
        stat_2 = new QWidget();
        stat_2->setObjectName("stat_2");
        tabWidget->addTab(stat_2, QString());
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(630, 40, 291, 41));
        label_4->setStyleSheet(QString::fromUtf8("background : none ;\n"
"font: 700 18pt \"Arial\";"));
        liste = new QPushButton(groupBox);
        liste->setObjectName("liste");
        liste->setGeometry(QRect(580, 120, 101, 41));
        liste->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 9pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 20px 20px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/Desktop/img/570170-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        liste->setIcon(icon17);
        stat = new QPushButton(groupBox);
        stat->setObjectName("stat");
        stat->setGeometry(QRect(700, 120, 111, 41));
        stat->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 9pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 20px 20px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        stat->setIcon(icon15);
        ajout = new QPushButton(groupBox);
        ajout->setObjectName("ajout");
        ajout->setGeometry(QRect(830, 120, 111, 41));
        ajout->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 9pt \"Segoe UI\";\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"qproperty-iconSize: 30px 30px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/Desktop/img/18926930-conception-d-icone-de-formulaire-gratuit-vectoriel-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ajout->setIcon(icon18);
        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(110, 140, 191, 22));
        comboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"font: 700 11pt \"Segoe UI\";\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 10px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 15px 15px; /* Ajuste la taille de l'ic\303\264ne */"));
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(-30, 130, 131, 61));
        pushButton_11->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/Desktop/img/employe.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_11->setIcon(icon19);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1005, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


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
        label_19->setText(QCoreApplication::translate("MainWindow", "Date_Embauche", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        pushButton_pdp2->setText(QString());
        sexe->setItemText(0, QCoreApplication::translate("MainWindow", "Homme", nullptr));
        sexe->setItemText(1, QCoreApplication::translate("MainWindow", "Femme", nullptr));

        groupBox_6->setTitle(QString());
        label_25->setText(QCoreApplication::translate("MainWindow", "Photo de profil", nullptr));
        pushButton_pdp->setText(QString());
        pushButton_choisirImage->setText(QCoreApplication::translate("MainWindow", "Importer image ", nullptr));
        pushButton_ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_8->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(Ajout), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        afficher->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label_5->setText(QString());
        pushButton_exporterPDF->setText(QString());
        pushButton_14->setText(QString());
        boutonTrier->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(liste_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(stat_2), QCoreApplication::translate("MainWindow", "Page", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251s ", nullptr));
        liste->setText(QCoreApplication::translate("MainWindow", "Liste", nullptr));
        stat->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        ajout->setText(QCoreApplication::translate("MainWindow", "Ajouter\n"
" employe", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Recherche par ID", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Recherche par Nom", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Recherche par date d'embauche", nullptr));

        pushButton_11->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
