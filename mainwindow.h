#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMap>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSortFilterProxyModel>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QImageCapture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QWidget>
#include <QEvent>
#include "facelogindialog.h"  // Ajouter cette ligne


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Déclaration de la classe FrameEventFilter
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void afficherEmployeSelectionne(const Employe &employe);

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_choisirImage_clicked();
    void afficherEmployes();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_exporterPDF_clicked();
    void trierEmployes();
    void afficherStatistiques();
    void changerIndication();
    void rechercherEmployes();
    void setEmploye(const Employe &employe);
    Employe getEmployeModifie() const;
    void on_pushButton_modifierPhoto_clicked();
    void on_pushButton_enregistrer_clicked();
    void on_pushButton_appliquerModif_clicked();
    void afficherDetailsEmploye(const QModelIndex &index);
    void onPhotoClicked(const QModelIndex &index);
    void on_pushButton_analysePDP_clicked();
    void saveImageToDatabase(const QPixmap &pixmap) ;
    void on_btnVerifier_clicked();



    void genererPDF(const QMap<QString, QVariant>& employee);
    void chargementTodoList();
    void on_btnAccederTodo_clicked();  // Ajouter cette ligne
    void toutMarquerCommeTermine();
    void on_pushButton_faceId_clicked();

private:
    Ui::MainWindow *ui;
    QString cheminImagePDP;
    int employeSelectionneId;
    Employe Etmp;
    Employe employe;
    QVariantMap currentEmployeeData;
    void analyzeImage(QPixmap newImage);
    QCamera *camera;
    QVideoWidget *viewfinder;
    QMediaCaptureSession *captureSession;
    QImageCapture *imageCapture;
    bool isCameraActive;
    QByteArray faceIdTemp;

    QByteArray analyzeImage(const QByteArray &imageData);
    QSqlQueryModel *todoModel = nullptr;
    bool compareFaces(const QString &tempImage, const QString &storedImage);



};

QPixmap createRoundedPixmap(const QPixmap &src, int size);

#endif // MAINWINDOW_H
