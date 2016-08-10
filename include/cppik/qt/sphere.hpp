#pragma once

#include <cppik/qt/object.hpp>
#include <cppik/math.hpp>

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace cppik { namespace qt {

struct sphere : public object {

  sphere (Qt3DCore::QEntity * parent, float radius) {
    material_ = new Qt3DExtras::QPhongMaterial(parent);
    entity_ = new Qt3DCore::QEntity(parent);
    mesh_ = new Qt3DExtras::QSphereMesh;
    transform_ = new Qt3DCore::QTransform;

    mesh_->setRadius(radius);

    entity_->addComponent(mesh_);
    entity_->addComponent(transform_);
    entity_->addComponent(material_);

    position_ = {0, 0, 0};
    radius_ = radius;
  }

  void update () override {
    // TODO: atomic op
    if (!require_update_) { return; }
    require_update_ = false;

    auto const & P = position_;

    transform_->setTranslation(QVector3D(P[0], P[1], P[2]));
  }

  float radius () const noexcept {
    return radius_;
  }

  void set_radius (float val) noexcept {
    radius_ = val;
    require_update_ = true;
  }

  void set_position (std::array<float, 3> const & val) noexcept {
    position_ = val;
    require_update_ = true;
  }

  void set_position (float x, float y, float z) noexcept {
    position_[0] = x;
    position_[1] = y;
    position_[2] = z;
    require_update_ = true;
  }

private:
  Qt3DRender::QMaterial * material_;
  Qt3DCore::QEntity * entity_;
  Qt3DExtras::QSphereMesh * mesh_;
  Qt3DCore::QTransform * transform_;

  std::array<float, 3> position_;
  float radius_;
  bool require_update_;
};

}}
