.PHONY: clean build run

build:
	g++ -o sim controller.cpp simulator.cpp sim.cpp

run: build
	./sim

clean:
	rm -f sim