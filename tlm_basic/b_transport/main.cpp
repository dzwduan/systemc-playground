#include "sysc/kernel/sc_time.h"
#include <systemc>

#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;

class MyInitiator : public sc_module {
public:
    SC_CTOR(MyInitiator) : socket("socket") {
        SC_THREAD(thread);
    }

    void thread() {
        int cycle_cnt = 0;
        sc_time delay = SC_ZERO_TIME;
        while (true) {
            cycle_cnt++;
            delay = sc_time(cycle_cnt, SC_NS);
            tlm::tlm_generic_payload *payload = new tlm::tlm_generic_payload();
            payload->set_address(0x10000 + cycle_cnt);
             std::cout << "\033[34m [" << sc_core::sc_time_stamp() << "]"
                << " call b_transport " << std::hex << payload->get_address()
                << " delay cycle " << cycle_cnt
                << " \033[0m" << std::endl;
            socket->b_transport(*payload, delay);
            wait(1, SC_NS);
        }
    }

    tlm_utils::simple_initiator_socket<MyInitiator> socket;
};


class MyTarget : public sc_module {
public:
    SC_CTOR(MyTarget): socket("socket") {
        socket.register_b_transport(this, &MyTarget::RecvReq);
    };

    void RecvReq(tlm::tlm_generic_payload& payload, sc_core::sc_time &delay) {
        wait(delay);
         std::cout << "\033[35m [" << sc_core::sc_time_stamp() << "]"
              << " RecvReqFunc " << std::hex << payload.get_address() << " \033[0m" << std::endl;
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


int sc_main(int argc, char * argv[]) {
    MyTop top("top");
    sc_start(20, sc_core::SC_NS);
    return 0;
}