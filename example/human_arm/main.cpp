
#include <iostream>
#include <QGuiApplication>

#include <cppik/functional/linear_sampler.hpp>
#include <cppik/solver/gradient_method.hpp>
#include <cppik/loss/range_loss.hpp>
#include <cppik/math.hpp>
#include <cppik/qt/create_view_and_root.hpp>
#include <cppik/qt/joint_arm.hpp>
#include <cppik/qt/object_manager.hpp>
#include <cppik/qt/sphere.hpp>
#include <cppik/external/cmdline/cmdline.h>

#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <random>
#include <string>

int main(int argc, char **argv) {
  using vec3f = std::array<float, 3>;
  using vec4f = std::array<float, 4>;
  using vec5f = std::array<float, 5>;
  cmdline::parser p;

  p.add<float>("alpha", '\0', "alpha", false, 0.01);
  p.add<float>("beta", '\0', "beta", false, 0.0001);
  p.add<float>("gamma", '\0', "gamma", false, 10.0);
  p.add<float>("delta", '\0', "delta", false, 1e-6);
  p.add<std::size_t>("step", '\0', "step", false, 256);
  p.add<std::size_t>("interval", '\0', "interval", false, 30);
  p.add<float>("arm-radius-1", '\0', "arm-radius-1", false, 0.5);
  p.add<float>("arm-radius-2", '\0', "arm-radius-2", false, 0.5);
  p.add<float>("arm-radius-3", '\0', "arm-radius-3", false, 0.5);
  p.add<float>("arm-length-1", '\0', "arm-length-1", false, 1.0);
  p.add<float>("arm-length-2", '\0', "arm-length-2", false, 2.0);
  p.add<float>("arm-length-3", '\0', "arm-length-3", false, 2.0);
  p.add<float>("sphere-radius", '\0', "sphere-raidus", false, 0.5);

  p.parse_check(argc, argv);

  float alpha = p.get<float>("alpha");
  float beta = p.get<float>("beta");
  float gamma = p.get<float>("gamma");
  float delta = p.get<float>("delta");
  int step = p.get<std::size_t>("step");
  int interval = p.get<std::size_t>("interval");

  float arm_radius1 = p.get<float>("arm-radius-1");
  float arm_radius2 = p.get<float>("arm-radius-2");
  float arm_radius3 = p.get<float>("arm-radius-3");
  float arm_length1 = p.get<float>("arm-length-1");
  float arm_length2 = p.get<float>("arm-length-2");
  float arm_length3 = p.get<float>("arm-length-3");
  float sphere_radius = p.get<float>("sphere-radius");

  QApplication app(argc, argv);

  Qt3DExtras::Qt3DWindow * view;
  Qt3DCore::QEntity * root;
  std::tie(view, root) = cppik::qt::create_view_and_root();

  cppik::qt::object_manager manager(root, 100);

  cppik::qt::sphere sphere(root, sphere_radius);
  manager.push_back(sphere);

  cppik::qt::joint_arm arm;
  manager.push_back(arm);
  arm.set_origin_frame(QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
  arm.add_arm(root, arm_radius1, arm_length1, cppik::qt::arm::Y, 0x0000ff);
  arm.add_arm(root, arm_radius2, arm_length2, cppik::qt::arm::Z, 0x0000ff);
  arm.add_arm(root, arm_radius3, arm_length3, cppik::qt::arm::X, 0x0000ff);

  cppik::qt::joint_arm arm2;
  manager.push_back(arm2);
  arm2.set_origin_frame(QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
  arm2.add_arm(root, arm_radius1, arm_length1, cppik::qt::arm::Y, 0x00ff00);
  arm2.add_arm(root, arm_radius2, arm_length2, cppik::qt::arm::Z, 0x00ff00);
  arm2.add_arm(root, arm_radius3, arm_length3, cppik::qt::arm::X, 0x00ff00);

  cppik::qt::joint_arm arm3;
  manager.push_back(arm3);
  arm3.set_origin_frame(QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
  arm3.add_arm(root, arm_radius1, arm_length1, cppik::qt::arm::Y, 0xff0000);
  arm3.add_arm(root, arm_radius2, arm_length2, cppik::qt::arm::Z, 0xff0000);
  arm3.add_arm(root, arm_radius3, arm_length3, cppik::qt::arm::X, 0xff0000);

  view->show();

  auto solver = cppik::solver::gradient_method<float>(
    alpha,
    beta,
    gamma,
    delta
  );

  std::thread worker([&] () {
    using namespace cppik;

    std::mt19937 engine;

    std::vector<vec3f> trajectory1(step);
    std::vector<vec3f> trajectory2(step);
    std::vector<vec3f> trajectory3(step);

    cppik::linear_sampler<float> sampler;

    trajectory1[0] = vec3f{
      std::uniform_real_distribution<float>(-M_PI, M_PI)(engine),
      std::uniform_real_distribution<float>(-M_PI, M_PI)(engine),
      std::uniform_real_distribution<float>(-M_PI, M_PI)(engine)
    };

    while (true) {
      float l = arm_length1 + arm_length2;
      vec3f t = {
        std::uniform_real_distribution<float>(-l, l)(engine),
        std::uniform_real_distribution<float>(0, l)(engine),
        std::uniform_real_distribution<float>(-l, l)(engine)
      };

      std::cout << "calculation start:" << std::endl;
      auto tp1 = std::chrono::system_clock::now();

      sphere.set_position(t[0], t[1], t[2]);
      auto loss = [&] (vec3f const & v) {
        return arm.loss(t)(v) +
          range_loss<float>(-M_PI, M_PI)(v[1]) + 
          range_loss<float>(-M_PI, M_PI)(v[2])
        ; 
      };
      solver.solve(trajectory1, loss, [&] (std::size_t i) {
        solver.alpha = alpha * sin(M_PI * i / step);
      });

      sampler(trajectory1, trajectory2);
      std::array<vec3f, 2> tmp{trajectory1.front(), trajectory1.back()};
      sampler(tmp, trajectory3);

      auto tp2 = std::chrono::system_clock::now();
      std::cout << "calculation end: " << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << " milliseconds elapsed." << std::endl;
      std::cout << solver.step << " / " << trajectory1.size() << " step." << std::endl;

      for (int i = 0; i < step; ++i) {
        for (int j = 0; j < trajectory1[i].size(); ++j) {
          arm.set_theta(j, trajectory1[i][j]);
          arm2.set_theta(j, trajectory2[i][j]);
          arm3.set_theta(j, trajectory3[i][j]);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
      }

      trajectory1[0] = trajectory1[step - 1];
    }
  });

  int ret = app.exec();
  return ret;
}
