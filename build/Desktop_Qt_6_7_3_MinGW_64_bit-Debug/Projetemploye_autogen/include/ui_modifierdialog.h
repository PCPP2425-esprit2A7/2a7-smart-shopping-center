/********************************************************************************
** Form generated from reading UI file 'modifierdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIERDIALOG_H
#define UI_MODIFIERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ModifierDialog
{
public:
    QGroupBox *groupBox;
    QPushButton *pushButton_10;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_3;
    QPushButton *pushButton_7;
    QLabel *label;
    QPushButton *pushButton_9;
    QLabel *label_4;
    QPushButton *liste;
    QPushButton *stat;
    QPushButton *ajout;
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
    QComboBox *sexe;
    QGroupBox *groupBox_6;
    QLabel *label_25;
    QPushButton *pushButton_pdp1;
    QPushButton *pushButton_modifierPhoto;
    QPushButton *pushButton_enregistrer;
    QPushButton *pushButton_pdp2;
    QPushButton *pushButton_11;

    void setupUi(QDialog *ModifierDialog)
    {
        if (ModifierDialog->objectName().isEmpty())
            ModifierDialog->setObjectName("ModifierDialog");
        ModifierDialog->resize(1045, 656);
        groupBox = new QGroupBox(ModifierDialog);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 0, 1091, 781));
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
        label->setGeometry(QRect(210, 20, 751, 81));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 25px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(230, 40, 41, 41));
        pushButton_9->setStyleSheet(QString::fromUtf8("background : transparent ;\n"
"background-color: transparent;\n"
"qproperty-iconSize: 30px 30px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon6(QIcon::fromTheme(QIcon::ThemeIcon::SystemLogOut));
        pushButton_9->setIcon(icon6);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(470, 50, 291, 41));
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
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Desktop/img/570170-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        liste->setIcon(icon7);
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
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Desktop/img/1170667-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        stat->setIcon(icon8);
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
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Desktop/img/18926930-conception-d-icone-de-formulaire-gratuit-vectoriel-removebg-preview[1].png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ajout->setIcon(icon9);
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setEnabled(true);
        groupBox_5->setGeometry(QRect(180, 190, 781, 411));
        groupBox_5->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border: 2px solid black ; /* Ajoute une bordure */\n"
"border-radius: 25px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_14 = new QLabel(groupBox_5);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(110, 20, 81, 21));
        label_14->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        nom = new QLineEdit(groupBox_5);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(200, 10, 161, 41));
        nom->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_15 = new QLabel(groupBox_5);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(100, 60, 141, 31));
        label_15->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(110, 280, 131, 41));
        label_16->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        prenom = new QLineEdit(groupBox_5);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(210, 50, 151, 41));
        prenom->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        telephone = new QLineEdit(groupBox_5);
        telephone->setObjectName("telephone");
        telephone->setGeometry(QRect(210, 90, 161, 41));
        telephone->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        email = new QLineEdit(groupBox_5);
        email->setObjectName("email");
        email->setGeometry(QRect(210, 130, 151, 41));
        email->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        label_17 = new QLabel(groupBox_5);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(100, 100, 141, 41));
        label_17->setStyleSheet(QString::fromUtf8("border:  transparent ;\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        label_18 = new QLabel(groupBox_5);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(100, 200, 121, 51));
        label_18->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(70, 240, 271, 51));
        label_19->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(100, 140, 111, 31));
        label_20->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
"background : none ;"));
        poste = new QLineEdit(groupBox_5);
        poste->setObjectName("poste");
        poste->setGeometry(QRect(200, 280, 161, 41));
        poste->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        statut = new QLineEdit(groupBox_5);
        statut->setObjectName("statut");
        statut->setGeometry(QRect(200, 210, 151, 41));
        statut->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"\n"
""));
        date_embauche = new QDateEdit(groupBox_5);
        date_embauche->setObjectName("date_embauche");
        date_embauche->setGeometry(QRect(190, 250, 171, 31));
        date_embauche->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 15px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        label_21 = new QLabel(groupBox_5);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(100, 180, 91, 31));
        label_21->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        label_22 = new QLabel(groupBox_5);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(110, 320, 201, 41));
        label_22->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 12pt \"Arial Rounded MT\";\n"
""));
        salaire = new QLineEdit(groupBox_5);
        salaire->setObjectName("salaire");
        salaire->setGeometry(QRect(200, 320, 161, 41));
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
        sexe->setGeometry(QRect(200, 180, 151, 31));
        groupBox_6 = new QGroupBox(groupBox_5);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(460, 40, 241, 331));
        groupBox_6->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 186, 196);\n"
