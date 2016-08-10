#pragma once

#include <cppik/type.hpp>
#include <cppik/math/norm.hpp>
#include <cppik/math/sqrt.hpp>
#include <cppik/math/sigmoid.hpp>
#include <cppik/vector/static_vector.hpp>

#include <random>
#include <utility>
#include <cassert>

namespace cppik { namespace solver {

/**
 *  @details
 */
template <class ValueType, size_type N, class Loss>
struct montecarlo_method {

  using value_type = ValueType;
  using loss_type = Loss;

  loss_type loss;
  value_type beta;
  int max_step;
  vector::static_vector<value_type, N> range;

  value_type loss_last;
  std::mt19937 engine;

  template <class L>
  montecarlo_method (L && loss, value_type beta, int max_step) noexcept
  : loss(std::forward<L>(loss)),
    beta(beta),
    max_step(max_step),
    loss_last(0) {
    for (size_type i = 0; i < N; ++i) {
      range[i] = 0.1;
    }
  }

  template <class ParVecVec, class PosVec, class Handler> 
  void solve (ParVecVec & par, PosVec const & target, Handler && handler) noexcept {
    size_type size = par.size();
    for (int i = 1; i < size; ++i) {
      solve_once(par[i - 1], target, par[i]);
      handler(i);
    }
  } 

  template <class ParVec, class PosVec> 
  void solve_once (ParVec const & prev, PosVec const & target, ParVec & next) noexcept {
    assert(prev.size() == N);
    assert(next.size() == N);

    for (size_type j = 0; j < max_step; ++j) {
      for (size_type i = 0; i < N; ++i) {
        next[i] = std::uniform_real_distribution<value_type>(-range[i], range[i])(engine) + prev[i];
      }
  
      value_type l = loss(next, target);
      if (l < loss_last) {
        loss_last = l;
        break;
      }

      value_type p = sigmoid(beta, loss_last - l);
      value_type P = std::uniform_real_distribution<value_type>(0, 1)(engine);
      std::cout << p << std::endl;
      if (P < p) {
        loss_last = l;
        break;
      }
    }
  } 

};

template <class ValueType, size_type N, class Loss>
auto make_montecarlo_method ( 
  Loss && loss,
  ValueType beta,
  size_type max_step
) -> montecarlo_method<ValueType, N, typename std::remove_reference<Loss>::type> {
  return montecarlo_method<ValueType, N, typename std::remove_reference<Loss>::type>(
    std::forward<Loss>(loss), beta, max_step
  ); 
}

}}
