#pragma once

#include <cppik/math/norm.hpp>
#include <cppik/math/sqrt.hpp>
#include <cppik/core.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace cppik { namespace solver {

/**
 *  @details
 *  gradient_method tries to minimize the loss defined as follows:
 *  \f$\text{loss} = L(\bm{x}_i)\f$
 *
 *  When people refer to "gradient method", it includs the steepest gradient method, conjugate gradient method and so on.
 *  This class implements the steepest gradient method.
 *  The trajectory is predicted using following scheme.
 *  \f$ \bm{x}_i = \bm{x}_{i - 1} - \alpha \frac{\partial L}{\partial \bm{x}}|_{\bm{x} = \bm{x}_{i - 1}} \f$
 *  where \f$ \alpha \f$ is the accleration coefficent.
 *
 *  In current implementation, the gradient vector is estimated using the first order central difference.
 *  \f$ \beta \f$ is the step used in numerical diference.
 *
 *  To suppress divergent behavior, there is the upper limit of the norm of the gradient vector.
 *  If the norm of the gradient vector is greater than the limit, the gradient vector is normarized.
 *  \f$ \gamma \f$ is the upper limit of the norm of the gradient vector.
 */

template <class ValueType = double, class SizeType = default_size_type>
struct gradient_method {

  using value_type = ValueType;
  using size_type = SizeType;

  value_type alpha;
  value_type beta;
  value_type gamma;
  value_type delta;
  size_type step;

  gradient_method (value_type alpha, value_type beta, value_type gamma, value_type delta) noexcept
  : alpha(alpha),
    beta(beta),
    gamma(gamma),
    delta(delta) {
  }

  void init () {
    step = 0;
  }

  template <class Xs, class F> 
  bool solve (Xs & xs, F && f) noexcept {
    init();
    for (size_type i = 1; i < size(xs); ++i) {
      if (!solve_once(get(xs, i - 1), get(xs, i), f)) { return true; }
    }
    return false;
  } 

  template <class Xs, class F, class Handler> 
  bool solve (Xs & xs, F && f, Handler && handler) noexcept {
    init();
    for (size_type i = 1; i < size(xs); ++i) {
      handler(i);
      if (!solve_once(get(xs, i - 1), get(xs, i), f)) { return true; }
    }
    return false;
  } 

  template <class X, class F> 
  bool solve_once (X const & x_prev, X & x_next, F && f) noexcept {
    assert(size(x_prev) > 0);
    assert(size(x_next) > 0);
    assert(size(x_prev) == size(x_next));

    step += 1;
    diff(x_prev, x_next, f);
    auto n = sqrt(norm(x_next));
    if (n < delta) {
      x_next = x_prev;
      return false;
    }
    n = alpha * ((n < gamma) ? 1 : gamma);

    using val = cppik::value_type<X>; 
    map(x_next, x_prev, [&] (val & xn, val const & xp) {
      xn = xp - n * xn;
    });
    return true;
  } 

  template <class X, class F> 
  void diff (X const & x, X & dx, F && f) noexcept {
    // TODO: dispatch based on ParVec::is_static_sized.

    assert(size(x) > 0);
    assert(size(dx) > 0);
    assert(size(x) == size(dx));

    X tmp1 = x;
    X tmp2 = x;
    get(tmp1, 0) -= beta;
    get(tmp2, 0) += beta;
    get(dx, 0) = (f(tmp2) - f(tmp1)) / (2 * beta);

    for (cppik::size_type<X> i = 1; i < size(x); ++i) {
      get(tmp1, i - 1) += beta;
      get(tmp2, i - 1) -= beta;
      get(tmp1, i) -= beta;
      get(tmp2, i) += beta;
      get(dx, i) = (f(tmp2) - f(tmp1)) / (2 * beta);
    }
  }

};

}}
