#include "tb.h"


// input 无限循环
void TestBench::stim() { //sc_thread
    ch_x = 3; ch_y = 4;
    wait(10, SC_NS);
    ch_x = 7; ch_y = 8;
    wait(10, SC_NS);
}

void TestBench::check() { // sc_Method
    cout << ch_x <<" "<< ch_y <<" "<< ch_s << endl;
    if (ch_s != ch_x + ch_y) sc_stop();
    else cout << "-> OK" << endl;
}