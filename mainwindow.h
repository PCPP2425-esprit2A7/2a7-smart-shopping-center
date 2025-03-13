#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QString>
#include <QSqlQueryModel>
#include <QPushButton>  // Inclure QPushButton ici
#include <QPropertyAnimation>
#include <QDebug>
#include <QSqlError>
class Dialog;

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
    QString cheminImagePDP;
   void afficherStatistiques();


private slots:
   void on_ajouter_button_clicked();
    void on_pushButton_choisirImage_clicked();
    void afficherEvenement();
    void on_supprimer_clicked();
    void on_btnDialog_clicked();
    void on_pdf_clicked();


    //void on_stat_clicked();

private:
    Ui::MainWindow *ui;
    QList<QList<QString>> getEventData();
    void setupMenuButtonAnimations(); // Fonction d'animation pour les boutons du menu
    QMap<QPushButton*, QPoint> initialPositions; // Sauvegarde des positions initiales
    //QSqlQueryModel *statModel;




};
#endif // MAINWINDOW_H
