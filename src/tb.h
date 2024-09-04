
#include "adder.h"

class TestBench : public sc_module {
    public:
    sc_signal<int> ch_x, ch_y, ch_s;

    Adder uut;
    void stim();
    void check();

    SC_HAS_PROCESS(TestBench); // sc-3.0 depulicated
    TestBench(sc_module_name nm) : sc_module(nm), uut("uut"), ch_x("ch_x"), ch_y("ch_y") {
        SC_THREAD(stim);  // only start once, call wait,
        SC_METHOD(check); // non-blocking , sensitive related
        sensitive << ch_s;

        uut.x(ch_x);
        uut.y(ch_y);
        uut.s(ch_s);
    }
};