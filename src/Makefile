SRC = $(wildcard *.cpp)

OBJ = $(patsubst %.cpp, %.o, $(SRC))

SYSTEMC_HOME := /home/dzw/systemc


INCLUDES := -I${SYSTEMC_HOME}/include -I.

CXXFLAGS := ${INCLUDES}

LDFLAGS := -L${SYSTEMC_HOME}/lib-linux64 


LIBS :=    -lsystemc \
	   -lfmt \
	   -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 


MODULE_NAME := $(notdir $(shell pwd))

prog: $(OBJ)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $(OBJ) -o $(MODULE_NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o *.vcd src

all: prog
.PHONY: all