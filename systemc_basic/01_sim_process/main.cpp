#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext.h"
#include "systemc"

using namespace sc_core;

/**
模拟过程：
1. 是 sc_module 类的成员函数，并且
3. 没有输入参数且不返回任何值，并且
2. 已向模拟内核注册

如何注册模拟过程：
1. SC_METHOD(func)：没有自己的执行线程，不消耗模拟时间，不能暂停，也不能调用 调用 wait() 的代码
2. SC_THREAD(func)：有自己的执行线程，可能消耗模拟时间，可以暂停，可以调用 调用 wait() 的代码
3. SC_CTHREAD(func, event)：SC_THREAD 的特殊形式，只能对时钟边沿事件具有静态敏感性

何时可以注册：
1. 在构造函数主体中，
2. 在模块的 before_end_of_elaboration 或 end_of_elaboration 回调中，
3. 或在从构造函数或回调调用的成员函数中

限制：
1. 注册只能在以下情况下执行在同一模块的成员函数上。
2. 不应从 end_of_elaboration 回调中调用 SC_CTHREAD。

注意：
1. SC_THREAD 可以执行 SC_METHOD 或 SC_CHTEAD 所做的所有操作。我将在示例中主要使用此过程。
2. 为了再次调用 SC_THREAD 或 SC_CTHREAD 进程，应有一个 while 循环确保它永远不会退出。
3. SC_THREAD 进程不需要 while 循环。它由 next_trigger() 再次调用
4. systemC 中的模拟时间不是程序运行的实际时间。它是由模拟内核管理的计数器。稍后解释。
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
