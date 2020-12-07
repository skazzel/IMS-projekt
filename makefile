main: main.o
	g++ main.o -o main -lm -lsimlib

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main
