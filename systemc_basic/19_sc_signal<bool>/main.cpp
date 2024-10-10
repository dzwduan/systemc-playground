
/*
sc_signal_in_if<bool> 和 sc_signal_in_if<sc_dt::sc_logic>
是提供适用于二值信号的附加成员函数的接口。sc_signal 实现了这些函数：
1. posedge_event()
返回对事件的引用，该事件在通道值发生变化且通道的新值为真或“1”时通知。
2. negedge_event()
返回对事件的引用，该事件在通道值发生变化且通道的新值为假或“0”时通知。
3.
当且仅当通道值在紧接的增量周期的更新阶段和当前模拟时间发生变化，并且通道的新值为真或“1”时，posedge()
返回真。
4.
当且仅当通道值在紧接的上一个增量周期的更新阶段和当前模拟时间发生变化，并且通道的新值为
false 或“0”时，negedge() 才返回 true。 运行
*/

#include <systemc>

using namespace sc_core;

SC_MODULE(SIGNAL_BOOL) {

  sc_signal<bool> b;
  SC_CTOR(SIGNAL_BOOL) {
    SC_THREAD(writer);

    SC_THREAD(consumer);
    sensitive << b;
    dont_initialize();

    SC_THREAD(consumer_pos);
    sensitive << b.posedge_event();
    dont_initialize();

    SC_THREAD(consumer_neg);
    sensitive << b.negedge_event();
    dont_initialize();
  }

  void writer() {
    bool v = true;
    while (true) {
      b.write(v);
      v = !v;
      wait(1, SC_SEC);
    }
  }

  void consumer() {
    while (true) {
      if (b.posedge()) {
        std::cout << sc_time_stamp() << ": consumer receives posedge, b = " << b
                  << std::endl;
      } else {
        std::cout << sc_time_stamp() << ": consumer receives negedge, b = " << b
                  << std::endl;
      }

      wait();
    }
  }

  void consumer_pos() {
    while (true) {
      std::cout << sc_time_stamp()
                << ": consumer_pos receives posedge, b = " << b << std::endl;
      wait();
    }
  }

  void consumer_neg() {
    while (true) {
      std::cout << sc_time_stamp()
                << ": consumer_neg receives negedge, b = " << b << std::endl;
      wait();
    }
  }
};

int sc_main(int argc, char *argv[]) {
  SIGNAL_BOOL sb("sb");
  sc_start(4, SC_SEC);
  return 0;
}
