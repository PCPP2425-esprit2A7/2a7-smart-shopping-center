#ifndef ESPACE3DVIEW_H
#define ESPACE3DVIEW_H

#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QVBoxLayout>

class Espace3DView : public QWidget
{
    Q_OBJECT

public:
    explicit Espace3DView(float largeur, float hauteur, float profondeur, QWidget *parent = nullptr);

private:
    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
};

#endif // ESPACE3DVIEW_H
