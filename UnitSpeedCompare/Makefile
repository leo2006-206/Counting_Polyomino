#COMPILER = clang++
COMPILER   = g++
NAME       = Polyomino

HEADER_PATH   = ../Header/
TARGET_HEADER = Poly_Int_Pair_v2.cpp Testcase_Int_Pair_v2.cpp
TARGET_MAIN   = main.cpp

SOURCE_HEADER = $(patsubst %, $(HEADER_PATH)%, $(TARGET_HEADER))
SOURCE        = $(SOURCE_HEADER) $(TARGET_MAIN)

# ---- flags ----
HEADER      = -I$(HEADER_PATH)
WARN        = -Wall -Wextra -Wpedantic
OPT_NON_LTO = -O3 -march=native
OPT         = $(OPT_NON_LTO) -flto=auto
THREAD 		= -fopenmp

# pls use clang++ for debug
ASM_MINI    = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
ASM         = $(ASM_MINI) -fverbose-asm

SAN   		= -fsanitize=undefined -fno-omit-frame-pointer
DEBUG 		= -O0 -g $(SAN)

FLAGS_SPEED	= $(WARN) $(OPT)       $(HEADER)
FLAGS_DEBUG = $(WARN) $(DEBUG)     $(HEADER)
FLAGS_ASM   = $(WARN) $(OPT_NON_LTO) $(HEADER) $(ASM_MINI)

# ---- objects ----
OBJ_SPEED = $(SOURCE:.cpp=.o)
OBJ_DEBUG   = $(SOURCE:.cpp=.debug.o)
ASSEMBLY    = $(SOURCE:.cpp=.s)

# ---- default ----
.PHONY: run debug clean asm

run: main
	./main

debug: debug_main
	./debug_main

asm: main_dis.s $(ASSEMBLY)

# ---- link ----
main: $(OBJ_SPEED)
	$(COMPILER) $(FLAGS_SPEED) $^ -o $@

debug_main: $(OBJ_DEBUG)
	$(COMPILER) $(FLAGS_DEBUG) $^ -o $@

# ---- compile rules ----
# release .o

main.o: main.cpp
	$(COMPILER) $(FLAGS_SPEED) -c $< -o $@

main.debug.o: main.cpp
	$(COMPILER) $(FLAGS_DEBUG) -c $< -o $@

%.o: %.cpp %.hpp
	$(COMPILER) $(FLAGS_SPEED) -c $< -o $@

# debug .debug.o
%.debug.o: %.cpp %.hpp
	$(COMPILER) $(FLAGS_DEBUG) -c $< -o $@

# main.s (release asm)
main.s: main.cpp
	$(COMPILER) $(FLAGS_ASM) -c $< -o $@

# generic asm for other sources if you want
%.s: %.cpp %.hpp
	$(COMPILER) $(FLAGS_ASM) -c $< -o $@


main_dis.s: main
	objdump -d -M intel main > main_dis.s

clean:
	rm -f *.o *.debug.o main main_dis.s *.s
