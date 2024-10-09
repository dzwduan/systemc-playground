
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext.h"
#include "systemc"

using namespace sc_core;

SC_MODULE(STAGE){

    SC_CTOR(STAGE){// elaboration
                   std::cout << sc_time_stamp() << " : Elaboration: constructor"
                             << std::endl;
SC_THREAD(thread); // init + simulation
}

~STAGE() {
  std::cout << sc_time_stamp() << " : Cleanup : destructor" << std::endl;
}

void thread() {
  std::cout << sc_time_stamp() << " : Execution.init" << std::endl;
  int i = 0;

  while (true) {
    wait(1, SC_SEC); // advance time
    std::cout << sc_time_stamp() << " : Execution.simulation" << std::endl;
    if (++i >= 2)
      sc_stop();
  }
}

void before_end_of_elaboration() override {
  std::cout << "before end of elaboration" << std::endl;
}
void end_of_elaboration() override {
  std::cout << "end of elaboration" << std::endl;
}
void start_of_simulation() override {
  std::cout << "start of simulation" << std::endl;
}
void end_of_simulation() override {
  std::cout << "end of simulation" << std::endl;
}
}
;

int sc_main(int argc, char *argv[]) {

  STAGE stage("stage"); // Elaboration
  sc_start();           // Execution  till sc_stop
  return 0;
}
