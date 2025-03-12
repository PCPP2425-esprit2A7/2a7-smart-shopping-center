#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

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


private:
    Ui::MainWindow *ui;
    QString cheminContrat; // 🔥 Ajout de la variable cheminContrat


};


#endif // MAINWINDOW_H
