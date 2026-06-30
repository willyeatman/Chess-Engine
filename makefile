SHELL := /bin/bash
CXX := /usr/bin/clang++
FLAGS := -Wall -Wextra -Wpedantic

all: game debug

game: outputs/main.o outputs/screen.o outputs/objects.o outputs/debug.o
	$(CXX) outputs/main.o outputs/screen.o outputs/objects.o outputs/debug.o -o outputs/game $(FLAGS)

outputs/main.o: src/main.cpp outputs/objects.o outputs/screen.o outputs/debug.o
	$(CXX) -c src/main.cpp -o outputs/main.o $(FLAGS)

outputs/screen.o: src/screen.cpp src/screen.h
	$(CXX) -c src/screen.cpp -o outputs/screen.o $(FLAGS)

outputs/objects.o: src/objects.cpp src/objects.h src/screen.h src/debug.h
	$(CXX) -c src/objects.cpp -o outputs/objects.o $(FLAGS)

outputs/debug.o: src/debug.cpp src/debug.h
	$(CXX) -c src/debug.cpp -o outputs/debug.o $(FLAGS)


debug: outputs/main_debug.o outputs/screen_debug.o outputs/objects_debug.o outputs/debug_debug.o
	$(CXX) outputs/main_debug.o outputs/screen_debug.o outputs/objects_debug.o outputs/debug_debug.o -o outputs/game_debug $(FLAGS) -DDEBUG

outputs/main_debug.o: src/main.cpp outputs/objects.o outputs/screen.o outputs/debug.o
	$(CXX) -c src/main.cpp -o outputs/main_debug.o $(FLAGS) -DDEBUG

outputs/screen_debug.o: src/screen.cpp src/screen.h
	$(CXX) -c src/screen.cpp -o outputs/screen_debug.o $(FLAGS) -DDEBUG

outputs/objects_debug.o: src/objects.cpp src/objects.h src/screen.h src/debug.h
	$(CXX) -c src/objects.cpp -o outputs/objects_debug.o $(FLAGS) -DDEBUG

outputs/debug_debug.o: src/debug.cpp src/debug.h
	$(CXX) -c src/debug.cpp -o outputs/debug_debug.o $(FLAGS) -DDEBUG


clean:
	rm -f outputs/*.o
	rm -f outputs/game