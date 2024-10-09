#include "sysc/kernel/sc_simcontext.h"
#include "systemc"

using namespace sc_core;

/**
 * event 用于进程同步，进程示例在event发生时触发或恢复，
 * 给定的event不能有多个pending notification
 */

SC_MODULE(EVENT) {
  sc_event e;
  SC_CTOR(EVENT) {
    SC_THREAD(trigger);
    SC_THREAD(catcher);
  }

  void trigger() {
    while (true) {
      e.notify(1, SC_SEC);
      if (sc_time_stamp() == sc_time(4, SC_SEC))
        e.cancel();

      wait(2, SC_SEC);
    }
  }

  void catcher() {
    while (true) {
      wait(e); // wait for event
      std::cout << "Event cateched at " << sc_time_stamp() << std::endl;
    }
  }
};

int sc_main(int, char **) {
  EVENT event("event");
  sc_start(8, SC_SEC);
  return 0;
}