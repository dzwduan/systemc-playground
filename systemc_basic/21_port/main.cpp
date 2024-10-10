/*
通信的三个关键概念：
1. interface：
a) 从 sc_interface 派生但不从 sc_object 派生的抽象类。
b) 包含一组纯虚拟函数，这些函数应在从该interface派生的一个或多个channel中定义。
2. port：
a) 提供编写模块的方法，使其独立于实例化的上下文。
b) 将interface方法调用转发到port绑定的channel。
c) 定义包含port的模块所需的一组服务（由port类型标识）。
3. channel：
a) sc_prim_channel 是所有原始channel的基类。
b) channel可以提供可使用interface方法调用范例调用的公共成员函数。
c) 原始channel应实现一个或多个interface。

简而言之：
port需要服务，interface定义服务，channel实现服务。
如果channel实现了port所需的interface，则port可以连接（绑定）到channel。
port基本上是指向channel的指针。

何时使用port：
1. 如果模块要调用属于模块本身之外的channel的成员函数，则应使用通过模块port的interface方法调用进行该调用。否则会被视为不良编码风格。
2. 但是，可以直接调用属于当前模块内实例化的channel的成员函数。这称为无port channel访问。
3. 如果模块要调用属于子模块内channel实例的成员函数，则应通过子模块的导出进行该调用。
*/


#include <systemc>

using namespace sc_core;

// a module that write to an outside channel
SC_MODULE(M1) {
    sc_signal<int> s;
    sc_port<sc_signal_out_if<int>> p;

    SC_CTOR(M1) {
        SC_THREAD(selfWrite);
        SC_THREAD(selfRead);

        sensitive << s;
        dont_initialize();

        SC_THREAD(outsideWrite);
    }

    void selfWrite() {
        int val = 1;
        while (true) {
            s.write(val++);
            wait(1, SC_SEC);
        }
    }

    void selfRead() {
        while(true) {
            std::cout << sc_time_stamp() << ": reads from own channel, val=" << s.read() << std::endl;
            wait();
        }
    }

    void outsideWrite() {
        int val = 1;
        while(true) {
            p->write(val++); //write to an outside channel, calls the write method of the outside channel. p is a pointer
            wait(1, SC_SEC);
        }
    }
};

// a module that reads from an outside channel
SC_MODULE(M2) {
    sc_port<sc_signal_in_if<int>> p;

    SC_CTOR(M2) {
        SC_THREAD(outsideRead);
        sensitive << p;
        dont_initialize();
    }

    void outsideRead() {
        while (true) {
            std::cout << sc_time_stamp() << ": reads from outside channel, val=" << p->read() << std::endl;
            wait();
        }
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