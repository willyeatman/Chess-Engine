SHELL := /bin/bash
CXX := /usr/bin/clang++
FLAGS := -Wall -Wextra -Wpedantic

all: game

game: outputs/main.o outputs/screen.o outputs/objects.o
	$(CXX) outputs/main.o outputs/screen.o outputs/objects.o -o outputs/game $(FLAGS)

outputs/main.o: src/main.cpp outputs/objects.o outputs/screen.o
	$(CXX) -c src/main.cpp -o outputs/main.o $(FLAGS)

outputs/screen.o: src/screen.cpp src/screen.h
	$(CXX) -c src/screen.cpp -o outputs/screen.o $(FLAGS)

outputs/objects.o: src/objects.cpp src/objects.h
	$(CXX) -c src/objects.cpp -o outputs/objects.o $(FLAGS)

clean:
	rm -f outputs/*.o
	rm -f outputs/game