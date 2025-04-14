#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "openaiclient.h"
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
#include <QSqlTableModel>
#include <QMap>
#include <QDate>
#include <QStringList>
#include <QStringListModel>  // Ajouter l'inclusion de QStringListModel
#include <QListView>  // Si tu utilises QListView
#include <QMessageBox>
#include<QMap>
#include<QVector>
#include<qtablewidget.h>
#include <QSqlQuery>
#include<QList>
#include <QDate>

#include <QListWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QDir>
#include <QFileInfoList>
#include <QTimer>
#include <QTextBrowser>
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
    void addTextToTextBrowser(QTextBrowser *textBrowserReponse, const QString &text, bool gauche) ;
    void addquestion(QTextBrowser *textBrowserReponse, const QString &text);
    void addLabelToRightInListWidget(QListWidget *listWidget, const QString &text, const QString &stylesheet, bool gauche, int width) { //mtaa chatbot
        // Créer un nouvel élément pour la liste
        QListWidgetItem *item = new QListWidgetItem(listWidget);

        // Créer un widget personnalisé avec layout
        QWidget *widget = new QWidget(listWidget);
        widget->setStyleSheet("border: none;");

        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(5, 5, 5, 5);

        QLabel *label = new QLabel("", widget);
        label->setStyleSheet(stylesheet);
        label->setFixedWidth(width);
        label->setWordWrap(true);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse); // optionnel

        // Gestion du sens (gauche/droite)
        if (gauche) {
           // texte à gauche
            layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
             layout->addWidget(label);
            label->setText(text); // directement sans animation
        } else {
            layout->addWidget(label);
            layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
             // texte à droite

            // Animation du texte lettre par lettre
            std::shared_ptr<int> index = std::make_shared<int>(0);
            QTimer *timer = new QTimer(widget);

            QObject::connect(timer, &QTimer::timeout, [=]() mutable {
                if (*index < text.length()) {
                    label->setText(text.left(*index + 1)+"\n");
                    label->adjustSize();
                    item->setSizeHint(label->sizeHint());
                    (*index)++;
                } else {
                    timer->stop();
                    timer->deleteLater();
                }
            });

            timer->start(50);
        }

        widget->setLayout(layout);
        item->setSizeHint(widget->sizeHint());
        listWidget->setItemWidget(item, widget);
    }
    void copyTableViewToTableWidget(QTableView *tableView, QTableWidget *tableWidget) { //mtaa tableview l tablewidget
        QAbstractItemModel *model = tableView->model();
        if (!model) return;

        int rowCount = model->rowCount();
        int colCount = model->columnCount();

        // Préparer le TableWidget
        tableWidget->setRowCount(rowCount);
        tableWidget->setColumnCount(colCount);

        // Copier les en-têtes (facultatif)
        for (int col = 0; col < colCount; ++col) {
            QString header = model->headerData(col, Qt::Horizontal).toString();
            tableWidget->setHorizontalHeaderItem(col, new QTableWidgetItem(header));
        }

        // Copier les données cellule par cellule
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                QModelIndex index = model->index(row, col);
                QVariant data = index.data();
                tableWidget->setItem(row, col, new QTableWidgetItem(data.toString()));
            }
        }
    }





private slots:
    void on_ajouter_button_clicked();
    void on_pushButton_choisirImage_clicked();
    void afficherEvenement();
    void on_supprimer_clicked();
    void on_btnDialog_clicked();
    void on_pdf_clicked();
    void on_historique_clicked();
    void on_btn_trierDate_clicked();
    void rechercherevenement();
    void on_dateSelect_dateChanged(const QDate &date);
    void on_flechedroite_clicked();
    void on_flechegauche_clicked();
    void on_pushButton_13moez_2_clicked();
    void on_calendrier_2_cellClicked(int row, int column);
    void on_planifier_clicked();
    void on_supprimer_ev_clicked();
    void on_ticket_clicked();

    void on_todo_clicked();



    void on_valide_date_clicked();



    void on_calendrier_clicked();

    //void on_tableView_activated(const QModelIndex &index);

    //void on_tableView_activated(const QModelIndex &index);

    void on_addTaskButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QList<QString>> getEventData();
    void setupMenuButtonAnimations(); // Fonction d'animation pour les boutons du menu
    QMap<QPushButton*, QPoint> initialPositions; // Sauvegarde des positions initiales
    QSqlQueryModel *statModel;
    void updateEventDisplay(const QList<QList<QString>> &eventData);
    QSqlTableModel *model;
    OpenAIClient *openAIClient;
    QMap<QDate, QStringList> evenementsParDate;
    QStringListModel *eventModel;  // Déclare eventModel ici
    void updateEventList(const QDate &date);  // Méthode pour mettre à jour la liste des événements
    QDate currentdate;
    int roww,columnn;
    int colummnn;
    QTableWidget *tablewidget;
    QSqlQuery query;





};
#endif // MAINWINDOW_H
