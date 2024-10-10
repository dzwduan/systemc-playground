#include "sysc/communication/sc_event_queue.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext.h"
#include <systemc>

/**
1. 多个事件队列可以通过“OR”运算来形成进程的静态敏感度。在静态敏感度中不能使用“AND”运算。
2. 事件队列不能用作 wait() 的输入，因此不能在动态敏感度中使用。
 */

using namespace sc_core;

SC_MODULE(QUEUE_COMBINED) {

  sc_event_queue eq1, eq2;

  SC_CTOR(QUEUE_COMBINED) {
    SC_THREAD(trigger);
    SC_THREAD(catcher);
    sensitive << eq1 << eq2;

    dont_initialize();
  }

  void trigger() {
      eq1.notify(1, SC_SEC);
      eq1.notify(2, SC_SEC);  

      eq2.notify(2, SC_SEC);
      eq2.notify(3, SC_SEC); 
  }

  void catcher() {
    while (true) {
      std::cout << sc_time_stamp() << " : catcher trigger" << std::endl;
      wait();
    }
  }


};

int sc_main(int argc, char *argv[]) {
  QUEUE_COMBINED process("combined");
  sc_start(10, SC_SEC);
  return 0;
}