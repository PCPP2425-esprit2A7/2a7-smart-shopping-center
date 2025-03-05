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

private slots:

    void on_ajouter_2_clicked();
    void afficherServices();
    void on_deleteButton_clicked();
    void supprimerService(int idService); // Déclaration de la fonction supprimer
    void on_modifyButton_clicked(); // Slot pour le bouton modifier


private:
    Ui::MainWindow *ui;
    void modifierService(int idService, const QString &nom, const QString &description, double cout, const QString &frequence, const QString &statut, const QString &dateDebut, const QString &dateFin);

};
#endif // MAINWINDOW_H

