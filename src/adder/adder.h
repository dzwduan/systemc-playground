#include <systemc.h>

SC_MODULE(Adder) {
  sc_in<int> x;
  sc_in<int> y;
  sc_out<int> s;

  void add() { s.write(x.read() + y.read()); }

  // 非阻塞
  SC_CTOR(Adder) {
    SC_METHOD(add);
    sensitive << x << y;
  }
};