#include "espace3dview.h"
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QOrbitCameraController>
#include <QVBoxLayout>
#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QBuffer>

/*
Espace3DView::Espace3DView(float largeur, float hauteur, float profondeur, QWidget *parent)
    : QWidget(parent)
{
    // Create 3D window
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    // Create root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Create cuboid mesh
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setXExtent(largeur);
    cuboid->setYExtent(hauteur);
    cuboid->setZExtent(profondeur);

    // Create material
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x6b6b6d)));
    material->setAmbient(QColor(QRgb(0x4d4d4f)));
    material->setSpecular(QColor(QRgb(0xffffff)));
    material->setShininess(100.0f);

    // Create transform
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(QVector3D(0, 0, 0));

    // Create entity
    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(rootEntity);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(material);
    cuboidEntity->addComponent(transform);

    // Set up camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Add camera controller
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed(50.0f);
    camController->setLookSpeed(180.0f);
    camController->setCamera(camera);

    // Set root entity
    view->setRootEntity(rootEntity);

    // Create container widget
    QWidget *container = QWidget::createWindowContainer(view);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(container);
}*/
