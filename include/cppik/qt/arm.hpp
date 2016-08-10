#pragma once

#include <QQuaternion>
#include <QVector3D>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace cppik { namespace qt {

struct arm {

  enum RotateAxis {
    X, Y, Z
  };

  using value_type = float;

  arm (Qt3DCore::QEntity * parent, value_type radius, value_type length, RotateAxis axis, unsigned int color) {
    material_ = new Qt3DExtras::QPhongMaterial(parent);
    entity_ = new Qt3DCore::QEntity(parent);
    mesh_ = new Qt3DExtras::QCylinderMesh;
    transform_ = new Qt3DCore::QTransform;

    material_->setDiffuse(QColor(QRgb(color)));

    mesh_->setRadius(radius);
    mesh_->setLength(length);
    mesh_->setRings(100);
    mesh_->setSlices(20);

    entity_->addComponent(mesh_);
    entity_->addComponent(transform_);
    entity_->addComponent(material_);

    radius_ = radius;
    length_ = length;
    axis_ = axis;
  }

  value_type radius () const noexcept {
    return radius_;
  }

  value_type length () const noexcept {
    return length_;
  }

  value_type theta () const noexcept {
    return theta_;
  }

  void set_theta (value_type val) noexcept {
    theta_ = val;
  }

  RotateAxis axis () const noexcept {
    return axis_;
  }

  void set_translation (QVector3D const & origin) noexcept {
    transform_->setTranslation(origin);
  }

  void set_rotation (QQuaternion const & quat) noexcept {
    transform_->setRotation(quat);
  }

private:
  Qt3DExtras::QPhongMaterial * material_;
  Qt3DCore::QEntity * entity_;
  Qt3DExtras::QCylinderMesh * mesh_;
  Qt3DCore::QTransform * transform_;

  value_type radius_;
  value_type length_;
  value_type theta_;
  RotateAxis axis_;

};

}}
