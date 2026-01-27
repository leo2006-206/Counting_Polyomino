#COMPILER = clang++
COMPILER   = g++
NAME       = Polyomino

HEADER_PATH	= ../Header/
PATH_OBJ	= $(HEADER_PATH)Obj_Speed/
PATH_DEBUG	= $(HEADER_PATH)Obj_Debug/
PATH_RECORD = $(HEADER_PATH)Obj_Record/
PATH_ASM	= $(HEADER_PATH)Assembly/


TARGET_HEADER = Poly_Int_Pair_v1 Poly_Int_Pair_v2 Poly_Int_Pair_v3
TARGET_MAIN   = main

# ---- flags ----
HEADER      = -I$(HEADER_PATH)
WARN        = -std=c++20 -Wall -Wextra -Wpedantic
OPT_NON_LTO = -O3 -march=native
OPT         = $(OPT_NON_LTO) -flto=auto
THREAD 		= -fopenmp

# pls use clang++ for debug
ASM_MINI    = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
ASM         = $(ASM_MINI) -fverbose-asm

SAN   		= -fsanitize=undefined -fno-omit-frame-pointer
DEBUG 		= -O0 -g $(SAN)

RECORD_OPT  = $(OPT_NON_LTO) -g -fno-omit-frame-pointer

FLAGS_SPEED	= $(WARN) $(OPT)       $(HEADER)
FLAGS_DEBUG = $(WARN) $(DEBUG)     $(HEADER)
FLAGS_ASM   = $(WARN) $(OPT_NON_LTO) $(HEADER) $(ASM_MINI)
FLAGS_RECORD= $(WARN) $(RECORD_OPT) $(HEADER)

# ---- objects ----
OBJ_SPEED 	= $(TARGET_HEADER:%=$(PATH_OBJ)%.o)			$(TARGET_MAIN).o
OBJ_DEBUG   = $(TARGET_HEADER:%=$(PATH_DEBUG)%.debug.o)	$(TARGET_MAIN).debug.o
OBJ_RECORD  = $(TARGET_HEADER:%=$(PATH_RECORD)%.record.o)	$(TARGET_MAIN).record.o
ASSEMBLY    = $(TARGET_HEADER:%=$(PATH_ASM)%.s)			$(TARGET_MAIN).s

# ---- default ----
.PHONY: run debug clean asm

run: main
	./main

debug: debug_main
	./debug_main

asm: main_dis.s $(ASSEMBLY)

stat: main
	perf stat -d ./main

record: record_main
	perf record ./record_main
	perf report


# ---- link ----
main: $(OBJ_SPEED)
	$(COMPILER) $(FLAGS_SPEED) $^ -o $@

debug_main: $(OBJ_DEBUG)
	$(COMPILER) $(FLAGS_DEBUG) $^ -o $@

record_main: $(OBJ_RECORD)
	$(COMPILER) $(FLAGS_RECORD) $^ -o $@

# ---- compile rules ----
# release .o

main.o: main.cpp
	$(COMPILER) $(FLAGS_SPEED) -c $< -o $@

main.debug.o: main.cpp
	$(COMPILER) $(FLAGS_DEBUG) -c $< -o $@

main.record.o: main.cpp
	$(COMPILER) $(FLAGS_RECORD) -c $< -o $@

$(PATH_OBJ)%.o: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_SPEED) -c $< -o $@

# debug .debug.o
$(PATH_DEBUG)%.debug.o: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_DEBUG) -c $< -o $@

$(PATH_RECORD)%.record.o: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_RECORD) -c $< -o $@

# main.s (release asm)
main.s: main.cpp
	$(COMPILER) $(FLAGS_ASM) $< -o $@

# generic asm for other sources if you want
$(PATH_ASM)%.s: $(HEADER_PATH)%.cpp $(HEADER_PATH)%.hpp
	$(COMPILER) $(FLAGS_ASM) $< -o $@


main_dis.s: main
	objdump -d -M intel main > main_dis.s

clean:
	rm -f *.o *.debug.o main main_dis.s *.s $(OBJ_SPEED) $(OBJ_DEBUG) $(ASSEMBLY)
