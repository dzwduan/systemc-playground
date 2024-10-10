
/*
1. sc_event& default_event()、sc_event& value_changed_event()：返回对值改变事件的引用。
2. bool event()：当且仅当信号值在紧接的先前增量周期的更新阶段和当前模拟时间发生变化时，才返回true。
*/

#include "sysc/kernel/sc_module.h"
#include <systemc>

using namespace sc_core;

SC_MODULE(SIGNAL_EVENT) {

  sc_signal<int> s1, s2;

  SC_CTOR(SIGNAL_EVENT) {
    SC_THREAD(producer1);
    SC_THREAD(producer2);
    SC_THREAD(consumer);

    sensitive << s1 << s2;

    dont_initialize(); // just for consumer
  }

  // write to s1
  void producer1() {
    int v = 1;
    while (true) {
      s1.write(v++);
      wait(2, SC_SEC);
    }
  }

  // write to s2
  void producer2() {
    int v = 1;
    while (true) {
      s2 = v++;
      wait(3, SC_SEC);
    }
  }

  void consumer() {
    while (true) {
      if (s1.event() == true && s2.event() == true) {
        std::cout << sc_time_stamp() << ": s1 & s2 triggered" << std::endl;
      } else if (s1.event() == true) {
        std::cout << sc_time_stamp() << ": s1 triggered" << std::endl;
      } else {
        std::cout << sc_time_stamp() << ": s2 triggered" << std::endl;
      }

      wait();
    }
  }
};

int sc_main(int argc, char *argv[]) {
  SIGNAL_EVENT se("se");
  sc_start(7, SC_SEC);
  return 0;
}
