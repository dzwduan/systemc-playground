#include "nb_transport.h"
#include "sysc/kernel/sc_time.h"


int sc_main(int argc, char *argv[]) {
    MyTop top("top");
    sc_start(20, sc_core::SC_NS);
    return 0;
}