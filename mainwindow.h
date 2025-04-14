#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "openaiclient.h"
#include <QMainWindow>
#include <QPushButton>
#include <QEvent>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <qtextedit.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    explicit HoverButton(QWidget *parent = nullptr);  // ✅ Déclaration correcte

signals:
    void hoverEntered();
    void hoverLeft();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};



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
    //void supprimerService(int idService); // Déclaration de la fonction supprimer
    void on_modifyButton_clicked(); // Slot pour le bouton modifier
    void afficherStatistiques();
    void on_pdf_clicked();
    void changerCouleurBouton();
    void on_save_stat_clicked();
    // Slot pour l'effet de survol et d'avancement des boutons de navigation
    void onEnterNavigationButton();
    void onLeaveNavigationButton();
    void filtrerServicesParStatut(const QString &statut);
    void trierServices();
    void on_saveButton_clicked();
    void rechercherService(const QString &searchText);
    void envoyerRequete();
    void executerRequeteSQL(const QString &sqlQuery);
    void genererDescriptionIA();
    void afficherDescription(const QString &description);
    void on_lineEdit_id_textChanged(const QString &text);
    void on_lineEdit_id_textChanged2(const QString &text);


private:


    // Fonction pour configurer les boutons de navigation (connexion des événements)
    void setUpNavigationButtons();
    void loadServiceDetails(int id);
     int idServiceAModifier = -1;
    //QSqlQueryModel *tableModel;  // Déclare un modèle SQL
    OpenAIClient *openAIClient;
    Ui::MainWindow *ui;
    QTextEdit *textBrowserReponse;  // Déclaration du QTextBrowser


};
#endif // MAINWINDOW_H

