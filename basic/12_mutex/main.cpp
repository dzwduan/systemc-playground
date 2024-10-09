#include "sysc/communication/sc_mutex.h"
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include <systemc>


using namespace sc_core;

/**
互斥锁：
1. 是预定义通道，旨在模拟用于控制对并发进程共享的资源的访问的互斥锁的行为。
2. 应处于两种互斥状态之一：解锁或锁定：
a) 一次只有一个进程可以锁定给定的互斥锁。
b) 互斥锁只能由锁定它的进程解锁。解锁后，互斥锁可能被其他进程锁

 成员函数：
1. int lock()：
a) 如果互斥锁已解锁，lock() 应锁定互斥锁并返回。
b) 如果互斥锁已锁定，lock() 应暂停，直到互斥锁被解锁（由另一个进程解锁）。
c) 如果多个进程尝试在同一个增量周期内锁定互斥锁，则选择将锁赋予哪个进程实例应是不确定的。
d) 应无条件返回值 0。
2. int trylock()：
a) 如果互斥锁已解锁，trylock() 应锁定互斥锁并返回值 0。
b) 如果互斥锁已锁定，trylock() 应立即返回值 –1。互斥锁应保持锁定状态。
3. int unlock()：
a) 如果互斥锁已解锁，unlock() 应返回值 –1。互斥锁应保持解锁状态。
b) 如果互斥锁被调用进程以外的进程实例锁定，unlock() 应返回值 –1。互斥锁应保持锁定状态。
c) 如果互斥锁被调用进程锁定，成员函数 unlock 应解锁互斥锁并返回值 0。
1. 应使用立即通知向其他进程发出解锁互斥锁操作的信号 
 * 
 */


SC_MODULE(MUTEX) {
    sc_mutex m;
    SC_CTOR(MUTEX) {
        SC_THREAD(thread1);
        SC_THREAD(thread2);

        // no static sensitivity
    }

    void thread1() {
        while (true) {
            if (m.trylock() == -1) {
                m.lock();
                std::cout << sc_time_stamp() << ": thread_1 obtained resource by lock()" << std::endl;
            } else {
                // 一开始没有锁，调用trylock之后加锁
                std::cout << sc_time_stamp() << ": thread_1 obtained resource by trylock()" << std::endl;
            }

            wait(1, SC_SEC);
            m.unlock();
            std::cout << sc_time_stamp() << ": unlocked by thread_1" << std::endl;
            wait(SC_ZERO_TIME);
        }
    }

    void thread2() {
        while (true) {
            if (m.trylock() == -1) {
                m.lock();
                std::cout << sc_time_stamp() << ": thread_2 obtained resource by lock()" << std::endl;
            } else {
                std::cout << sc_time_stamp() << ": thread_2 obtained resource by trylock()" << std::endl;
            }

            wait(1, SC_SEC);
            m.unlock();
            std::cout << sc_time_stamp() << ": unlocked by thread_2" << std::endl;
            wait(SC_ZERO_TIME);
        }
    }
};


int sc_main(int argc, char *argv[]) {
    MUTEX mutex("mutex");
    sc_start(4, SC_SEC);
    return 0;
}