/**
sc_buffer 是从 sc_signal 类派生的预定义原始通道。
它与 sc_signal 类的不同之处在于，每当写入缓冲区时都会通知值更改事件，而不是仅在信号值更改时通知。
例如，
如果“信号”的当前值 == 1：向其写入 1 不会触发值更新事件。
如果“缓冲区”的当前值 == 1：向其写入 1 将触发值更新事件。
*/

#include <systemc>

using namespace sc_core;

SC_MODULE(BUFFER) {

};