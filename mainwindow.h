#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QSortFilterProxyModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QHBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QEnterEvent>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    explicit HoverButton(QWidget *parent = nullptr) : QPushButton(parent) {
        setAttribute(Qt::WA_Hover); // Active la gestion du survol
    }

protected:
    void enterEvent(QEnterEvent *event) override {
        if (event->type() == QEvent::Enter) {
            emit hoverEntered();  // Signale que le curseur est sur le bouton
            qDebug() << "Hover entered";  // Message de débogage
        }
    }

    void leaveEvent(QEvent *event) override {
        if (event->type() == QEvent::Leave) {
            emit hoverLeft();  // Signale que le curseur a quitté le bouton
            qDebug() << "Hover left";  // Message de débogage
        }

        // Appel de la méthode parente (facultatif)
        QPushButton::leaveEvent(event);
    }

signals:
    void hoverEntered();
    void hoverLeft();
};
// ✅ Classe MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString cheminImagePDP;

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_choisirImage_clicked();
    void afficherEmployes();
    void on_pushButton_supprimer_clicked();
    void modifierEmploye();
    void on_pushButton_exporterPDF_clicked();
    void trierEmployes();
    void changerCouleurBouton();
    // ✅ Déclare les slots pour les animations
    void setUpNavigationButtons();
    void onEnterNavigationButton();
    void onLeaveNavigationButton();

private:
    Ui::MainWindow *ui;
    int employeSelectionneId;
    //Employe Etmp;
    QSortFilterProxyModel *proxyModel;
};

#endif // MAINWINDOW_H
