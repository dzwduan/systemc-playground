#include "sysc/kernel/sc_simcontext.h"
#include <systemc>

/**
 * method: 没有自己的执行线程，不消耗模拟时间，不能暂停，不能调用wait
 * thread: 有自己的执行线程，可能消耗模拟时间，可以暂停，可以调用wait
 * 
 * next_trigger : 只有method可以调用，来创建动态sensitive
 */

 using namespace sc_core;

 SC_MODULE(PROCESS) {
    SC_CTOR(PROCESS) {
        SC_THREAD(thread);
        SC_METHOD(method);
    }

    // 结果中，idx正常++
    void thread() {
        int idx = 0;
        while (true) {
            std::cout << "thread" << idx++ << " @ " << sc_time_stamp() << std::endl;
            wait(1, SC_SEC);
        }
    }

    // 结果中，idx并不会增加，始终为idx = 0，因为method中声明的局部变量会被销毁！如果设置class中的idx2，就不一样了
    void method() {
        int idx = 0;
        std::cout << "method" << idx++ << " @ " << sc_time_stamp() << std::endl;
        next_trigger(1, SC_SEC);
    }

    // int idx2 = 0;
 };


 int sc_main(int argc, char *argv[]) {
    PROCESS process("process");
    sc_start(4, SC_SEC);
    return 0;
 }