"border : none ;"));
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(30, 30, 141, 31));
        label_25->setStyleSheet(QString::fromUtf8("border: 2px solid transparent ; /* Ajoute une bordure */\n"
"font: 700 13pt \"Arial Rounded MT\";\n"
""));
        pushButton_pdp1 = new QPushButton(groupBox_6);
        pushButton_pdp1->setObjectName("pushButton_pdp1");
        pushButton_pdp1->setGeometry(QRect(40, 70, 131, 121));
        pushButton_pdp1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_modifierPhoto = new QPushButton(groupBox_5);
        pushButton_modifierPhoto->setObjectName("pushButton_modifierPhoto");
        pushButton_modifierPhoto->setGeometry(QRect(470, 250, 201, 41));
        pushButton_modifierPhoto->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 126, 171);\n"
"font: 900 10pt \"Segoe UI\";\n"
"border: 12px solid transparent ; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 20px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
""));
        QIcon icon10(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        pushButton_modifierPhoto->setIcon(icon10);
        pushButton_enregistrer = new QPushButton(groupBox_5);
        pushButton_enregistrer->setObjectName("pushButton_enregistrer");
        pushButton_enregistrer->setGeometry(QRect(520, 310, 101, 31));
        pushButton_enregistrer->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"background-color: rgb(132, 194, 49);\n"
"border-radius: 15px;\n"
"font: 700 10pt \"Arial Rounded MT\";"));
        pushButton_pdp2 = new QPushButton(groupBox_5);
        pushButton_pdp2->setObjectName("pushButton_pdp2");
        pushButton_pdp2->setGeometry(QRect(20, 10, 51, 51));
        pushButton_pdp2->setStyleSheet(QString::fromUtf8("qproperty-iconSize: 70px 70px; /* Ajuste la taille de l'ic\303\264ne */\n"
"border : transparent ;\n"
"background : none;"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Downloads/istockphoto-1474007937-170667a-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdp2->setIcon(icon11);
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(-30, 120, 131, 61));
        pushButton_11->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 241, 244);\n"
"border: 2px solid; /* Ajoute une bordure */\n"
"border-image-source: linear-gradient(to right, black, white); /* D\303\251finir le gradient */\n"
"border-image-slice: 1; /* Permet d'appliquer le gradient sur toute la bordure */\n"
"border-radius: 30px; /* Ajuste selon le degr\303\251 d'arrondi souhait\303\251 */\n"
"qproperty-iconSize: 42px 42px; /* Ajuste la taille de l'ic\303\264ne */"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Desktop/img/employe.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_11->setIcon(icon12);

        retranslateUi(ModifierDialog);

        QMetaObject::connectSlotsByName(ModifierDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifierDialog)
    {
        ModifierDialog->setWindowTitle(QCoreApplication::translate("ModifierDialog", "Dialog", nullptr));
        groupBox->setTitle(QString());
        pushButton_10->setText(QString());
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        pushButton_3->setText(QString());
        pushButton_7->setText(QString());
        label->setText(QString());
        pushButton_9->setText(QString());
        label_4->setText(QCoreApplication::translate("ModifierDialog", "Modifier l'employ\303\251 ", nullptr));
        liste->setText(QCoreApplication::translate("ModifierDialog", "Liste", nullptr));
        stat->setText(QCoreApplication::translate("ModifierDialog", "Statistiques", nullptr));
        ajout->setText(QCoreApplication::translate("ModifierDialog", "Ajouter\n"
" employe", nullptr));
        groupBox_5->setTitle(QString());
        label_14->setText(QCoreApplication::translate("ModifierDialog", "Nom", nullptr));
        label_15->setText(QCoreApplication::translate("ModifierDialog", "Pr\303\251nom", nullptr));
        label_16->setText(QCoreApplication::translate("ModifierDialog", "Poste", nullptr));
        label_17->setText(QCoreApplication::translate("ModifierDialog", "T\303\251l\303\251phone\n"
"", nullptr));
        label_18->setText(QCoreApplication::translate("ModifierDialog", "Statut", nullptr));
        label_19->setText(QCoreApplication::translate("ModifierDialog", "Date_Embauche", nullptr));
        label_20->setText(QCoreApplication::translate("ModifierDialog", "Email", nullptr));
        label_21->setText(QCoreApplication::translate("ModifierDialog", "Sexe", nullptr));
        label_22->setText(QCoreApplication::translate("ModifierDialog", "Salaire", nullptr));
        sexe->setItemText(0, QCoreApplication::translate("ModifierDialog", "Homme", nullptr));
        sexe->setItemText(1, QCoreApplication::translate("ModifierDialog", "Femme", nullptr));

        groupBox_6->setTitle(QString());
        label_25->setText(QCoreApplication::translate("ModifierDialog", "Photo de profil", nullptr));
        pushButton_pdp1->setText(QString());
        pushButton_modifierPhoto->setText(QCoreApplication::translate("ModifierDialog", "Modifier Photo de Profil ", nullptr));
        pushButton_enregistrer->setText(QCoreApplication::translate("ModifierDialog", "Enregistrer", nullptr));
        pushButton_pdp2->setText(QString());
        pushButton_11->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ModifierDialog: public Ui_ModifierDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIERDIALOG_H
