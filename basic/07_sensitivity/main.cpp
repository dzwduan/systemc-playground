#include <systemc>

/**
 * 两种类型的敏感度：
    1. 静态敏感度在阐述期间是固定的，由模块中每个流程的敏感度列表支持。
    2. 动态敏感度可能在流程本身的控制下随时间变化，由线程的 wait() 或方法的next_trigger() 支持，与具体的event挂钩
 */

using namespace sc_core;

SC_MODULE(SENSITIVITY) {
  sc_event e1, e2;
  SC_CTOR(SENSITIVITY) {
    SC_THREAD(trigger_1); // register processes
    SC_THREAD(trigger_2);
    SC_THREAD(catch_1or2_dyn);
    SC_THREAD(catch_1or2_static);

    sensitive << e1 << e2;
  }

  void trigger_1() {
    wait(SC_ZERO_TIME); // delay trigger by a delta cycle, make sure catcher is
                        // ready
    while (true) {
      e1.notify();     // trigger e1
      wait(2, SC_SEC); // dynamic sensitivity, re-trigger after 2 s
    }
  }

  void trigger_2() {
    wait(SC_ZERO_TIME);
    while (true) {
      e2.notify();     // trigger e2
      wait(3, SC_SEC); // dynamic sensitivity, re-trigger after 3 s
    }
  }

  void catch_1or2_dyn() {
    while (true) {
      wait(e1 | e2); // dynamic sensitivity, 与event相关
      std::cout << "Dynamic sensitivty: e1 or e2 @ " << sc_time_stamp()
                << std::endl;
    }
  }

  void catch_1or2_static() {
    while (true) {
      wait(); // static sensitivity 
      std::cout << "Static sensitivity: e1 or e2 @ " << sc_time_stamp()
                << std::endl;
    }
  }
};

int sc_main(int, char *[]) {
  SENSITIVITY sensitivity("sensitivity");
  sc_start(7, SC_SEC);
  return 0;
}