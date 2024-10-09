#include "systemc.h"

using namespace sc_core;

/**
 * sc_start之后会进程初始化，初始化执行3步骤：
 * 1. update phase, 不执行 delta notification phase
 * 2. 将method thread进程添加到可运行的process中（不包含调用dont_initialize，时钟线程）
 * 3. 运行delta notification phase
 */

SC_MODULE(INIT) {
    sc_event e;
    SC_CTOR(INIT) {
        SC_THREAD(trigger);
        SC_THREAD(catcher_1);
        SC_THREAD(catcher_2);
        SC_THREAD(catcher_3);
        sensitive << e;

        dont_initialize(); // 阻止调度程序在初始化阶段执行thread/method 进程，适用于最后声明的进程，比如这里的catcher_3
    }

    void trigger() {
        while(true) {
            // e triggered at 1 3 5 7
            e.notify(1, SC_SEC);
            wait(2, SC_SEC);
        }
    }

    void catcher_1() {
        while (true) {
            std::cout << sc_time_stamp() << ": catcher_1 triggered" << std::endl;
            wait(e);
        }
    }

    void catcher_2() {
        wait(e);
        while (true) {
            std::cout << sc_time_stamp() << ": catcher_2 triggered" << std::endl;
            wait(e);
        }
    }

    void catcher_3() { // relies on dont_initialize() to avoid initialization
        while (true) {
            std::cout << sc_time_stamp() << ": catcher_3 triggered" << std::endl;
            wait(e);  // dynamic sensitivity
        }
    }
};



int sc_main(int argc, char *argv[]) {
    INIT init("init");
    sc_start(4, SC_SEC);
    return 0;
}