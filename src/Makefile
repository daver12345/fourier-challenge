.PHONY: clean build run

build:
	g++ -std=c++11 -o sim controller.cpp simulator.cpp serial.cpp sim.cpp

run: build
	./sim

clean:
	rm -f sim