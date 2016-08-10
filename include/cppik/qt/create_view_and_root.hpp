#pragma once

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <tuple>

namespace cppik { namespace qt {

std::tuple<Qt3DExtras::Qt3DWindow *, Qt3DCore::QEntity *> create_view_and_root () {
  Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
  
  Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
  view->registerAspect(input);

  // Root entity
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

  // Camera
  Qt3DRender::QCamera *cameraEntity = view->camera();

  cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
  cameraEntity->setPosition(QVector3D(20, 0, 5.0f));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));

  // For camera controls
  Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camController->setCamera(cameraEntity);

  // Set root object of the scene
  view->setRootEntity(rootEntity);

  return std::make_tuple(view, rootEntity);
}

}}
