CXX = clang++

INCLUDES = $(shell libpng-config --cflags)
INCLUDES += $(shell libpng-config --ldflags)

CXXFLAGS = -std=c++14 $(INCLUDES)

defaut: all

all: bin mandelbrot mandelbrot_debug

bin:
	mkdir bin

mandelbrot: src/mandelbrot.cpp bin
	$(CXX) $< $(CXXFLAGS) -O2 -o bin/mandelbrot

mandelbrot_debug: src/mandelbrot.cpp bin
	$(CXX) $< $(CXXFLAGS) -O0 -g -Wall -o bin/mandelbrot_debug

clean:
	rm -rf bin
