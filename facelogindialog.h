#ifndef FACELOGINDIALOG_H
#define FACELOGINDIALOG_H

#include <QDialog>
#include <QCamera>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QByteArray>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
namespace Ui {
class FaceLoginDialog;
}

class FaceLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaceLoginDialog(QWidget *parent = nullptr);
    ~FaceLoginDialog();

    // Retourne l'ID de l'employé authentifié
    QString getAuthenticatedEmployeeId() const { return authenticatedEmployeeId; }

    // Fonction statique pour enregistrer le FaceID
    static bool registerFaceID(QWidget* parent, const QString& employeeId);

private slots:
    void onCaptureClicked();
    void onImageCaptured(int id, const QImage &preview);
    void processFaceRecognition();


private:
    Ui::FaceLoginDialog *ui;
    QCamera *camera;
    QVideoWidget *viewfinder;
    QMediaCaptureSession *captureSession;
    QImageCapture *imageCapture;
    QImage capturedImage;
    QString tempImagePath;
    QString authenticatedEmployeeId;
    QLabel *frameLabel;

    // Variables membres
    bool isRegistrationMode = false;
    QString currentEmployeeId;

    // Fonctions membres
    bool compareFaces(const QString &tempImage, const QString &storedImage);
    QString findMatchingEmployee(const QString &tempImage);
    bool saveFaceIDToDatabase(const QString &employeeId, const QImage &faceImage);
    QImage detectAndDrawFacialLandmarks(const QImage &image);
    void drawFaceFrame(QPainter &painter, const QRect &faceRect);
};

#endif // FACELOGINDIALOG_H
