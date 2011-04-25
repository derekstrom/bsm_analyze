CCC      = g++

# Subsystems that have compilable libraries
SUBMOD   = bsm_input
LIB		 = libbsm_analyze.so.1.0

# Get list of all heads, sources and objects. Each source (%.cc) whould have
# an object file
HEADS    = $(wildcard ./interface/*.h)
SRCS     = $(wildcard ./src/*.cc)

OBJS     = $(foreach obj,$(addprefix ./obj/,$(patsubst %.cc,%.o,$(notdir $(SRCS)))),$(obj))

# List of programs with main functions to be filtered out of objects
PROGS    = $(patsubst ./src/%.cpp,%,$(wildcard ./src/*.cpp))

# Special rules to clean each subsystem
CLEAN_MOD = $(addprefix CLN_,$(SUBMOD))

# Flags used in compilation
ifeq ($(strip $(DEBUG)),)
	DEBUG = -O2
else
	DEBUG = -O0 -g
endif

CXXFLAGS = ${DEBUG} -fPIC -pipe -Wall -I./ -I/opt/local/include -I${ROOTSYS}/include -I${BOOST_ROOT}/include  -I./input/interface $(foreach subsys,$(SUBMOD),$(addprefix -I./,$(subsys)))
LIBS     = -L/opt/local/lib -lprotobuf -L${BOOST_ROOT}/lib -L./lib $(foreach mod,$(SUBMOD),$(addprefix -l,$(mod)))
LDFLAGS  = -shared -W1 `root-config --libs` -L/opt/local/lib -lprotobuf -L${BOOST_ROOT}/lib -L./lib $(foreach mod,$(SUBMOD),$(addprefix -l,$(mod)))

# Rules to be always executed: empty ones
.PHONY: $(SUBMOD) all

all: $(SUBMOD) obj lib prog

help:
	@echo "make <rule>"
	@echo
	@echo "Rules"
	@echo "-----"
	@echo
	@echo "  obj        compile objects"
	@echo "  lib        compile shared library"
	@echo

obj: $(OBJS)

lib: $(LIB)

prog: $(PROGS)



# Compile modules
$(SUBMOD):
	$(MAKE) -C $@ 
	ln -fs ../$@/lib/lib$@.so ./lib/

# Object files depend on all sources and headers but only sources should be
# compiled
$(OBJS): $(addprefix ./src/,$(patsubst %.o,%.cc,$(notdir $@))) 
	@echo "[+] Compiling objects ..."
	$(CCC) $(CXXFLAGS) -c $(addprefix ./src/,$(patsubst %.o,%.cc,$(notdir $@))) -o $@
	@echo

$(LIB): $(OBJS)
	@echo "[+] Creating shared libraries ..."
	$(CCC) $(LDFLAGS) -o $(addprefix ./lib/,$@) $(OBJS)
	@cd ./lib; ln -fs $@ $(basename $@); ln -fs $(basename $@) $(basename $(basename $@))
	@echo



# Executables
#
$(PROGS): $(OBJS) 
	@echo "[+] Compiling programs ..."
	$(CCC) $(CXXFLAGS) `root-config --glibs` $(LIBS) $(OBJS) $(PROTOCOBJS) ./src/$@.cpp -o ./bin/$@
	@echo

# This rule will clean libraries also code depend on. Run:
#     make cleanall
# from top folder for global clean of all systems
cleanbin:
ifneq ($(strip $(PROGS)),)
	rm -f $(addprefix ./bin/,$(PROGS))
endif

clean: cleanbin
	rm -f ./obj/*.o
	rm -f $(addprefix ./lib/,$(basename $(basename $(LIB)))*)

$(CLEAN_MOD):
	$(MAKE) -C $(subst CLN_,,$@) clean
	rm -f ./lib/lib$(subst CLN_,,$@).so

cleanall: clean $(CLEAN_MOD)
