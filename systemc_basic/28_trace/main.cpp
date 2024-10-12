/*
跟踪文件：
1. 记录模拟过程中按时间顺序排列的值变化序列。
2. 使用 VCD（值变化转储）文件格式。
3. 只能由 sc_create_vcd_trace_file 创建和打开。
4. 可以在阐述期间或模拟期间的任何时间打开。
5. 包含只能由 sc_trace 跟踪的值。
6. 应在将值跟踪到该文件之前打开，并且如果自打开文件以来已过去一个或多个增量周期，则不应将值跟踪到给定的跟踪文件。
7. 应由 sc_close_vcd_trace_file 关闭。跟踪文件不应在模拟的最后一个增量周期之前关闭。
*/

// Learn with Examples, 2020, MIT license
#include "sysc/tracing/sc_trace.h"
#include <systemc>
using namespace sc_core;


SC_MODULE(MODULE) {
  sc_port<sc_signal<int>> p;
  SC_CTOR(MODULE) {
    SC_THREAD(writer);
  }

  void writer() {
    int v = 1;
    while (true) {
      p->write(v++);
      wait(1, SC_SEC);
    }
  }
};


int sc_main(int argc, char *argv[]) {
  MODULE m("m");
  sc_signal<int> s;

  m.p(s);

  sc_trace_file *file = sc_create_vcd_trace_file("trace");
  sc_trace(file, s, "signal");
  sc_start(5, SC_SEC);
  sc_close_vcd_trace_file(file);
  return 0;
}
