#include <systemc.h>
#include "axi_slave_interface.h"
#include "sysc/kernel/sc_module.h"


SC_MODULE(AxiSlave) {
    // interface
    sc_in<bool> en;
    sc_port<AXI_2_Slave_Port> mod_interface{"Module_interface"};
    AXI_S_Interface axi_interface;

    // Internal signals
    sc_signal<int> address;
    sc_signal<uint8_t> length, data;

    SC_CTOR(AxiSlave) {
        SC_THREAD(handshake_process);
        sensitive << en;
        sensitive << axi_interface.clk.pos();

        sensitive << axi_interface.AW_address;
        sensitive << axi_interface.AW_valid;
        sensitive << axi_interface.W_data;
        sensitive << axi_interface.W_last;
        sensitive << axi_interface.W_valid;
        sensitive << axi_interface.B_ready;

        sensitive << axi_interface.AR_address;
        sensitive << axi_interface.AR_valid;
        sensitive << axi_interface.R_ready;
    }

    void handshake_process() {
        int counter, r_err, w_err;
        Read_Response r_resp;

        while(true) {
            axi_interface.AW_ready = false;
            axi_interface.AR_ready = false;
            axi_interface.W_ready = false;
            axi_interface.B_valid = false;
            axi_interface.R_valid = false;

            if (en.read() == true && mod_interface->is_ready() == true) {
                if (PRINT_AXI_SLAVE_DEBUG)
                  cout << "@" << sc_time_stamp() << " [SENSOR INFO] : waiting for a read or write request..." << endl;

                // if both read/write addr false, wait
                if(axi_interface.AW_valid->read() ==  false && axi_interface.AR_valid.read() == false) {
                    wait(axi_interface.AW_valid->posedge_event() | axi_interface.AR_valid->posedge_event());
                }

                mod_interface->start_transaction(); //TODO:

                // read : 收到addr
                if(axi_interface.AR_valid->read() == true) {
                    while (mod_interface->is_ready() == false) wait(axi_interface.clk.posedge_event());
                }
                else if (axi_interface.AW_valid->read() == true) {

                } 
                else {
                    // reset all

                    // 读突发最后一次传输
                    axi_interface.R_last = false; 
                    // 从机读数据
                    axi_interface.R_data = 0;
                    // 从机写响应，表示写传输状态
                    axi_interface.B_resp = 0;
                }
            }
        }
    }
};