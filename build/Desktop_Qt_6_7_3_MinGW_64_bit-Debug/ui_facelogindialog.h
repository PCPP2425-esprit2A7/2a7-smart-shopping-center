/********************************************************************************
** Form generated from reading UI file 'facelogindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACELOGINDIALOG_H
#define UI_FACELOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FaceLoginDialog
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QFrame *cameraFrame;
    QLabel *statusLabel;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *captureButton;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *FaceLoginDialog)
    {
        if (FaceLoginDialog->objectName().isEmpty())
            FaceLoginDialog->setObjectName("FaceLoginDialog");
        FaceLoginDialog->resize(800, 600);
        FaceLoginDialog->setMinimumSize(QSize(800, 600));
        FaceLoginDialog->setStyleSheet(QString::fromUtf8("QDialog {\n"
"    background-color: #f0f2f5;\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #1a1a1a;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0078d4;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 4px;\n"
"    font-size: 14px;\n"
"    min-width: 100px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #005a9e;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #004578;\n"
"}\n"
"\n"
"QPushButton#captureButton {\n"
"    background-color: #107c10;\n"
"    font-size: 16px;\n"
"    padding: 12px 24px;\n"
"}\n"
"\n"
"QPushButton#captureButton:hover {\n"
"    background-color: #0b5c0b;\n"
"}\n"
"\n"
"QLabel#statusLabel {\n"
"    color: #666666;\n"
"    font-size: 16px;\n"
"    margin: 10px;\n"
"}\n"
"\n"
"QWidget#cameraFrame {\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 8px;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QFrame#mainFrame {\n"
"    background-color: #ffffff;\n"
"    border-radius: 10"
                        "px;\n"
"    padding: 20px;\n"
"}"));
        mainLayout = new QVBoxLayout(FaceLoginDialog);
        mainLayout->setObjectName("mainLayout");
        titleLabel = new QLabel(FaceLoginDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 24px;\n"
"font-weight: bold;\n"
"color: #0078d4;\n"
"margin-bottom: 10px;"));
        titleLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(titleLabel);

        cameraFrame = new QFrame(FaceLoginDialog);
        cameraFrame->setObjectName("cameraFrame");
        cameraFrame->setMinimumSize(QSize(640, 360));
        cameraFrame->setMaximumSize(QSize(16777215, 360));
        cameraFrame->setFrameShape(QFrame::StyledPanel);
        cameraFrame->setFrameShadow(QFrame::Raised);
        cameraFrame->setStyleSheet(QString::fromUtf8("border: 2px solid #cccccc;\n"
"border-radius: 8px;\n"
"background-color: #ffffff;"));

        mainLayout->addWidget(cameraFrame);

        statusLabel = new QLabel(FaceLoginDialog);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("color: #666666;\n"
"font-size: 16px;\n"
"margin: 10px;"));
        statusLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(statusLabel);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        captureButton = new QPushButton(FaceLoginDialog);
        captureButton->setObjectName("captureButton");
        captureButton->setMinimumSize(QSize(180, 40));
        captureButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        captureButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #107c10;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-size: 16px;\n"
"    padding: 8px 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #0b5c0b;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #094509;\n"
"}"));

        buttonLayout->addWidget(captureButton);

        cancelButton = new QPushButton(FaceLoginDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(120, 40));
        cancelButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        cancelButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #d83b01;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-size: 16px;\n"
"    padding: 8px 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #a32d01;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #7a2201;\n"
"}"));

        buttonLayout->addWidget(cancelButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer_2);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(FaceLoginDialog);

        QMetaObject::connectSlotsByName(FaceLoginDialog);
    } // setupUi

    void retranslateUi(QDialog *FaceLoginDialog)
    {
        FaceLoginDialog->setWindowTitle(QCoreApplication::translate("FaceLoginDialog", "Authentification par Reconnaissance Faciale", nullptr));
        titleLabel->setText(QCoreApplication::translate("FaceLoginDialog", "Reconnaissance Faciale", nullptr));
        statusLabel->setText(QCoreApplication::translate("FaceLoginDialog", "Positionnez votre visage dans le cadre", nullptr));
        captureButton->setText(QCoreApplication::translate("FaceLoginDialog", "\360\237\223\270 Capturer", nullptr));
        cancelButton->setText(QCoreApplication::translate("FaceLoginDialog", "Annuler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FaceLoginDialog: public Ui_FaceLoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACELOGINDIALOG_H
