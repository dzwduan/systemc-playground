SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))
# MODULE_NAME := $(notdir $(shell pwd))
prog := emu

# !!! 替换成自己的systemc路径
SYSTEMC_HOME := /home/dzw/systemc


INCLUDES := -I${SYSTEMC_HOME}/include -I.

CXXFLAGS := ${INCLUDES}

LDFLAGS := -L${SYSTEMC_HOME}/lib-linux64 


LIBS :=    -lsystemc \
	   -lfmt \
	   -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 

run: $(prog)
	@./emu

$(prog): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $(OBJ) -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm *.o $(prog)
