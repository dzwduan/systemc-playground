
/*
sc_signal：
1. 是预定义的原始通道，旨在模拟携带数字电子信号的单条电线的行为。
2. 使用评估更新方案来确保在同时进行读写操作的情况下确定性行为。我们维护当前值和新值。
3. 如果新值与当前值不同，其 write() 方法将提交更新请求。
4. 实现 sc_signal_inout_if<T> 接口。

构造函数：
1. sc_signal()：从其初始化列表中调用基类构造函数，如下所示：sc_prim_channel(sc_gen_unique_name("signal"))
2. sc_signal(const char*name_)：从其初始化列表中调用基类构造函数，如下所示：sc_prim_channel(name_)

成员函数：
1. T& read() 或 operator const T&()：返回对信号当前值的引用，但不得修改信号的状态。
2. void write(const T&)：修改信号的值，使信号在下一个增量周期（但在此之前）似乎具有新值（由成员函数read 返回）。
3. operator=：相当于 write()
4. sc_event& default_event()、sc_event& value_changed_event()：返回对值更改事件的引用。
5. boolevent()：当且仅当信号值在紧接的增量周期的更新阶段和当前模拟时间发生变化时，才返回 true。

与 fifo 相比：
1. sc_signal 只有一个用于读/写的插槽
2. sc_signal 仅在新值与当前值不同时触发更新请求
3. 从 sc_signal 读取不会删除该值

除执行阶段外，sc_signal：
1. 可以在阐述期间写入以初始化信号的值。
2. 可以在详细说明期间或模拟暂停时从函数 sc_main 写入，即在调用函数 sc_start之前或之后。
*/

#include <systemc>

using namespace sc_core;

SC_MODULE(SIGNAL) {

  sc_signal<int> s;
  SC_CTOR(SIGNAL) {
    SC_THREAD(read_write);
  }

  void read_write() { 
    s.write(3); 
    // s and s.read same value
    std::cout << "s = " << s << "; " << s.read() << std::endl;
    wait(SC_ZERO_TIME);
    std::cout << "after delta_cycle, s = " << s << std::endl;
    s = 4;
    s = 5;
    int tmp = s;
    std::cout << "s = " << tmp << std::endl;
    wait(SC_ZERO_TIME);
    std::cout << "after delta_cycle, s = " << s.read() << std::endl;
  }
};

int sc_main(int argc, char *argv[]) {
  SIGNAL s("s");
  s.s = -1;
  sc_start();
  return 0;
}
