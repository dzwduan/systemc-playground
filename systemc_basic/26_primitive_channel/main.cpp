/*
sc_prim_channel:
1. 是所有原始通道的基类
2. 为原始通道提供对调度程序更新阶段的唯一访问权限。
3. 不包含层次结构、端口或模拟过程。
4. 与分层通道一样，原始通道可以提供可使用接口方法调用范例调用的公共成员函数。
提供以下成员函数：
  a) request_update()：调度程序将通道的更新请求排队
  b) async_request_update()：
    1) 调度程序以线程安全的方式将通道的更新请求排队。可以从 systemC 内核以外的操作系统线程可靠地调用。
    2) 不建议从在 systemC 内核上下文中执行的函数调用
  c) update()：
    1) 由调度程序在更新阶段回调，以响应对 request_update 或 async_request_update 的调用。
    2) 应用程序可以覆盖此成员函数。 sc_prim_channel 中此函数的定义本身不执行任何操作。
    3) 通常仅读取和修改当前对象的数据成员并创建增量通知。
    4) 不得：
      a) 调用 sc_prim_channel 类的任何成员函数，但成员函数 update 本身（如果在当前对象的基类中被重写）除外
      b) 调用不带参数的 sc_event 类的成员函数 notify() 来创建即时通知
      c) 调用 sc_process_handle 类的任何成员函数进行进程控制（例如暂停或终止）
      d) 更改除当前对象的数据成员之外的任何存储的状态。
      e) 读取除当前对象以外的任何原始通道实例的状态。
      f) 调用其他通道实例的接口方法。特别是，成员函数 update 不应写入任何信号。
  d) next_trigger()
  e) wait()

通道应实现一个或多个接口，因此需要从接口类（基类类型 sc_interface）继承。接口为通道提供所需的方法。
*/

// Learn with Examples, 2020, MIT license
#include "sysc/communication/sc_interface.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_module_name.h"
#include <systemc>
using namespace sc_core;

//============================= Interface =========================//

// interface for interrupt generator
class GEN_IF : public sc_interface {
  public: 
    virtual void notify() = 0;
};

// interface for interrupt receiver
class RECV_IF : public sc_interface {
public:
  virtual const sc_event& default_event()  const  = 0;
};




//============================== port =============================//

// interrupt class 
class INTERRUPT : public sc_prim_channel, public GEN_IF, public RECV_IF {
public:
  INTERRUPT(sc_module_name name) : sc_prim_channel(name) {}

  // implement gen-if
  void notify() { e.notify();} 

  // implement recv-if
  const sc_event& default_event() const {
    return e;
  }

private:
  sc_event e;  //private event for synchronization
};


// interrupt generator 
SC_MODULE(GEN) {
// port to generate interrupt
  sc_port<GEN_IF> p;

  SC_CTOR(GEN) {
    SC_THREAD(gen_interrupt);
  }

  void gen_interrupt() {
    while (true) {
      p->notify();
      wait(1, SC_SEC);
    }
  }
};

SC_MODULE(RECV) {

  sc_port<RECV_IF> p;

  SC_CTOR(RECV) {
    SC_THREAD(recv_interrupt);
    sensitive << p;
    dont_initialize();
  }

  void recv_interrupt() {
    while (true) {
       std::cout << sc_time_stamp() << ": interrupt received" << std::endl;
       wait();
    }
  }
};

int sc_main(int argc, char *argv[]) {
  GEN g("g");
  RECV r("r");
  INTERRUPT i("i");
  g.p(i);
  r.p(i);

  sc_start(2, SC_SEC);
  return 0;
}