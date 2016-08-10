#pragma once

#include <cppik/qt/object.hpp>
#include <cppik/math.hpp>

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace cppik { namespace qt {

struct cylinder : public object {

  using value_type = float;

  cylinder (Qt3DCore::QEntity * parent, value_type radius, value_type length) {
    material_ = new Qt3DExtras::QPhongMaterial(parent);
    entity_ = new Qt3DCore::QEntity(parent);
    mesh_ = new Qt3DExtras::QCylinderMesh;
    transform_ = new Qt3DCore::QTransform;

    mesh_->setRadius(radius);
    mesh_->setLength(length);
    mesh_->setRings(100);
    mesh_->setSlices(20);

    entity_->addComponent(mesh_);
    entity_->addComponent(transform_);
    entity_->addComponent(material_);

    position_ = {0, 0, 0};
    radius_ = radius;
    length_ = length; 
  }

  void update () override {
    // TODO: atomic op
    if (!require_update_) { return; }
    require_update_ = false;

    value_type theta = theta_;
    value_type phi = phi_;
    value_type st, ct;
    std::tie(st, ct) = sincos(theta);
    value_type sp, cp;
    std::tie(sp, cp) = sincos(phi);

    auto const & P = position_;
    value_type l = length_;

    transform_->setTranslation(QVector3D(
      P[0] + l * sp * ct / 2,
      P[1] + l * sp * st / 2,
      P[2] + l * cp / 2
    ));

    transform_->setRotation(
      QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 360 * theta / (M_PI * 2) - 90) *
      QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), - 360 * phi/ (M_PI * 2) + 90)
    );

  }

  value_type theta () const noexcept {
    return theta_;
  }

  value_type phi () const noexcept {
    return phi_;
  }

  value_type radius () const noexcept {
    return radius_;
  }

  value_type length () const noexcept {
    return length_;
  }

  void set_theta (value_type val) noexcept {
    theta_ = val;
    require_update_ = true;
  }

  void set_phi (value_type val) noexcept {
    phi_ = val;
    require_update_ = true;
  }

  void set_radius (value_type val) noexcept {
    radius_ = val;
    require_update_ = true;
  }

  void set_length (value_type val) noexcept {
    length_ = val;
    require_update_ = true;
  }

  std::array<value_type, 3> const & position () const noexcept {
    return position_;
  }

  void set_position (std::array<value_type, 3> const & val) noexcept {
    position_ = val;
  }

  void set_position (value_type x, value_type y, value_type z) noexcept {
    position_[0] = x;
    position_[1] = y;
    position_[2] = z;
  }

private:
  Qt3DRender::QMaterial * material_;
  Qt3DCore::QEntity * entity_;
  Qt3DExtras::QCylinderMesh * mesh_;
  Qt3DCore::QTransform * transform_;

  std::array<value_type, 3> position_;
  value_type radius_;
  value_type length_;
  value_type theta_;
  value_type phi_;

  bool require_update_;
};

}}
