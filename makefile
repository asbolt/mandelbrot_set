OPT ?= -O3
OBJ = ./build/main.o ./build/mandelbrot.o ./build/fps.o
SFML = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
FLAGS = -I./include -mavx2 -c

all: $(OBJ)
	g++ $(OPT) $(OBJ) -o sfml-app $(SFML)

./build/main.o: main.cpp
	g++ $(OPT) $(FLAGS) main.cpp -o ./build/main.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./build/mandelbrot.o: ./source/mandelbrot.cpp
	g++ $(OPT) $(FLAGS) ./source/mandelbrot.cpp -o ./build/mandelbrot.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./build/fps.o: ./source/fps.cpp
	g++ $(OPT) $(FLAGS) ./source/fps.cpp -o ./build/fps.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
