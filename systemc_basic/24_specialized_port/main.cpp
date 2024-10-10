/*
除了使用基本的 sc_port
类来声明端口外，还有各种专用端口类可用于不同的通道类型，或提供附加功能：
1. sc_in：用于信号的专用端口类。
2. sc_fifo_in 是从 fifo 读取时使用的专用端口类。
3. sc_fifo_out 是写入 fifo 时使用的专用端口类。
4. sc_in<bool> 和 sc_in<sc_dt::sc_logic>：value_changed()、pos()、neg()
5. sc_inout 是用于信号的专用端口类：value_changed()、initialize()
6. sc_inout<bool> 和 sc_inout<sc_dt::sc_logic>
是专用端口类，它们为二值信号提供附加成员函数：value_changed()、initialize()、pos()、neg()
7. sc_out 派生自 sc_inout 类，与 sc_inout
类相同，除了作为派生类所固有的差异（例如构造函数和赋值运算符）之外。
8. sc_in_resolved 是用于解析信号的专用端口类。它类似于它派生自的
sc_in<sc_dt::sc_logic>。唯一的区别是，sc_in_resolved 类的端口应绑定到
sc_signal_resolved 类的通道，而 sc_in<sc_dt::sc_logic> 类的端口可绑定到
sc_signal<sc_dt::sc_logic,WRITER_POLICY> 或 sc_signal_resolved 类。
9. sc_inout_resolved 是用于已解析信号的专用端口类。其行为类似于其派生自的端口类
sc_inout<sc_dt::sc_logic>。唯一的区别是，sc_inout_resolved 类的端口应绑定到
sc_signal_resolved 类的通道，而 sc_inout<sc_dt::sc_logic> 类的端口可绑定到
sc_signal<sc_dt::sc_logic,WRITER_POLICY> 或 sc_signal_resolved 类的通道。
10. sc_out_resolved 派生自 sc_inout_resolved
类，除了作为派生类所固有的差异（例如构造函数和赋值运算符）外，它与
sc_inout_resolved 类完全相同。
11. sc_in_rv 是用于已解析信号的专用端口类。其行为类似于其派生自的端口类
sc_in<sc_dt::sc_lv<W>>。唯一的区别是，sc_in_rv 类的端口应绑定到 sc_signal_rv
类的通道，而 sc_in<sc_dt::sc_lv<W>> 类的端口可以绑定到
sc_signal<sc_dt::sc_lv<W>,WRITER_POLICY> 类或 sc_signal_rv 类的通道。
12. sc_inout_rv 是用于已解析信号的专用端口类。它的行为类似于它所派生的端口类
sc_inout<sc_dt::sc_lv<W>>。唯一的区别是，类 sc_inout_rv 的端口应绑定到类
sc_signal_rv 的通道，而类 sc_inout<sc_dt::sc_lv<W>> 的端口可以绑定到
sc_signal<sc_dt::sc_lv<W>,WRITER_POLICY> 或类 sc_signal_rv。
13. sc_out_rv 派生自类
sc_inout_rv，除了作为派生类所固有的差异（例如构造函数和赋值运算符）外，它与类
sc_inout_rv 完全相同。

基本 sc_port<sc_signal_inout_if<int>> 只能访问信号通道提供的成员函数：
1. read()
2. write()
3. default_event() // 当使用端口通过 sc_sensitive 类的 operator<<
定义静态敏感度时调用。
4. event() // 检查事件是否发生，返回 true/false
5. value_changed_event() // 值改变事件

sc_port<sc_signal_inout_if<bool>> 可以访问 signal<bool>
通道提供的这些附加成员函数：
6. posedge() // 如果值从 false 变为 true，则返回 true
7. posedge_event() // 值从 false 变为 true，则事件
8. negedge() // 如果值从 true 变为 false，则返回 true
9. negedge_event() // 值从 tru 变为 false，则事件

sc_inout<> 的专用端口提供了附加成员函数：
10. initialize() // 在端口绑定到通道之前初始化端口值
11. value_changed() // 用于在端口绑定到通道之前建立灵敏度（指针未初始化）

当下划线信号通道为 bool 或 sc_logic 类型时，sc_inout<bool>
提供另外两个成员函数：
12. pos() // 在端口绑定前建立灵敏度
13. neg() // 在端口绑定前建立灵敏度

上面列出的成员函数中：
1~9 由信号通道提供，可通过“port->method()”访问；
10~13 由专用端口提供，可通过“port.method()”访问。
*/

#include <systemc>

using namespace sc_core;

SC_MODULE(WRITER) {
  sc_out<bool> p1, p2;

  SC_CTOR(WRITER) {
    SC_THREAD(writer);
    p1.initialize(true);
  }

  void writer() {
    bool v = true;

    while (true) {
      p1->write(v);
      v = !v;
      wait(1, SC_SEC);
    }
  }
};

SC_MODULE(READER) {
  sc_in<bool> p1, p2;

  SC_CTOR(READER) {
    SC_THREAD(reader1);
    sensitive << p1 << p2;
    dont_initialize();

    SC_THREAD(reader2);
    sensitive << p1.value_changed();
    dont_initialize();

    SC_THREAD(reader3);
    sensitive << p1.neg();
    dont_initialize();

    SC_THREAD(reader4);
    sensitive << p1.pos();
    dont_initialize();
  }

  void reader1() {
    while (true) {

      std::cout << sc_time_stamp() << ": default_event. p1 = " << p1->read()
                << "; p1 triggered? " << p1->event() << "; p2 triggered? "
                << p2->event() << std::endl;
      wait();
    }
  }

  void reader2() {
    while (true) {
      std::cout << sc_time_stamp()
                << ": value_changed_event. p1 = " << p1->read() << std::endl;
      wait();
    }
  }

  void reader3() {
    while (true) {

      std::cout << sc_time_stamp() << ": negedge_event. p1 = " << p1->read()
                << "; negedge = " << p1->negedge() << std::endl;
      wait();
    }
  }

  void reader4() {
    while (true) {
      std::cout << sc_time_stamp() << ": posedge_event. p1 = " << p1->read()
                << "; posedge = " << p1->posedge() << std::endl;
      wait();
    }
  }
};


int sc_main(int, char*[]) {
//instantiate writer
  WRITER writer("writer");
//instantiate reader
  READER reader("reader");
//declare boolean signal channel
  sc_signal<bool> b1, b2;
//port binding
  writer.p1(b1);
  writer.p2(b2);
  reader.p1(b1);
  reader.p2(b2);
  sc_start(4, SC_SEC);
  return 0;
}