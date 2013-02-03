CFLAGS=-funroll-loops -flto -O3 -lncurses


all: cube

cube: main.o GPIO_Access.o Cube.o Tools.o Timing.o AnimationCubePulse.o	AnimationRunningLight.o
	g++ $(CFLAGS) main.o GPIO_Access.o Cube.o Tools.o Timing.o AnimationCubePulse.o AnimationRunningLight.o -o cube 

main.o: main.cxx
	g++ $(CFLAGS) -c main.cxx

GPIO_Access.o: GPIO_Access.cxx GPIO_Access.h
	g++ $(CFLAGS) -c GPIO_Access.cxx

Cube.o: Cube.cxx Cube.h
	g++ $(CFLAGS) -c Cube.cxx
	
Tools.o: Tools.cxx Tools.h
	g++ $(CFLAGS) -c Tools.cxx

Timing.o: Timing.cxx Timing.h
	g++ $(CFLAGS) -c Timing.cxx

AnimationCubePulse.o: AnimationCubePulse.cxx AnimationCubePulse.h
	g++ $(CFLAGS) -c AnimationCubePulse.cxx

AnimationRunningLight.o: AnimationRunningLight.cxx AnimationRunningLight.h
	g++ $(CFLAGS) -c AnimationRunningLight.cxx

clean:
	rm -rf *o cube

