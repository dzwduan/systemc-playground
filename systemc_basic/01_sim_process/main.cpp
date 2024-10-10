#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext.h"
#include "systemc"

using namespace sc_core;

/**
    1. SC_METHOD(func): does not have its own thread of execution, consumes no simulated time, cannot be suspended, and cannot call code that calls wait()
    2. SC_THREAD(func): has its own thread of execution, may consume simulated time, can be susupended, and can call code that calls wait()
    3. SC_CTHREAD(func, event): a special form of SC_THREAD thatcan only have a static sensitivity of a clock edge event
 */


SC_MODULE(PROCESS) {
    sc_clock clk;

    SC_CTOR(PROCESS) : clk("clk", 1, SC_SEC) {
        SC_METHOD(method);
        SC_THREAD(thread);
        SC_CTHREAD(cthread, clk);
    }

    // cant use wait, cant be suspended
    void method() {
        std::cout << "method triggered @ " << sc_time_stamp() << std::endl;
        next_trigger(sc_time(1, SC_SEC));
    }

    void thread() {
        while (true) {
            std::cout << "thread triggered @ " << sc_time_stamp() << std::endl;
            wait();
        }
    }

    void cthread() {
        while (true) {
            std::cout << "cthread triggered @ " << sc_time_stamp() << std::endl;
            wait();
        }
    }
};

int sc_main(int argc, char *argv[]) {

    PROCESS process("process");

    std::cout << "Execution phase begin @ " << sc_time_stamp() << std::endl;
    sc_start(2, SC_SEC);
    std::cout << "Execution phase end @ " << sc_time_stamp() << std::endl;
    return 0;
}   
