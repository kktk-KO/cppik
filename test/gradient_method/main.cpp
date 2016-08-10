#include <cppik/solver/gradient_method.hpp>
#include <cppik/core.hpp>
#include <cppik/math.hpp>

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

int main () {
  std::vector<double> xs(200);
  cppik::get(xs, 0) = 10.0;

  cppik::solver::gradient_method<double> solver(0.1, 0.001, 10, 1e-5);

  solver.solve(xs, [] (double v) { return v * v; });
  assert(cppik::abs(xs[solver.step]) < 1e-5);
}
