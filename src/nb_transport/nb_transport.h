#include "sysc/kernel/sc_module.h"
#include "systemc.h"
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


    void send_req_thread() {

    }


    tlm::tlm_sync_enum nb_transport_bw_func(tlm::tlm_generic_payload &payload, tlm::tlm_phase &phase, sc_time &delay) {

    }

    void send_end_resp_thread() {

    }



    sc_event m_slv_req_evt;
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