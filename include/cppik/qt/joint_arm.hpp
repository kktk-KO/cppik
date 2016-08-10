#pragma once

#include <cppik/qt/arm.hpp>
#include <cppik/qt/object.hpp>

namespace cppik { namespace qt {

struct joint_arm : object {

  using size_type = std::size_t;
  using value_type = float; 

  void add_arm (Qt3DCore::QEntity * parent, value_type radius, value_type length, arm::RotateAxis axis, unsigned int color) {
    arm_.emplace_back(parent, radius, length, axis, color);
  }

  void update () override {
    QVector3D origin = origin_;
    QQuaternion frame = origin_frame_;
    for (auto & arm : arm_) {
      switch (arm.axis()) {
        case arm::X:
          frame *= QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 180 * arm.theta() / M_PI); break;
        case arm::Y:
          frame *= QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 180 * arm.theta() / M_PI); break;
        case arm::Z:
          frame *= QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 180 * arm.theta() / M_PI); break;
      }
      
      QVector3D x;
      QVector3D y;
      QVector3D z;
      frame.getAxes(&x, &y, &z);
      QVector3D P = origin + (arm.length() / 2) * y;
      arm.set_translation(P);
      arm.set_rotation(frame); 
      origin += arm.length() * y;
    }
    target_ = origin;
  }

  value_type theta (size_type i) noexcept {
    assert(0 <= i && i <= arm_.size());
    return arm_[i].theta();
  }

  void set_theta (size_type i, value_type theta) noexcept {
    assert(0 <= i && i <= arm_.size());
    arm_[i].set_theta(theta);
  }

  QVector3D target () const noexcept {
    return target_;
  }

  QVector3D origin () const noexcept {
    return origin_;
  }

  void set_origin (QVector3D const & vec) noexcept {
    origin_ = vec;
  }

  QQuaternion origin_frame () const noexcept {
    return origin_frame_;
  }

  void set_origin_frame (QQuaternion const & quat) noexcept {
    origin_frame_ = quat;
  }


  template <class Y>
  struct Loss {

    Loss (joint_arm & arm, Y const & y)
    : arm(arm), y(y) {
    }

    template <class X>
    value_type operator() (X const & x) noexcept {
      QVector3D origin = arm.origin_;
      QQuaternion frame = arm.origin_frame_;
      for (int i = 0; i < arm.arm_.size(); ++i) {
        auto & a = arm.arm_[i];
        switch (a.axis()) {
          case arm::X:
            frame *= QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), (180 / M_PI) * x[i]); break;
          case arm::Y:
            frame *= QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), (180 / M_PI) * x[i]); break;
          case arm::Z:
            frame *= QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), (180 / M_PI) * x[i]); break;
        }
        
        QVector3D x2;
        QVector3D y2;
        QVector3D z2;
        frame.getAxes(&x2, &y2, &z2);
        origin += a.length() * y2;
      }
      origin -= QVector3D(y[0], y[1], y[2]);
      value_type loss = origin[0] * origin[0] + origin[1] * origin[1] + origin[2] * origin[2];
      return loss;
    }

  private:
    joint_arm & arm;
    Y const & y;
  };
 
  template <class Y>
  Loss<Y> loss (Y const & y) noexcept {
    return Loss<Y>(*this, y);
  }

private:
  std::vector<arm> arm_;
  QVector3D target_;
  QVector3D origin_;
  QQuaternion origin_frame_;
};

}}
