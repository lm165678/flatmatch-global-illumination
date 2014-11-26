
SRC_C = main.c png_helper.c rectangle.c vector3_cl.c photonmap.c geoSphere.c
SRC_CC = parseLayout.cc global_illumination_cl.cc 
SRC = $(SRC_C) $(SRC_CC)

OBJ_C  = $(patsubst %.c,build/c_%.o,$(SRC_C))
OBJ_CC = $(patsubst %.cc,build/cc_%.o,$(SRC_CC))
OBJ    = $(OBJ_C) $(OBJ_CC)

BC_C  = $(patsubst %.c,build/c_%.bc,$(SRC_C))
BC_CC = $(patsubst %.cc,build/cc_%.bc,$(SRC_CC))
BC    = $(BC_C) $(BC_CC)


CC = gcc#clang
CPP= g++#clang++

EMCC=emcc
EMPP=em++
EMCC_FLAGS=-I./include -std=c99
EMPP_FLAGS=-I./include -std=c++11

OPT_FLAGS = -O2
OSX_INCLUDES = #-I /usr/local/include -framework OpenCL
OSX_LIBS = #-L /usr/local/lib -framework OpenCL
FLAGS = -g -Wall -Wextra -msse3 $(OPT_FLAGS)
PROFILE = 
#PROFILE = -fprofile-generate
#PROFILE = -fprofile-use

CFLAGS = $(FLAGS) $(PROFILE) -std=c99 -flto #$(OSX_INCLUDES)
CCFLAGS = $(FLAGS) $(PROFILE) -std=c++11 -flto #$(OSX_INCLUDES)
LD_FLAGS = $(PROFILE) $(OSX_LIBS) -lOpenCL -lm  $(OPT_FLAGS) -flto
.PHONY: all clean

all: make.dep globalIllumination tiles
#	 @echo [ALL] $<

globalIllumination: $(OBJ) build
	@echo [LD] $@
	@$(CPP) $(OBJ)  $(LD_FLAGS) -lpng -o $@

index.js: $(BC)
	@echo [LD] $@
	@$(EMCC) -Os $(BC) lib/*.bc \
    --embed-file 137.png \
    -s EXPORTED_FUNCTIONS='["_main","_getJsonFromLayout", "_getJsonFromLayoutMem", "_performGlobalIlluminationNative","_parseLayoutMem", "_createGeometryObject"]'\
    -s TOTAL_MEMORY=33554432\
    -s ASSERTIONS=1 \
    -s NO_EXIT_RUNTIME=1 \
    -o $@

#    -s ALLOW_MEMORY_GROWTH=1 \
	
build: 
	@echo [MKDIR] $@
	@echo mkdir -p $@

build/c_%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) $< -c -o $@

build/c_%.bc: %.c
	@echo [EMCC] $<
	@$(EMCC) $(EMCC_FLAGS) -O2 $< -o $@

build/cc_%.o: %.cc 
	@echo [CP] $<
	@$(CPP) $(CCFLAGS) $< -c -o $@

build/cc_%.bc: %.cc 
	@echo [EM++] $<
	@$(EMPP) $(EMPP_FLAGS) -O2 $< -o $@


tiles:
	@echo [MKDIR] tiles
	@mkdir tiles
    

clean:
	@echo [CLEAN]
	@rm -rf *.o *.oo
	@rm -rf build/*
	@rm -rf *~
	@rm -rf globalIllumination
	@rm -rf coverage.info callgrind.out.*
	@rm -f index.js.mem index.js

make.dep: $(SRC_C) $(SRC_CC)
	@echo [DEP]
	@$(CC)  -MM -I /usr/local/include $(SRC_C) | sed "s/\([[:graph:]]*\)\.o/build\/c_\\1.o/g" > make.dep
	@$(CPP) -MM -I /usr/local/include $(SRC_CC)| sed "s/\([[:graph:]]*\)\.o/build\/cc_\\1.o/g" >> make.dep

include make.dep

