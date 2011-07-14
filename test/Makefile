CCC      = g++

SUBMOD   = bsm_input bsm_analyze bsm_stat bsm_core JetMETObjects

# List of programs with main functions to be filtered out of objects
PROGS    = $(patsubst ./src/%.cpp,%,$(wildcard ./src/*.cpp))

# Flags used in compilation
ifeq ($(strip $(DEBUG)),)
	DEBUG = -O2
else
	DEBUG = -O0 -g
endif

CXXFLAGS = ${DEBUG} -fPIC -pipe -Wall -DSTANDALONE -I../ -I/opt/local/include -I${ROOTSYS}/include -I${BOOST_ROOT}/include -I./bsm_input/message 

ifeq ($(shell uname),Linux)
	LIBS = -L/opt/local/lib -lprotobuf -L${BOOST_ROOT}/lib -L../lib $(foreach mod,$(SUBMOD),$(addprefix -l,$(mod))) -lboost_thread -lboost_regex -lboost_filesystem
else
	LIBS = -L/opt/local/lib -lprotobuf -L${BOOST_ROOT}/lib -L../lib $(foreach mod,$(SUBMOD),$(addprefix -l,$(mod))) -lboost_thread-mt -lboost_regex -lboost_filesystem
endif

# Rules to be always executed: empty ones
.PHONY: all

all: prog

help:
	@echo "make <rule>"
	@echo
	@echo "Rules"
	@echo "-----"
	@echo
	@echo "  prog	compile executables"
	@echo

prog: $(PROGS)

# Executables
#
$(PROGS): $(addprefix ./src/,$(addsuffix .cpp,$(notdir $@)))
	@echo "[+] Compiling programs ..."
	$(CCC) $(CXXFLAGS) `root-config --glibs` $(LIBS) ./src/$@.cpp -o ./bin/test_$@
	@echo

clean:
ifneq ($(strip $(PROGS)),)
	rm -f $(addprefix ./bin/test_,$(PROGS))
endif
