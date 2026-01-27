COMPILER = clang++
# COMPILER = g++
NAME = Polyomino
IMAGE = polyomino_testing
WARN = -Wall -Wextra -Wpedantic
OPT = -O3 -march=native -flto
ASM = -S -fverbose-asm -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
ASM_MINI = -S -masm=intel -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
THREAD = -fopenmp
NAME = PolyominoAnalyse
IMAGE = polyomino_analyse_testing

run:main
	./main

asm: main.s $(NAME).cpp

main: main.o $(NAME).o
	$(COMPILER) $(WARN) $(OPT) main.o $(NAME).o -o main

main.o: main.cpp
	$(COMPILER) $(WARN) $(OPT) -c main.cpp -o main.o 

$(NAME).o: $(NAME).cpp $(NAME).hpp
	$(COMPILER) $(WARN) $(OPT) -c $(NAME).cpp -o $(NAME).o

main.s: main.cpp 
	$(COMPILER) $(WARN) $(OPT) $(ASM_MINI) main.cpp -o main.s

clean:
	rm -f main.o $(NAME).o

# docker:
# 	docker desktop start
# building:
# 	docker build -t $(IMAGE) .
# running:
# 	docker run --rm -it -v "$(CWD):/app" -w /app $(IMAGE) ash
# removing:
# 	docker image rm $(IMAGE)