all:
	g++ -o3 -c main.cpp
	g++ -o3 main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./sfml-app