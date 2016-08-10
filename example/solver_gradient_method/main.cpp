#include <QGuiApplication>

#include <cppik/solver/gradient_method.hpp>
#include <cppik/math.hpp>
#include <cppik/qt/create_view_and_root.hpp>
#include <cppik/qt/multi_joint_arm_2d.hpp>
#include <cppik/qt/sphere.hpp>
#include <cppik/qt/object_manager.hpp>
#include <cppik/external/cmdline/cmdline.h>

#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <random>
#include <string>

int main(int argc, char **argv)
{
  cmdline::parser p;

  p.add<double>("alpha", '\0', "alpha", true);
  p.add<double>("beta", '\0', "beta", true);
  p.add<double>("gamma", '\0', "gamma", false, 10.0);
  p.add<double>("delta", '\0', "delta", false, 1e-6);
  p.add<std::size_t>("step", '\0', "step", false, 100);
  p.add<double>("arm-radius-1", '\0', "arm-radius-1", false, 0.5);
  p.add<double>("arm-radius-2", '\0', "arm-radius-2", false, 0.5);
  p.add<double>("arm-length-1", '\0', "arm-length-1", false, 3.0);
  p.add<double>("arm-length-2", '\0', "arm-length-2", false, 3.0);
  p.add<double>("sphere-radius", '\0', "sphere-raidus", false, 0.5);

  p.parse_check(argc, argv);

  double alpha = p.get<double>("alpha");
  double beta = p.get<double>("beta");
  double gamma = p.get<double>("gamma");
  double delta = p.get<double>("delta");
  int step = p.get<std::size_t>("step");

  double arm_radius1 = p.get<double>("arm-radius-1");
  double arm_radius2 = p.get<double>("arm-radius-2");
  double arm_length1 = p.get<double>("arm-length-1");
  double arm_length2 = p.get<double>("arm-length-2");
  double sphere_radius = p.get<double>("sphere-radius");

  QApplication app(argc, argv);

  Qt3DExtras::Qt3DWindow * view;
  Qt3DCore::QEntity * root;
  std::tie(view, root) = cppik::qt::create_view_and_root();

  cppik::qt::object_manager manager(root, 100);

  cppik::qt::multi_joint_arm_2d arm(root);
  manager.push_back(arm);
  arm.add_arm(arm_radius1, arm_length1);
  arm.add_arm(arm_radius2, arm_length2);

  cppik::qt::sphere sphere(root, sphere_radius);
  manager.push_back(sphere);

  view->show();

  auto solver = cppik::solver::gradient_method<double>(
    alpha,
    beta,
    gamma,
    delta
  );

  std::thread worker([&] () {
    using namespace cppik;

    std::mt19937 engine;

    std::vector<std::array<double, 2>> trajectory1(step);

    trajectory1[0] = std::array<double, 2>{
      std::uniform_real_distribution<double>(0, M_PI)(engine),
      std::uniform_real_distribution<double>(-M_PI, M_PI)(engine)
    };

    while (true) {
      std::array<double, 2> t = {
        std::uniform_real_distribution<double>(- arm_length1 - arm_length2, arm_length1 + arm_length2)(engine),
        std::uniform_real_distribution<double>(- arm_length1 - arm_length2, arm_length1 + arm_length2)(engine)
      };

      std::cout << "calculation start:" << std::endl;
      auto tp1 = std::chrono::system_clock::now();

      sphere.set_position(t[0], t[1], 0);
      solver.solve(trajectory1, arm.loss(t), [&] (std::size_t i) {
        solver.alpha = alpha * sin(M_PI * i / step);
      });

      auto tp2 = std::chrono::system_clock::now();
      std::cout << "calculation end: " << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << " milliseconds elapsed." << std::endl;
      std::cout << solver.step << " / " << trajectory1.size() << " step." << std::endl;

      for (int i = 0; i < step; ++i) {
        arm.set_theta(0, trajectory1[i][0]);
        arm.set_theta(1, trajectory1[i][1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
      }

      trajectory1[0] = trajectory1[step - 1];
    }
  });

  int ret = app.exec();
  return ret;
}
