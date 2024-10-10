#include "sysc/communication/sc_event_queue.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext.h"
#include <systemc>

/**
 event queue:
 1. 有成员函数notify
 2. 分层channel，可以有多个Pending notifications
 3. 只能在elaboration期间构造
 4. 不支持 immediate notification
 */

using namespace sc_core;

SC_MODULE(QUEUE) {

  sc_event e;
  sc_event_queue eq;

  SC_CTOR(QUEUE) {
    SC_THREAD(trigger);
    SC_THREAD(catch_e);
    sensitive << e;

    dont_initialize(); // 仅对 cache_e生效

    SC_THREAD(catch_eq);
    sensitive << eq;
    dont_initialize(); // 仅对 cache_eq生效
  }

  void trigger() {
    while (true) {
      e.notify(2, SC_SEC);
      e.notify(1, SC_SEC);  // event只能处理最后一个

      eq.notify(2, SC_SEC);
      eq.notify(1, SC_SEC); // event queue能同时处理多个
      wait(10, SC_SEC);
    }
  }

  void catch_e() {
    while (true) {
      std::cout << sc_time_stamp() << " : catch e" << std::endl;
      wait(); // static sensitive
    }
  }

  void catch_eq() {
    while (true) {
      std::cout << sc_time_stamp() << " : catch eq" << std::endl;
      wait(); // static sensitive
    }
  }
};

int sc_main(int argc, char *argv[]) {
  QUEUE process("queue");
  sc_start(20, SC_SEC);
  return 0;
}