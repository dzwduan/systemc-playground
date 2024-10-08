#ifndef __B_TRANSPORT_H__
#define __B_TRANSPORT_H__

#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include "systemc"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

/**               target.b_transport
 * initiator ============================> [target register_b_transport]
 */

using namespace sc_core;

class MyInitiator : public sc_module {
public:
  SC_CTOR(MyInitiator) : socket("socket") { SC_THREAD(MainThread); }

  void MainThread() {
    int cycle_cnt = 0;
    auto t_delay = sc_core::SC_ZERO_TIME;
    while (true) {
      cycle_cnt++;
      t_delay = sc_time(cycle_cnt, SC_NS);
      auto t_payload = new tlm::tlm_generic_payload();
      t_payload->set_address(0x10000 + cycle_cnt);
      std::cout << "\033[34m [" << sc_core::sc_time_stamp() << "]"
                << " call b_transport " << std::hex << t_payload->get_address()
                << " delay cycle " << cycle_cnt << " \033[0m" << std::endl;

      socket->b_transport(*t_payload, t_delay);
      wait(1, SC_NS);
    }
  }

  tlm_utils::simple_initiator_socket<MyInitiator> socket;
};

class MyTarget : public sc_module {
public:
  SC_CTOR(MyTarget) : socket("socket") {
    socket.register_b_transport(this, &MyTarget::RecvReqFunc);
  }

  void RecvReqFunc(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
    wait(delay);
    std::cout << "\033[35m [" << sc_core::sc_time_stamp() << "]"
         << " RecvReqFunc " << std::hex << payload.get_address() << " \033[0m"
         << std::endl;
  }

  tlm_utils::simple_target_socket<MyTarget> socket;
};

class MyTop : public sc_module {
public:
  SC_CTOR(MyTop) : m_init("init"), m_target("target") {
    m_init.socket.bind(m_target.socket);
  }

  MyInitiator m_init;
  MyTarget m_target;
};



#endif