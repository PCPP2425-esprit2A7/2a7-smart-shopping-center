#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString cheminImagePDP;


private slots:
    void on_pushButton_ajouter_clicked();

    void on_pushButton_ajouter_clicked_maintenace();
    void afficher_maintenance();



    void on_pushButton_choisirImage_clicked();
    void afficherEmployes();


    void on_tabWidget_3_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
