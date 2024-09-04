#include "tb.h"
#include <systemc.h>

int sc_main(int argc, char *argv[]) {
    TestBench tb("tb");

// add wave support
    sc_trace_file *tf = sc_create_vcd_trace_file("wave");
    sc_trace(tf, tb.ch_x, "ch_x");
    sc_trace(tf, tb.ch_y, "ch_y");
    sc_trace(tf, tb.ch_s, "s");

    sc_start();
    cout << "Simulation finished" << endl;

    sc_close_vcd_trace_file(tf);
    return 0;
}