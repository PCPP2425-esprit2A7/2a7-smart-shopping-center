#include "cameradialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMediaDevices>

CameraDialog::CameraDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Enregistrement Face ID");
    setMinimumSize(800, 600);
    setModal(true);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Conteneur pour la caméra et le cadre
    QWidget *cameraContainer = new QWidget(this);
    cameraContainer->setMinimumSize(800, 450);
    cameraContainer->setStyleSheet("background-color: black;");
    mainLayout->addWidget(cameraContainer);

    // Viewfinder
    m_viewfinder = new QVideoWidget(cameraContainer);
    m_viewfinder->setGeometry(0, 0, 800, 450);

    // Cadre vert
    m_frameWidget = new QWidget(cameraContainer);
    m_frameWidget->setStyleSheet("border: 10px solid #00ff00; border-radius: 25px; background-color: rgba(0, 255, 0, 0.1);");
    updateFramePosition();

    // Instructions
    QLabel *instructionLabel = new QLabel("Positionnez votre visage dans le cadre vert", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet(
        "color: #00ff00;"
        "font-weight: bold;"
        "font-size: 24px;"
        "padding: 15px;"
        "background-color: rgba(0, 0, 0, 0.8);"
        "border-radius: 15px;"
        );
    mainLayout->addWidget(instructionLabel);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_captureButton = new QPushButton("📸 Capturer", this);
    QPushButton *cancelButton = new QPushButton("❌ Annuler", this);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 15px 40px;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "   font-size: 18px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:disabled { background-color: #cccccc; }";

    m_captureButton->setStyleSheet(buttonStyle);
    cancelButton->setStyleSheet(buttonStyle.replace("#4CAF50", "#f44336").replace("#45a049", "#da190b"));

    buttonLayout->addWidget(m_captureButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Configuration de la caméra
    setupCamera();

    // Connexions
    connect(m_captureButton, &QPushButton::clicked, this, &CameraDialog::captureImage);
    connect(cancelButton, &QPushButton::clicked, this, &CameraDialog::reject);

    // Timer pour le redimensionnement
    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setSingleShot(true);
    connect(m_resizeTimer, &QTimer::timeout, this, &CameraDialog::updateFramePosition);
}

CameraDialog::~CameraDialog()
{
    stopCamera();
}

void CameraDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    m_resizeTimer->start(100); // Délai pour éviter trop de mises à jour
}

void CameraDialog::setupCamera()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune caméra disponible");
        return;
    }

    m_captureSession = new QMediaCaptureSession(this);
    m_camera = new QCamera(cameras.first(), this);
    m_imageCapture = new QImageCapture(this);

    m_captureSession->setCamera(m_camera);
    m_captureSession->setVideoOutput(m_viewfinder);
    m_captureSession->setImageCapture(m_imageCapture);

    connect(m_imageCapture, &QImageCapture::imageCaptured, this, &CameraDialog::processImage);
    connect(m_camera, &QCamera::errorOccurred, this, &CameraDialog::handleCameraError);

    m_camera->start();
}

void CameraDialog::updateFramePosition()
{
    if (!m_frameWidget || !m_viewfinder)
        return;

    int frameSize = qMin(m_viewfinder->width(), m_viewfinder->height()) * 0.7;
    m_frameWidget->setGeometry(
        (m_viewfinder->width() - frameSize) / 2,
        (m_viewfinder->height() - frameSize) / 2,
        frameSize,
        frameSize
        );
    m_frameWidget->raise();
}

void CameraDialog::stopCamera()
{
    if (m_camera && m_camera->isActive())
        m_camera->stop();

    delete m_imageCapture;
    delete m_captureSession;
    delete m_camera;

    m_imageCapture = nullptr;
    m_captureSession = nullptr;
    m_camera = nullptr;
}

void CameraDialog::captureImage()
{
    if (m_imageCapture && m_imageCapture->isReadyForCapture()) {
        m_captureButton->setEnabled(false);
        m_imageCapture->capture();
    }
}

void CameraDialog::processImage(int id, const QImage &image)
{
    Q_UNUSED(id);
    // Traitement de l'image capturée...
    emit imageCaptured(image);
    accept();
}

void CameraDialog::handleCameraError(QCamera::Error error, const QString &errorString)
{
    Q_UNUSED(error);
    QMessageBox::warning(this, "Erreur Caméra", errorString);
}
