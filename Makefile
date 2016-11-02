all: Main

filing.o:
	g++ filing.cpp -c

clean:
	rm -rf *.o

Main: main.cpp filing.o
	g++ main.cpp filing.o -o main.o
