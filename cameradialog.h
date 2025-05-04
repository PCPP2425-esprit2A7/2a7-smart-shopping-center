#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QImageCapture>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

class CameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraDialog(QWidget *parent = nullptr);
    ~CameraDialog();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupCamera();
    void updateFramePosition();
    void stopCamera();

private slots:
    void captureImage();
    void processImage(int id, const QImage &image);
    void handleCameraError(QCamera::Error error, const QString &errorString);

signals:
    void imageCaptured(const QImage &image);

private:
    QVideoWidget *m_viewfinder = nullptr;
    QWidget *m_frameWidget = nullptr;
    QPushButton *m_captureButton = nullptr;
    QCamera *m_camera = nullptr;
    QMediaCaptureSession *m_captureSession = nullptr;
    QImageCapture *m_imageCapture = nullptr;
    QTimer *m_resizeTimer = nullptr;
};

#endif // CAMERADIALOG_H
