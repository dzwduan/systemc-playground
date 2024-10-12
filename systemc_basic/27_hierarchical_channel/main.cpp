/*
分层通道：
1. 应继承 sc_channel 基类，该基类与 sc_module 相同。因此，分层通道是 systemC 模块。
2. 应从接口继承，以使其连接到端口。
与常规 systemC 模块一样，分层通道可能具有模拟过程、端口等。

此示例显示了实现 sc_signal_inout_if<int> 的自定义分层通道。根据 sc_signal_inout_if 的定义，我们必须实现以下函数：
1. void write(const int&)
2. const int& read() const
3. const sc_event& value_changed_event() const
4. const sc_event& default_event() const
5. const int& get_data_ref() const
6. bool event() const
*/

// Learn with Examples, 2020, MIT license
#include "sysc/communication/sc_interface.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_module_name.h"
#include "sysc/kernel/sc_process.h"
#include <systemc>
using namespace sc_core;

class SIGNAL : public sc_channel, public sc_signal_inout_if<int> {
  public:

    SIGNAL(sc_module_name name = sc_gen_unique_name("SIG")) : sc_channel(name) {}

    void write(const int& v) {
      if (v != m_val) {
        m_val = v;
        e.notify();
      }
    }

    const int& read() const {
      return m_val;
    }

    const sc_event& value_changed_event() const {
      return e;
    }

    const sc_event& default_event() const {
      return value_changed_event();
    }

    const int& get_data_ref() const {
      return m_val;
    }

    bool event() const {
      return true;
    }
    
    private:
      int m_val = 0;
      sc_event e;
};


SC_MODULE(TEST) {
  SIGNAL s;

  SC_CTOR(TEST) {
    SC_THREAD(writer);
    SC_THREAD(reader);

    sensitive << s;
    dont_initialize();
  }

  void writer() {
    int v = 1;
    while (true) {
      s.write(v++);
      wait(1, SC_SEC);
    }
  }

  void reader() {
    while (true) {
      std::cout << sc_time_stamp() << ": val = " << s.read() << std::endl;
      wait();
    }
  }
};


int sc_main(int argc, char *argv[]) {
  TEST test("t");
  sc_start(2, SC_SEC);
  return 0;
}
