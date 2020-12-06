projekt: projekt.o
	g++ projekt.o -o projekt -lm -lsimlib

projekt.o: projekt.cpp
	g++ -c projekt.cpp

clean:
	rm *.o projekt
