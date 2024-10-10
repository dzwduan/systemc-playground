#include "sysc/communication/sc_semaphore.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include <systemc>

/**
 * 信号量：
1. 是预定义通道，旨在模拟用于提供对共享资源的有限并发访问的软件信号量的行为。
2. 具有一个整数值，即信号量值，该值在构造信号量时设置为允许的并发访问数。
a) 如果初始值为 1，则信号量相当于互斥锁。

成员函数：
1. int wait()：
a) 如果信号量值大于 0，wait() 应减少信号量值并返回。
b) 如果信号量值等于 0，wait() 应暂停，直到信号量值增加（由另一个进程增加）。
c) 应无条件返回值 0。
2. int trywait()：
a) 如果信号量值大于 0，trywait() 应减少信号量值并返回值 0。
b) 如果信号量值等于 0，trywait() 应立即返回值 -1，而不修改信号量值。
3. int post()：
a) 应增加信号量值。
b) 应使用立即通知向任何等待进程发出增加信号量值的操作信号。
c) 应无条件返回值 0。
4. int get_value()：应返回信号量值。
 *
 */

using namespace sc_core;

SC_MODULE(SEMAPHORE) {
  sc_semaphore s;
// no static sensitivity
  SC_CTOR(SEMAPHORE) : s(2) {
    SC_THREAD(thread1);
    SC_THREAD(thread2);
    SC_THREAD(thread3);
  }

  void thread1() {
    while (true) {
      if (s.trywait() == -1) {
        s.wait();
      }
      // 调用trywait，加锁
      std::cout << sc_time_stamp() << ": locked by thread_1, value is "
                << s.get_value() << std::endl;
      wait(1, SC_SEC);
      s.post(); // 释放之后，增加semaphore
      std::cout << sc_time_stamp() << ": unlocked by thread_1, value is "
                << s.get_value() << std::endl;
      wait(SC_ZERO_TIME); // give time for the other process to lock
    }
  }

  void thread2() {
    while (true) {
      if (s.trywait() == -1) {
        s.wait();
      }
      // 调用trywait，加锁
      std::cout << sc_time_stamp() << ": locked by thread_2, value is "
                << s.get_value() << std::endl;
      wait(1, SC_SEC);
      s.post(); // 释放之后，增加semaphore
      std::cout << sc_time_stamp() << ": unlocked by thread_2, value is "
                << s.get_value() << std::endl;
      wait(SC_ZERO_TIME); // give time for the other process to lock
    }
  }

  void thread3() {
    while (true) {
      if (s.trywait() == -1) {
        s.wait();
      }
      // 调用trywait，加锁
      std::cout << sc_time_stamp() << ": locked by thread_3, value is "
                << s.get_value() << std::endl;
      wait(1, SC_SEC);
      s.post(); // 释放之后，增加semaphore
      std::cout << sc_time_stamp() << ": unlocked by thread_3, value is "
                << s.get_value() << std::endl;
      wait(SC_ZERO_TIME); // give time for the other process to lock
    }
  }
};

int sc_main(int argc, char *argv[]) {
  SEMAPHORE semaphore("semaphore");
  sc_start(4, SC_SEC);
  return 0;
}
