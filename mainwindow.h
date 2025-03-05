#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "connection.h"
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
    void on_pushButton_15_clicked();

    void afficherEspaces();

    void on_afficher_clicked();

    void on_supprimer_clicked();

    void on_modifier_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
