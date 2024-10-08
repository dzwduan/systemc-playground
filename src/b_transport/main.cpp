#include "b_transport.h"

int sc_main(int argc, char *argv[]) {
  MyTop top("top");
  sc_start(20, SC_NS);
  return 0;
}