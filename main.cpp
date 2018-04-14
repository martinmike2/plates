
#include <QApplication>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>

#include <QtCore>
#include "orbittransformcontroller.h"

Qt3DCore::QEntity *createScene()
{
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    Qt3DCore::QEntity *sphere = new Qt3DCore::QEntity(rootEntity);
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh;
    sphereMesh->setRadius(3);

    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;
    OrbitTransformController *controller = new OrbitTransformController(sphereTransform);
    controller->setTarget(sphereTransform);
    controller->setRadius(20.0f);

    QPropertyAnimation *sphereRotateTransformAnimation = new QPropertyAnimation(sphereTransform);
    sphereRotateTransformAnimation->setTargetObject(controller);
    sphereRotateTransformAnimation->setPropertyName("angle");
    sphereRotateTransformAnimation->setStartValue(QVariant::fromValue(0));
    sphereRotateTransformAnimation->setEndValue(QVariant::fromValue(360));
    sphereRotateTransformAnimation->setDuration(10000);
    sphereRotateTransformAnimation->setLoopCount(-1);
    sphereRotateTransformAnimation->start();

    sphere->addComponent(sphereMesh);
    sphere->addComponent(sphereTransform);
    sphere->addComponent(material);

    return rootEntity;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *scene = createScene();

    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0,0,40.0f));
    camera->setViewCenter(QVector3D(0,0,0));

    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
    camController->setLinearSpeed(50.0f);
    camController->setLookSpeed(150.0f);
    camController->setCamera(camera);


    view.setRootEntity(scene);
    view.show();

    return a.exec();
}
