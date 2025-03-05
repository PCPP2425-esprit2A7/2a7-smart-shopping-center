#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "employe.h"

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

    void on_pushButton_choisirImage_clicked();
    void afficherEmployes();
    void on_pushButton_supprimer_clicked();
    //void on_pushButton_modifier_clicked();
    //void on_pushButton_enregistrerModif_clicked();
    //void on_tableView_clicked(const QModelIndex &index); // Ajoutez cette ligne


private:
    Ui::MainWindow *ui;
    int employeSelectionneId;  // Initialiser à -1 dans le constructeur
};

#endif // MAINWINDOW_H
