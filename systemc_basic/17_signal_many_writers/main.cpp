
/*
sc_signal 的类定义：
template <class T, sc_writer_policy WRITER_POLICY = SC_ONE_WRITER> class
sc_signal: public sc_signal_inout_if<T>, public sc_prim_channel {}
1. 如果 WRITER_POLICY == SC_ONE_WRITER，则在模拟期间的任何时间从多个流程实例写入给定信号实例都是错误的。
2. 如果 WRITER_POLICY == SC_MANY_WRITERS:
a) 在任何给定评估阶段从多个流程实例写入给定信号实例都是错误的，
b) 但不同的流程实例可以在不同的增量周期内写入给定信号实例。

因此，默认情况下 sc_signal 只有一个写入器；当声明为 MANY_WRITERS
时，写入器可以在不同时间写入信号通道。 至于消费者，sc_signal
可能有多个消费者。它们可以同时或不同时间从信号通道读取。
*/

#include "sysc/communication/sc_writer_policy.h"
#include <systemc>

using namespace sc_core;

SC_MODULE(MULTI) {

  sc_signal<int> s1;
  sc_signal<int, sc_core::SC_MANY_WRITERS> s2;
  SC_CTOR(MULTI) {

    SC_THREAD(writer1);  // write to s1
    SC_THREAD(writer2);  // write to s1 and s2
    SC_THREAD(consumer1);

    sensitive << s1;

    dont_initialize(); // just for consumer1
    SC_THREAD(consumer2);
    sensitive << s1 << s2;
    dont_initialize(); // just for consumer2
  }

  void writer1() {
    int v = 1;
    while (true) {
      s1.write(v);
      s2.write(v);
      std::cout << sc_time_stamp() << ": writer1 writes " << v++ << std::endl;
      wait(1, SC_SEC);
    }
  }

  void writer2() {
    int v = -1;
    while (true) {
      // s1.write(v); /* cannot, otherwise runtime error: (E115) sc_signal<T>
      // cannot have more than one driver*/

      // needed to offset the write time. Otherwise runtime error: conflicting
      // write in delta
      wait(SC_ZERO_TIME);

      s2.write(v);
      std::cout << sc_time_stamp() << ": writer2 writes " << v-- << std::endl;
      wait(1, SC_SEC);
    }
  }

  void consumer1() {
    while (true) {
      std::cout << sc_time_stamp() << ": consumer1 reads s1=" << s1.read()
                << "; s2=" << s2.read() << std::endl;
      wait(); // wait for s1
    }
  }

  void consumer2() {
    while (true) {
      std::cout << sc_time_stamp() << ": consumer2 reads s1=" << s1.read() << "; s2=" << s2.read() << std::endl; // read s1 and s2
      wait(); // wait for s1 or s2
    }
  }
};

int sc_main(int argc, char *argv[]) {
  MULTI consumer("c");
  sc_start(2, SC_SEC);
  return 0;
}
