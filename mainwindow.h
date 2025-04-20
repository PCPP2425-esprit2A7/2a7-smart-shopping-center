#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void afficherLocataires();
private slots :
    void on_btnAjouterLocataire_clicked() ;
    void on_btnSupprimerLocataire_clicked();
    void on_btnModifierLocataire_clicked();
    void trierlocataire();
    void exporterPDF();
    void rechercherLocataire(const QString &searchText);
    void on_upload_clicked();
    void envoyerImageAOpenAI(const QString &cheminImage);
    void traiterReponseOpenAI(QNetworkReply *reply);
    //void analyserDocument();
    void afficherStatistiques();

private:
    Ui::MainWindow *ui;
    QString cheminContrat;


};


#endif // MAINWINDOW_H
