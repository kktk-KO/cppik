#pragma once

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace cppik { namespace qt {

Qt3DCore::QTransform * create_sphere (Qt3DCore::QEntity * root, float radius) {
  Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(root);
  Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(root);
  Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh;
  sphereMesh->setRadius(radius);

  Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;

  sphereEntity->addComponent(sphereMesh);
  sphereEntity->addComponent(sphereTransform);
  sphereEntity->addComponent(material);
  return sphereTransform;
}

}}
