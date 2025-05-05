#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTextBrowser>
#include <QListWidget>
#include <QListView>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTimer>
#include <QEvent>
#include <QPointer>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QMap>
#include <QVector>
#include <QDate>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QPropertyAnimation>
#include <QProcess>
#include <QCamera>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QImageCapture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QGraphicsScene>

#include "arduino.h"
#include "chatmanager.h"
#include "openaiclient.h"
#include "chatwindow.h"
#include "employe.h"
#include "facelogindialog.h"
#include "event.h"
#include "dimensionsdialog.h"
#include "espace3dview.h"
#include "qcombobox.h"





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Déclaration de la classe FrameEventFilter
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void afficherLocataires();


    void afficherEmployeSelectionne(const Employe &employe);

    //_______________maleeek___________________

    QString cheminImagePDPEVENT;
    void afficherStatistiquesEvent();
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

    //_________________yassmine_____________


    void on_pushButton_ajouter_clicked();
    void on_pushButton_choisirImage_clicked();
    void afficherEmployes();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_exporterPDF_clicked();
    void trierEmployes();
    void afficherStatistiques();
    void rechercherEmployes();
    void setEmploye(const Employe &employe);
    Employe getEmployeModifie() const;
    void on_pushButton_modifierPhoto_clicked();
    void on_pushButton_enregistrer_clicked();
    void on_pushButton_appliquerModif_clicked();
    void afficherDetailsEmploye(const QModelIndex &index);
    void onPhotoClicked(const QModelIndex &index);
    void on_pushButton_analysePDP_clicked();
    void saveImageToDatabase(const QPixmap &pixmap) ;
    void on_btnVerifier_clicked();



    void genererPDF(const QMap<QString, QVariant>& employee);
    void chargementTodoList();
    void on_btnAccederTodo_clicked();  // Ajouter cette ligne
    void toutMarquerCommeTermine();
    void on_pushButton_faceId_clicked();
    void on_cvButton_clicked();

    void on_btnAjouterEmpreinte_clicked();  // Nouveau bouton
    void onEmpreinteEnregistree(int id);
    void onErreurArduino(const QString &message);
    void on_btnCheckEmpreinte_clicked();

    void slotEmpreinteReconnue(int id);
    void slotEmpreinteNonTrouvee();
    void genererBadgePDF(const QVariantMap &employee);
    void suggestEmailsByIA(const QString& prenom, const QString& nom);
    void envoyerEmailSelonStatut(const QString& nom, const QString& prenom, const QString& email, const QString& statut);



    //____________________youssef_________________
    void on_ajouter_srv_clicked();
    void afficherServices();
    void on_deleteButton_clicked();
    //void supprimerService(int idService); // Déclaration de la fonction supprimer
    void on_modifier_srv_clicked(); // Slot pour le bouton modifier
    void afficherStatistiquesServ();
    void on_pdf_clicked();
    void changerCouleurBouton();
    void on_save_stat_clicked();
    // Slot pour l'effet de survol et d'avancement des boutons de navigation
    void onEnterNavigationButton();
    void onLeaveNavigationButton();
    // void filtrerServicesParStatut(const QString &statut);
    void trierServices();
    void on_save_srv_clicked();
    void rechercherService(const QString &searchText);
    void envoyerRequete();
    void executerRequeteSQL(const QString &sqlQuery);
    void genererDescriptionIA();
    void afficherDescription(const QString &description);
    void on_lineEdit_id_textChanged(const QString &text);
    void on_lineEdit_id_textChanged2(const QString &text);
    void on_btnVoix_clicked();
    void readVoiceOutput();
    void processVoiceFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void openChatWindow() ;
    void updateChatSenderName(int index);





    //_______________________MALEK____________________________

    void on_ajouter_button_ev_clicked();
    void on_pushButton_choisirImage_event_clicked();
    void afficherEvenement();
    void on_supprimer_event_clicked();
    void on_pdf_ev_clicked();
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
    void on_valide_date_clicked();
    void on_calendrier_clicked();
    void on_todo_ok_clicked();
    void on_table_todo_ev_clicked(const QModelIndex &index);
    void on_btnVoix_ev_clicked();



    void on_pushButton_save_modif_ev_clicked();
    void chargerDetailsEvenement(int eventId);
    void on_pushButton_modif_affiche_clicked();
    void on_voiraffiche_clicked();
    void on_btnUpdate_ev_clicked();




    //_____________Ahmed_________________


    void validerChamp(QLineEdit* champ, QLabel* labelErreur, const QString& message, bool estValide);
    void validerChamp2(QWidget* champ, QLabel* labelErreur, const QString& message, bool estValide);

    void afficher_maintenance();
    void afficherDescriptionComplete(const QModelIndex &index);
    void afficherDetailsEmployeMain(const QModelIndex &index);
    void afficherDetailsEspace(const QModelIndex &index);
    void handleTableClick(const QModelIndex &index);






    void on_tabWidget_3_tabBarClicked(int index);
    void on_supprimer_maintenance_clicked();
    void on_modifier_maintenance_clicked();
    void on_update_maintenance_clicked();

    void on_liste_maintenance_clicked();

    void on_ajouter_maintenance_2_clicked();

    void on_exporter_maintenance_clicked();

    //recherche
    void rechercherMaintenance(const QString &searchText);

    //stat
    void afficherStatistiquesMaintenance();
    void on_stat_maintenance_clicked();

    //tri

    void trierMaintenance();

    //analyse
    void on_analyse_maintenance_clicked();
    QString nettoyerTexte(const QString& texte);
    QString normaliserTexte(const QString &texte);

    //SMS
    void sendSMS(const QString& phoneNumber, const QString& message);

    //badge
    void on_badge_clicked();

    void on_ajouter_maintenance_clicked();


    //menu
    void changerBoutonSelectionne(QPushButton* boutonSelectionne);
    void initialiserNavigation();

    //badge
    void on_badge_maintenance_clicked();




    //______________insaf______________


    void on_afficher_clicked();
    void on_supprimer_espace_clicked();
    void on_modifier_espace_clicked();
    void chargerDonneesEspace(int id);

    void on_exporterPDF_clicked();
    void on_liste_clicked();
    void on_lineEdit_Nom_textChanged(const QString &text);
    void on_stat_2_clicked();
    void rechercherEspaces();
    void on_id_locataire_entered();
    void on_map_clicked();
    void on_space_clicked(const QString &spaceName);
    void createMapZones(QGraphicsScene* scene);
    void on_enregistrer_mod_espace_clicked();
    void loadSpacesFromDatabase(QGraphicsScene* scene, QComboBox* spaceSelector, QGraphicsView* view);
    void showMapView();


    void on_tri_espaces_clicked();


    void on_ajouterespace_clicked();
    void on_lineEdit_id_locataire_2_espace_editingFinished();

    //__________________HAMZA______________________



    void on_btnAjouterLocataire_clicked() ;
    void on_btnSupprimerLocataire_clicked();
    void on_btnModifierLocataire_clicked();
    void trierlocataire();
    void exporterPDF_locataire();
    void rechercherLocataire(const QString &searchText);
    void on_upload_clicked();
    void envoyerImageAOpenAI(const QString &cheminImage);
    void traiterReponseOpenAI(QNetworkReply *reply);
    //void analyserDocument();
    void afficherStatistiques_locataire();
    void genererContratDepuisOpenAI();
    void envoyerPromptAOpenAI(const QString& prompt);
    void verifierEmail(const QString &text);



