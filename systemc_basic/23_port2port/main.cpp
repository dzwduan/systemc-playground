/*
到目前为止，我们介绍了以下情况：
1. 通过通道连接同一模块的两个进程：
process1() --> channel --> process2()
2. 通过端口和通道连接不同模块的两个进程：
module1::process1() --> module1::port1 --> channel --> module2::port2 --> module2::process2()
3. 通过导出连接不同模块的两个进程：
module1::process1() --> module1::channel --> module1::export1 --> module2::port2 --> module2::process2()

在所有这些情况下，都需要一个通道来连接端口。有一种特殊情况允许端口直接连接到子模块的端口。即
module::port1 --> module::submodule::port2
*/


#include "sysc/communication/sc_signal_ifs.h"
#include <systemc>

using namespace sc_core;

// a module that write to an outside channel
SC_MODULE(SUBM1) {

    sc_port<sc_signal_out_if<int>> p;

    SC_CTOR(SUBM1) {
        SC_THREAD(writer);
        // no sensitive
    }

    void writer() {
        int val = 1;
        while (true) {
            p->write(val++); // write to internal channel
            wait(1, SC_SEC);
        }
    }

};

// a module that reads from an outside channel
SC_MODULE(SUBM2) {
    sc_port<sc_signal_in_if<int>> p;

    SC_CTOR(SUBM2) {
        SC_THREAD(reader);
        sensitive << p;     // only sensitive to outside signal
        dont_initialize();
    }

    void reader() {
        while (true) {
            std::cout << sc_time_stamp() << ": reads from outside channel, val=" << p->read() << std::endl;
            wait();
        }
    }
};


SC_MODULE(M1) {
    sc_port<sc_signal_out_if<int>> p;

    SUBM1 sb1;

    SC_CTOR(M1) : sb1("sb1") {
        sb1.p(p);
    }
};


SC_MODULE(M2) {
    sc_port<sc_signal_in_if<int>> p;

    SUBM2 sb2;

    SC_CTOR(M2) : sb2("sb2") {
        sb2.p(p);
    }
};

int sc_main(int argc, char* argv[]) {
    M1 m1("m1");
    M2 m2("m2");

    sc_signal<int> s;
    m1.p(s); // bind port p of m1 to channel(signal) s
    m2.p(s); // bind port p of m2 to channel(signal) s
    sc_start(2, SC_SEC);
    return 0;
}

//module1::sub1::write() --> module1::sub1::p --> module1::p --> s --> module2::p --> module2::sub2::p --> module2::sub2::read()