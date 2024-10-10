/**
sc_buffer 是从 sc_signal 类派生的预定义原始通道。
它与 sc_signal
类的不同之处在于，每当写入缓冲区时都会通知值更改事件，而不是仅在信号值更改时通知。
例如，
如果“信号”的当前值 == 1：向其写入 1 不会触发值更新事件。
如果“缓冲区”的当前值 == 1：向其写入 1 将触发值更新事件。
*/

#include "sysc/kernel/sc_module.h"
#include <systemc>

using namespace sc_core;

SC_MODULE(BUFFER) {

  sc_signal<int> s;
  sc_buffer<int> b;

  SC_CTOR(BUFFER) {
    SC_THREAD(writer);

    SC_THREAD(consumer1);
    sensitive << s;
    dont_initialize();

    SC_THREAD(consumer2);
    sensitive << b;
    dont_initialize();
  }

  void writer() {
    int val = 1;
    while (true) {
      for (int i = 0; i < 2; i++) {
        s.write(val);
        b.write(val);

        wait(1, SC_SEC);
      }
      val++;
    }
  }

  void consumer1() {
    while (true) {
      std::cout << sc_time_stamp() << ": consumer1 receives " << s.read()
                << std::endl;
      wait();
    }
  }

  void consumer2() {
    while (true) {
      std::cout << sc_time_stamp() << ": consumer2 receives " << b.read()
                << std::endl;
      wait();
    }
  }
};

int sc_main(int argc, char *argv[]) {

  BUFFER b("b");
  sc_start(4, SC_SEC);
  return 0;
}