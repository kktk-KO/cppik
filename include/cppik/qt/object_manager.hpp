#pragma once

#include <cppik/qt/object.hpp>

#include <vector>
#include <memory>
#include <mutex>

namespace cppik { namespace qt {

struct object_manager {

/**
 *  @details
 *  interval is in milliseconds.
 */
  object_manager (QObject * parent, int interval) {
    tmr_ = new timer(parent, *this, interval);
  }

  void update () {
    std::lock_guard<std::mutex> lk(mtx_);
    for (auto & obj : objects_) {
      obj->update();
    }
  }

  void push_back (object & obj) {
    std::lock_guard<std::mutex> lk(mtx_);
    objects_.push_back(&obj);
  }

private:
  struct timer : QObject {
    object_manager & man;
    int interval;
    int timerId; 
    // interval is in milliseconds.
    timer (QObject * parent, object_manager & man, int interval)
    : QObject(parent), man(man), interval(interval) {
      timerId = startTimer(interval);
    }

    void timerEvent (QTimerEvent * e) {
      if (e->timerId() == timerId) { man.update(); }
    }
  };

  timer * tmr_;
  std::vector<object *> objects_; 
  std::mutex mtx_;
};

}}
