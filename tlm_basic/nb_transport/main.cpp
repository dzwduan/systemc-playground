
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include "tlm_core/tlm_2/tlm_2_interfaces/tlm_fw_bw_ifs.h"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"
#include "tlm_core/tlm_2/tlm_generic_payload/tlm_phase.h"
#include "tlm_utils/peq_with_get.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include <systemc>

// refer https://blog.csdn.net/zgcjaxj/article/details/126512927
using namespace sc_core;

/**
 * init begin_req + end_resp + register nb_transport_bw_func
 * target end_req + begin_resp + register nb_transport_fw_func

   其中 init 需要注册port + event e + peq_with_get(用于设置路径延迟，需要sensitive << e.event())
   init 有 两个函数：

   send_req_thread : while (true) { paylaod set addr, port->nb_transport_fw, wait(e), wait(1)}
   nb_transport_w_func: switch(pase) {END_REQ: e.notify(); begin_resp : peq.notify(...) }


   target 需要注册port + req_fifo + resp_fifo, 有三个函数，

   nb_transport_fw_func : 根据phase所在阶段，req_begin 写入req_fifo , end_resp do nothing
   end_req_thread : while (true) { read req_fifo, port->nb_transport_bw, write resp_fifo, wait(1)}
   begin_resp_thread : while(true) { read resp_fifo, port->nb_transport_bw, wiat(1)}
 */

class MyInitiator : public sc_module {
public:
  SC_CTOR(MyInitiator) : socket("socket"), peq("peq") {
    socket.register_nb_transport_bw(this, &MyInitiator::nb_transport_bw_func);
    SC_THREAD(SendReqThread);
    SC_THREAD(SendEndRespThread);
    sensitive << peq.get_event();
  }

  tlm::tlm_sync_enum nb_transport_bw_func(tlm::tlm_generic_payload &payload,
                                          tlm::tlm_phase &phase,
                                          sc_time &delay);
  void SendReqThread();
  void SendEndRespThread();

public:
  sc_event end_req_event;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> peq;
  tlm_utils::simple_initiator_socket<MyInitiator> socket;
};

class MyTarget : public sc_module {
public:
  SC_CTOR(MyTarget) : socket("socket") {
    socket.register_nb_transport_fw(this, &MyTarget::nb_transport_fw_func);

    SC_THREAD(EndReqThread);
    SC_THREAD(BeginRespThread);
  };

  tlm::tlm_sync_enum nb_transport_fw_func(tlm::tlm_generic_payload &payload,
                                          tlm::tlm_phase &phase,
                                          sc_time &delay);
  void EndReqThread();
  void BeginRespThread();

  tlm_utils::simple_target_socket<MyTarget> socket;
  sc_fifo<tlm::tlm_generic_payload *> req_fifo;
  sc_fifo<tlm::tlm_generic_payload *> resp_fifo;
};

// init  send req to  target
void MyInitiator::SendReqThread() {
  
}

void MyInitiator::SendEndRespThread() {}

// resp/req start from target to init, call init.nb_transport_bw
tlm::tlm_sync_enum
MyInitiator::nb_transport_bw_func(tlm::tlm_generic_payload &payload,
                                  tlm::tlm_phase &phase, sc_time &delay) {

                                  }

// resp/req end from init to target, call target.nb_transport_fw
tlm::tlm_sync_enum
MyTarget::nb_transport_fw_func(tlm::tlm_generic_payload &payload,
                               tlm::tlm_phase &phase, sc_time &delay) {
                                switch(phase) {

                                }

                                return tlm::TLM_ACCEPTED;
                               }

class MyTop : public sc_module {
public:
  SC_CTOR(MyTop) : m_init("init"), m_target("target") {
    m_init.socket.bind(m_target.socket);
  }

  MyInitiator m_init;
  MyTarget m_target;
};

int sc_main(int argc, char *argv[]) {
  MyTop top("top");
  sc_start(20, sc_core::SC_NS);
  return 0;
}