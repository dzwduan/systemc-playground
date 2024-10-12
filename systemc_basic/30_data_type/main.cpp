// Learn with Examples, 2020, MIT license
#include <systemc>
using namespace sc_core;

/*
sc_signal<T> 和 sc_fifo<T> 可与各种数据类型一起使用。SystemC 已支持内置数据类型 T。
为了将自定义数据类型用于 sc_signal 和 sc_fifo，应为该数据类型实现以下成员函数：
1. 赋值运算符，即 operator=()：读写方法需要
2. 相等运算符，即 operator==()：sc_signal 需要用于 value_changed_event()
3. 流式输出，即 ostream& operator<<()：打印数据结构需要
4. sc_trace()：允许将数据类型与 systemC 跟踪工具一起使用；允许使用 wavefor 查看器查看跟踪数据。
*/


struct CUSTOMIZED_TYPE {
  int x, y;

  CUSTOMIZED_TYPE(int x = 0, int y = 0) : x(x), y(y) {}
  CUSTOMIZED_TYPE& operator=(const CUSTOMIZED_TYPE& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  bool operator==(const CUSTOMIZED_TYPE& rhs) {
    return x== rhs.x && y == rhs.y;
  }
};

  std::ostream& operator<<(std::ostream& os, const CUSTOMIZED_TYPE& val) {
    os << "x = " << val.x << "; y = " << val.y << std::endl;
    return os;
  }

  inline void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) {
    sc_trace(f, val.x, name + ".x");
    sc_trace(f, val.y, name + ".y");
  }

SC_MODULE(MODULE) { // test module
  sc_signal<CUSTOMIZED_TYPE> s; // customized signal
  SC_CTOR(MODULE) { // constructor
    SC_THREAD(writer); // writer process
    SC_THREAD(reader); // reader process
    sensitive << s; // sensitive to customized signal s
    dont_initialize();
  }
  void writer() {
    int x = 1; // init signal
    int y = 2;
    while (true) {
      s.write(CUSTOMIZED_TYPE{x++, y++}); // write to signal
      wait(1, SC_SEC); // wait 1 s
    }
  }
  void reader() {
    while (true) {
      std::cout << sc_time_stamp() << ": receives " << s.read() << std::endl; // read from signal
      wait(); // wait for value_changed_event
    }
  }
};

int sc_main(int, char*[]) {
  MODULE module("module"); // instantiate module
  sc_trace_file* file = sc_create_vcd_trace_file("trace"); // open trace file
  sc_trace(file, module.s, "customized_type");  // trace customized signal
  sc_start(2, SC_SEC); // run simulation for 2 s
  sc_close_vcd_trace_file(file); // close trace file
  return 0;
}