#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QStringListModel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_afficher_clicked();
    void on_pushButton_15_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();
    void on_exporterPDF_clicked();
    void on_liste_clicked();
    void on_tri_activated(int index);
    void on_lineEdit_Nom_textChanged(const QString &text);
    void on_stat_2_clicked();
    void on_research_clicked();
    void on_id_locataire_entered();

private:
    Ui::MainWindow *ui;
    QCompleter* completer;
    void setupAutocomplete();
    void afficherEspaces();
};

#endif // MAINWINDOW_H
