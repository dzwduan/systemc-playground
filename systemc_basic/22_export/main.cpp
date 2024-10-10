/*
导出：
1. 允许模块为其父模块提供接口。
2. 将接口方法调用转发到导出所绑定的通道。
3. 定义一组由包含导出的模块提供的服务。

何时使用导出：
1. 通过导出提供接口是模块简单实现接口的替代方法。
2. 使用显式导出允许单个模块实例以结构化方式提供多个接口。
3. 如果模块要调用属于子模块内通道实例的成员函数，则应通过子模块的导出进行该调用。
*/


#include <systemc>

using namespace sc_core;

// a module that write to an outside channel
SC_MODULE(M1) {
    sc_signal<int> s;
    sc_export<sc_signal<int>> p;

    SC_CTOR(M1) {
        p(s);  // bind export to internal channel
        SC_THREAD(writer);
        // no sensitive
    }

    void writer() {
        int val = 1;
        while (true) {
            s.write(val++); // write to internal channel
            wait(1, SC_SEC);
        }
    }

};

// a module that reads from an outside channel
SC_MODULE(M2) {
    sc_port<sc_signal_in_if<int>> p;

    SC_CTOR(M2) {
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


int sc_main(int argc, char* argv[]) {
    M1 m1("m1");
    M2 m2("m2");

    m2.p(m1.p);

    // sc_signal<int> s;
    // m1.p(s); // bind port p of m1 to channel(signal) s
    // m2.p(s); // bind port p of m2 to channel(signal) s
    sc_start(2, SC_SEC);
    return 0;
}