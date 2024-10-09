#include <systemc>

/**
 * sc_fifo：
1. 是预定义的原始通道，旨在模拟 fifo 的行为，即先进先出缓冲区。
2. 具有多个用于存储值的slots。构造对象时，slots数量是固定的。
3. 实现 sc_fifo_in_if<T> 接口和 sc_fifo_out_if<T> 接口。

构造函数：
1. 显式 sc_fifo(int size_ = 16)：从其初始化列表中调用基类构造函数，如下所示：sc_prim_channel(sc_gen_unique_name( "fifo" ))
2. 显式 sc_fifo(const char* name_, int size_ = 16)：从其初始化列表中调用基类构造函数，如下所示：sc_prim_channel(name_)
两个构造函数都将 fifo 中的slots数量初始化为参数 size_ 给出的值。slots数量应大于零。

读取的成员函数：
1. void read(T&)、T read()：
a) 返回最近最少写入 fifo 的值，并从 fifo 中删除该值，使其无法再次读取。
b) 从 fifo 读取值的顺序应与值写入 fifo 的顺序完全匹配。
c) 在当前增量周期内写入 fifo 的值在该增量周期内不可读取，但可在紧接着的增量周期内读取。
d) 如果 fifo 为空，则应暂停，直到收到数据写入事件通知。
2. bool nb_read(T&)：
a)、b)、c) 与 read() 相同
d) 如果 fifo 为空，成员函数 nb_read 应立即返回，不修改 fifo 的状态，不调用 request_update，返回值为 false。否则，如果有值可供读取，则 nb_read() 的返回值应为 true。
3. 运算符 T()：等同于“运算符 T() {return read();}”

write 的成员函数：
1. write(const T&)：
a) 将作为参数传递的值写入 fifo。
b) 可以在单个增量周期内写入多个值。
c) 如果在当前增量周期期间从 fifo 读取值，则这样创建的 fifo 中的空槽直到紧接着的增量周期才会变为空闲状态以供写入。
d) 如果 fifo 已满，write() 应暂停，直到收到数据读取事件通知。
2. bool nb_write(const T&)：
a)、b)、c) 与 write() 相同
d) 如果 fifo 已满，nb_write() 应立即返回，不修改 fifo 的状态，不调用 request_update，返回值为 false。否则，nb_write() 的返回值应为 true。
3. 运算符=：等同于“sc_fifo <T>＆运算符=（const T＆a）{写入（a）； return *this;}"

事件的成员函数：
1. sc_event& data_written_event()：应返回对数据写入事件的引用，该事件在增量周期结束时发生的增量通知阶段中通知，在该阶段中将值写入 fifo。
2. sc_event& data_read_event()：应返回对数据读取事件的引用，该事件在增量周期结束时发生的增量通知阶段中通知，在该阶段中从 fifo 读取值。

可用值和空闲槽的成员函数：
1. int num_available()：返回当前增量周期中可供读取的值的数量。计算应扣除当前增量周期中读取的任何值，但不得添加当前增量周期中写入的任何值。
2. int num_free()：返回当前增量周期中可供写入的空槽数量。计算应扣除当前增量周期中写入的任何槽，但不得添加通过读取当前增量周期来添加任何空闲的slots。
 * 
 */