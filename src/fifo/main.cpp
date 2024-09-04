#include "fifo.h"

SC_MODULE(producer) {
    public:
        sc_port<write_if> out;  // port 类似于 bundle

        SC_CTOR(producer) {
            SC_THREAD(p_main);
        }

        void p_main() {
            char c;
            out->reset();
            while (true) {
                out->write(c);
                cout << "Produced: " << c << endl;
                c = c + 1;
                if (c > 'z') c = 'a';
                wait(5, SC_NS);
            }
        }
};


SC_MODULE(consumer) {
    public:
        sc_port<read_if> in;

        SC_CTOR(consumer) {
            SC_THREAD(c_main);
        }

        void c_main() {
            char c;
            while (true) {
                in->read(c);
                cout << "Consumed: " << c << endl;
                wait(5, SC_NS);
            }
        }
};


SC_MODULE(top) {
    public:
        fifo *fifo_inst;
        producer *prod_inst;
        consumer *cons_inst;

        SC_CTOR(top) {
            fifo_inst = new fifo("fifo_inst");
            prod_inst = new producer("prod_inst");
            cons_inst = new consumer("cons_inst");

            prod_inst->out(*fifo_inst);
            cons_inst->in(*fifo_inst);
        }
};


int sc_main (int argc, char *argv[]) {
    top top("Top");
    sc_start();
    return 0;
}