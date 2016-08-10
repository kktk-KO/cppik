#pragma once

#include <cppik/core.hpp>
#include <cppik/math.hpp>
#include <cppik/qt/cylinder.hpp>
#include <cppik/qt/object.hpp>

#include <memory>

namespace cppik { namespace qt {

/**
 *  @details
 *  Except update method, writing and reading methods are thread-safe.
 */
struct multi_joint_arm_2d : object {

  using value_type = float;
  using size_type = default_size_type;

  multi_joint_arm_2d (Qt3DCore::QEntity * parent) noexcept
  : parent_(parent) {
  }

/**
 *  @details
 *  This method SHOULD be called from QT thread.
 */
  void update () {
    value_type length = 0;
    value_type theta = 0;
    std::array<value_type, 3> p = position();
    std::array<value_type, 2> P{p[0], p[1]};

    size_type N = arm_.size();

    for (size_type i = 0; i < N; ++i) {
      if (!arm_[i]) { return; }
      length = arm_[i]->length();
      theta += theta_[i];

      auto sc = sincos(theta);
      arm_[i]->set_position(P[0], P[1], 0);
      arm_[i]->set_theta(theta);
      arm_[i]->update();
      P[0] += length * sc.second;
      P[1] += length * sc.first;
    }
  }

  void add_arm (value_type radius, value_type length) {
    arm_.emplace_back(new cylinder(parent_, radius, length));
    arm_.back()->set_phi(M_PI / 2);
    theta_.push_back(0);
  }

  value_type theta (size_type i) const noexcept {
    return theta_[i];
  }

  void set_theta (size_type i, value_type v) {
    theta_[i] = v;
  }

  value_type radius (size_type i) const noexcept {
    return arm_[i]->radius();
  }

  void set_radius (size_type i, value_type v) {
    arm_[i]->set_radius(v);
  }

  value_type length (size_type i) const noexcept {
    return arm_[i]->length();
  }

  void set_length (size_type i, value_type v) {
    arm_[i]->set_length(v);
  }

  std::array<value_type, 3> const & position () const noexcept {
    return arm_.front()->position();
  } 

  void set_position (value_type x, value_type y, value_type z) {
    arm_.front()->set_position(x, y, z);
  }

  size_type size () const noexcept {
    return arm_.size();
  }

  template <class Y>
  struct Loss {

    Loss (multi_joint_arm_2d & arm, Y const & y)
    : arm(arm), y(y) {
    }

    template <class X>
    value_type operator() (X const & x) {
      std::array<value_type, 2> P{0, 0};
      value_type theta = 0;
 
      size_type N = cppik::size(x);

      for (size_type i = 0; i < N; ++i) {
        value_type length = arm.length(i); 
        theta += get(x, i);
        auto sc = sincos(theta);
        P[0] += length * sc.second;
        P[1] += length * sc.first;
      }
      P[0] -= y[0];
      P[1] -= y[1];
      value_type loss = P[0] * P[0] + P[1] * P[1];
      return loss;
    }
  private:
    multi_joint_arm_2d & arm;
    Y const & y;
  };

  template <class Y>
  Loss<Y> loss (Y const & y) noexcept {
    return Loss<Y>(*this, y);
  }

private:
  Qt3DCore::QEntity * parent_; 
  std::vector<std::unique_ptr<cylinder>> arm_;
  std::vector<value_type> theta_;
};

}}
