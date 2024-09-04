#include "tb.h"


sc_event new_stimulus;

// input 无限循环
void TestBench::stim() { //sc_thread
    ch_x = 3; ch_y = 4;
 //   wait(10, SC_NS);
    new_stimulus.notify();
    ch_x = 5; ch_y = 6;
    new_stimulus.notify();
    ch_x = 7; ch_y = 8;
    new_stimulus.notify();
 //   wait(10, SC_NS);
}

void TestBench::check() { // sc_Method
    // cout << ch_x <<" "<< ch_y <<" "<< ch_s << endl;
    // if (ch_s != ch_x + ch_y) sc_stop();
    // else cout << "-> OK" << endl;

    for(;;) {
        wait(new_stimulus);
        if (ch_s != ch_x + ch_y) cout << "Error" << endl;
        else cout << "-> OK" << endl;
    }
}