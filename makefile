SHELL := /bin/bash
CXX := /usr/bin/clang++

all: game

game: outputs/main.o outputs/screen.o outputs/objects.o outputs/logic.o
	$(CXX) outputs/main.o outputs/screen.o outputs/objects.o -o outputs/game

outputs/main.o: src/main.cpp outputs/objects.o outputs/screen.o outputs/logic.o
	$(CXX) -c src/main.cpp -o outputs/main.o

outputs/screen.o: src/screen.cpp src/screen.h
	$(CXX) -c src/screen.cpp -o outputs/screen.o

outputs/objects.o: src/objects.cpp src/objects.h
	$(CXX) -c src/objects.cpp -o outputs/objects.o

outputs/logic.o: src/logic.cpp src/logic.h
	$(CXX) -c src/logic.cpp -o outputs/logic.o

clean:
	rm -f outputs/*.o
	rm -f outputs/game