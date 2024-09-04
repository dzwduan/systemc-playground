#include <systemc.h>
#include "initiator.h"
#include "target.h"

class top : public sc_module {
    public :
        sc_signal<char> data;
        sc_signal<bool> rdy;
        sc_signal<bool> ack;

        initiator *initiator_inst;
        target * target_inst;
};