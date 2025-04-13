CC = g++
FLAGS ?= -O3 -mavx2 -I./include
SFML = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

CSRC = mandelbrot.cpp fps.cpp main.cpp 
COBJ = $(CSRC:%.cpp=./build/%.o)

.PHONY: all
all: $(COBJ)
	$(CC) $(FLAGS) $(COBJ) -o mandelbrot $(SFML)

./build/%.o: source/%.cpp
	$(CC) $(FLAGS) -c $^ -o $@ $(SFML)

.PHONY: clean
clean:
	rm -r ./build/*.o
	rm -r mandelbrot
	