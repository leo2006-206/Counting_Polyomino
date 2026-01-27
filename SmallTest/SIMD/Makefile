COMPILER = clang++
# COMPILER   = g++
NAME       = Polyomino

HEADER_PATH	= ../Header/
OBJ_PATH	= $(HEADER_PATH)Object/
DEBUG_PATH	= $(HEADER_PATH)Debug_Object/
ASM_PATH	= $(HEADER_PATH)Assembly/

TARGET_HEADER = 
TARGET_MAIN   = main

# ---- flags ----
HEADER      = -I$(HEADER_PATH)
WARN        = -Wall -Wextra -Wpedantic
OPT_NON_LTO = -O3 -march=native
OPT         = $(OPT_NON_LTO) -flto=auto
STD			= -std=c++20
THREAD 		= -fopenmp

# pls use clang++ for debug
ASM_MINI    = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
ASM         = $(ASM_MINI) -fverbose-asm

SAN   		= -fsanitize=undefined,address -fno-omit-frame-pointer
DEBUG 		= -O0 -g $(SAN) -march=native

FLAGS_SPEED	= $(STD) $(WARN) $(HEADER) $(OPT)
FLAGS_DEBUG = $(STD) $(WARN) $(HEADER) $(DEBUG)
FLAGS_ASM   = $(STD) $(WARN) $(HEADER) $(OPT_NON_LTO) $(ASM_MINI)

# ---- objects ----
OBJ_SPEED 	= $(TARGET_HEADER:%=$(OBJ_PATH)%.o)			$(TARGET_MAIN).o
OBJ_DEBUG   = $(TARGET_HEADER:%=$(DEBUG_PATH)%.debug.o)	$(TARGET_MAIN).debug.o
ASSEMBLY    = $(TARGET_HEADER:%=$(ASM_PATH)%.s)			$(TARGET_MAIN).s

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

$(OBJ_PATH)%.o: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_SPEED) -c $< -o $@

# debug .debug.o
$(DEBUG_PATH)%.debug.o: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_DEBUG) -c $< -o $@

# main.s (release asm)
main.s: main.cpp
	$(COMPILER) $(FLAGS_ASM) $< -o $@

# generic asm for other sources if you want
$(ASM_PATH)%.s: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_ASM) $< -o $@


main_dis.s: main
	objdump -d -M intel main > main_dis.s

clean:
	rm -f *.o *.debug.o main main_dis.s *.s $(OBJ_SPEED) $(OBJ_DEBUG) $(ASSEMBLY)
