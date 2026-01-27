COMPILER = g++
CODE = cpp
HEADER = hpp
CWD = C:\Users\jwtt6\LeoWong\SCC_Major\Algorithms_DataStructures\Algorithm\Counting_Polyomino
NAME = Polyomino
IMAGE = polyomino_testing

# run:main
# 	./main

# main: main.o $(NAME).o
# 	$(COMPILER) -Wall -Wextra -Wpedantic -g -O3 main.o $(NAME).o -o main

# main.o: main.$(CODE)
# 	$(COMPILER)  -Wall -Wextra -Wpedantic -g -O3 -c main.$(CODE) -o main.o 

# $(NAME).o: $(NAME).$(CODE) $(NAME).$(HEADER)
# 	$(COMPILER)  -Wall -Wextra -Wpedantic -g -O3 -c $(NAME).$(CODE) -o $(NAME).o 

# clean:
# 	rm -f main.o $(NAME).o

docker:
	docker desktop start
building:
	docker build -t $(IMAGE) .
running:
	docker run --rm -it \
	--security-opt seccomp=unconfined \
	--cap-add SYS_ADMIN \
  	--cap-add SYS_PTRACE \
	--privileged \
	-v "$(CWD):/app" -w /app $(IMAGE) ash
removing:
	docker image rm $(IMAGE)