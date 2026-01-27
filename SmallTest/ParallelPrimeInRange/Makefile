COMPILER = g++
CODE = cpp
HEADER = hpp
# NAME = Polyomino
# IMAGE = polyomino_testing
OPT = -std=c++20 -O3 -march=native
# OPT = -O3 -march=native
CWD = C:\Users\jwtt6\LeoWong\SCC_Major\Algorithms_DataStructures\Algorithm\Counting_Polyomino\SmallTest\ParallelPrimeInRange

assembly:
	$(COMPILER) -Wall -Wextra -Wpedantic -g $(OPT) -S -fopenmp main.cpp -o main.s

run:main
	./main

main: main.o
	$(COMPILER) -Wall -Wextra -Wpedantic -g $(OPT) -fopenmp main.o -o main

main.o: main.$(CODE)
	$(COMPILER)  -Wall -Wextra -Wpedantic -g $(OPT) -fopenmp -c main.$(CODE) -o main.o 

# $(NAME).o: $(NAME).$(CODE) $(NAME).$(HEADER)
# 	$(COMPILER)  -Wall -Wextra -Wpedantic -g $(OPT) -fopenmp -c $(NAME).$(CODE) -o $(NAME).o 

clean:
	rm .\*.o .\*.exe

# docker:
# 	docker desktop start
# building:
# 	docker build -t $(IMAGE) .
# running:
# 	docker run --rm -it -v "$(CWD):/app" -w /app $(IMAGE) ash
# removing:
# 	docker image rm $(IMAGE)