private:

    //_______________________yassmine_________________

    Ui::MainWindow *ui;
    QString cheminImagePDP;
    int employeSelectionneId;
    Employe Etmp;
    Employe employe;
    QVariantMap currentEmployeeData;
    void analyzeImage(QPixmap newImage);
    QCamera *camera;
    QVideoWidget *viewfinder;
    QMediaCaptureSession *captureSession;
    QImageCapture *imageCapture;
    bool isCameraActive;
    QByteArray faceIdTemp;
    QByteArray empreinteTemp;
    enum Mode { Idle, Enroll, Validate } fingerprintMode = Idle;

    QByteArray analyzeImage(const QByteArray &imageData);
    QSqlQueryModel *todoModel = nullptr;
    bool compareFaces(const QString &tempImage, const QString &storedImage);
    QString extractInfo(const QString& content, const QRegularExpression& regex);
    Arduino *arduino;
    int dernierIdEmploye; // Pour stocker le dernier ID



    //____________________youssef________________
    // Fonction pour configurer les boutons de navigation (connexion des événements)
    void setUpNavigationButtons();
    void loadServiceDetails(int id);
    int idServiceAModifier = -1;
    //QSqlQueryModel *tableModel;  // Déclare un modèle SQL
    OpenAIClient *openAIClient;
    QTextEdit *textBrowserReponse;  // Déclaration du QTextBrowser
    QProcess *process = nullptr;
    QString bufferOutput;
    ChatManager *m_chatManager; // <-- Ajoutez cette ligne
    QPointer<chatwindow> m_chatWindow;  // <-- Ajoutez cette ligne



    //____________________________MALEK______________________


    QList<QList<QString>> getEventData();
    void setupMenuButtonAnimations(); // Fonction d'animation pour les boutons du menu
    QMap<QPushButton*, QPoint> initialPositions; // Sauvegarde des positions initiales
    QSqlQueryModel *statModel;
    void updateEventDisplay(const QList<QList<QString>> &eventData);
    QSqlTableModel *model;

    QMap<QDate, QStringList> evenementsParDate;
    QStringListModel *eventModel;  // Déclare eventModel ici
    void updateEventList(const QDate &date);  // Méthode pour mettre à jour la liste des événements
    QDate currentdate;
    int roww,columnn;
    int colummnn;
    QTableWidget *tablewidget;
    QSqlQuery query;
    QStandardItemModel *todoModelEvent;
    int idEvenementAModifier=-1;
    Evenement evenementCourant;



    //___________Ahmed_______________


    QSortFilterProxyModel *maintenanceProxyModel = nullptr;
    Qt::SortOrder lastSortOrder = Qt::AscendingOrder;
    int lastSortedColumn = -1;
    QSqlQueryModel *maintenanceModel;
    QString analyseMaintenanceResult;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;



    //_____________insaf_______________


    QCompleter* completer;
    QNetworkAccessManager* networkManager;
    int espaceID_a_modifier = -1;
    QGraphicsScene* m_scene; // Ajoutez cette ligne


    void getAISuggestions(const QString &inputText);
    void afficherEspaces();
    void show3DView(const QString &spaceName);


    //_____________HAMZA________________

    QString cheminContrat;


protected:
    bool eventFilter(QObject *object, QEvent *event) override;

};

QPixmap createRoundedPixmap(const QPixmap &src, int size);

#endif // MAINWINDOW_H
