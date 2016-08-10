#pragma once

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace cppik { namespace qt {

Qt3DCore::QTransform * create_cylinder (Qt3DCore::QEntity * root, double radius, double length) {
  Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(root);
  Qt3DCore::QEntity *cylinderEntity = new Qt3DCore::QEntity(root);
  Qt3DExtras::QCylinderMesh *cylinderMesh = new Qt3DExtras::QCylinderMesh;
  cylinderMesh->setRadius(radius);
  cylinderMesh->setLength(length);
  cylinderMesh->setRings(100);
  cylinderMesh->setSlices(20);

  Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform;

  cylinderEntity->addComponent(cylinderMesh);
  cylinderEntity->addComponent(cylinderTransform);
  cylinderEntity->addComponent(material);
  return cylinderTransform;
}

}}
