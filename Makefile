CFLAGS=-funroll-loops -flto -O3


all: cube

cube: main.o GPIO_Access.o Cube.o
	g++ $(CFLAGS) main.o GPIO_Access.o Cube.o -o cube 

main.o: main.cxx
	g++ $(CFLAGS) -c main.cxx

GPIO_Access.o: GPIO_Access.cxx GPIO_Access.h
	g++ $(CFLAGS) -c GPIO_Access.cxx

Cube.o: Cube.cxx Cube.h
	g++ $(CFLAGS) -c Cube.cxx

clean:
	rm -rf *o cube
