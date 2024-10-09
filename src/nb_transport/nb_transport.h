#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include "systemc.h"
#include "tlm_core/tlm_2/tlm_2_interfaces/tlm_fw_bw_ifs.h"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_phase.h"
#include "tlm_utils/peq_with_get.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"


using namespace sc_core;


class MyInitiator_nb : public sc_module {
    public:
    SC_CTOR(MyInitiator_nb) : socket("socket"), m_test_peq("test_peq") {

        socket.register_nb_transport_bw(this, &MyInitiator_nb::nb_transport_bw_func);
        SC_THREAD(send_req_thread);
        SC_THREAD(send_end_resp_thread);
        sensitive << m_test_peq.get_event();
    }


    // from initator to target
    void send_req_thread() {
        int cycle_cnt = 0;
        tlm::tlm_phase t_phase = tlm::BEGIN_REQ;
        auto t_delay = SC_ZERO_TIME;

        while (true) {

            cycle_cnt ++;
            t_delay = sc_time(cycle_cnt, SC_NS);
            auto t_payload = new tlm::tlm_generic_payload();
            t_payload->set_address(0x10000 + cycle_cnt);

            std::cout << this->name() << "\033[34m [" << sc_core::sc_time_stamp() << "]"
                << " call nb_transport_fw, BEGIN_REQ phase, addr=0x" << std::hex << t_payload->get_address()
                << " delay cycle " << cycle_cnt
                << " \033[0m" << std::endl;

            socket->nb_transport_fw(*t_payload, t_phase, t_delay);

            // here add block logic
            // basic block logic : need wait recv end_req phase
            wait(m_slv_end_req_evt);

            wait(1, SC_NS);
        }
    }


    // from target to initator
    tlm::tlm_sync_enum nb_transport_bw_func(tlm::tlm_generic_payload &payload, tlm::tlm_phase &phase, sc_time &delay) {

        switch(phase) {
            case tlm::END_REQ:

                m_slv_end_req_evt.notify();
            case tlm::BEGIN_RESP:

            
        }

        return tlm::TLM_ACCEPTED;
    }

    void send_end_resp_thread() {

    }



    sc_event m_slv_end_req_evt;
    tlm_utils::simple_initiator_socket<MyInitiator_nb> socket;
    tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_test_peq;
};



class MyTarget_nb : public sc_module {
    public:
        SC_CTOR(MyTarget_nb) : socket("socket") {

            socket.register_nb_transport_fw(this, &MyTarget_nb::nb_transport_fw_func);
            SC_THREAD(begin_resp_thread);
            SC_THREAD(main_thread);
        }
 
        tlm::tlm_sync_enum nb_transport_fw_func(tlm::tlm_generic_payload &paylod, tlm::tlm_phase &phase, sc_time &delay) {

        }

        void begin_resp_thread() {

        }

        void main_thread() {

        }

    tlm_utils::simple_target_socket<MyTarget_nb> socket;
    sc_fifo<tlm::tlm_generic_payload *> m_req_fifo;
    sc_fifo<tlm::tlm_generic_payload *> m_resp_fifo;
};


class MyTop : public sc_module {
    public :
        SC_CTOR(MyTop) : m_init("m_init"), m_target("m_target") {
            m_init.socket.bind(m_target.socket);
        }

    MyInitiator_nb m_init;
    MyTarget_nb m_target;
};