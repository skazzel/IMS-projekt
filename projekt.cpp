#include "simlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

Histogram custom("curtomer", 0, 1000, 20);

class Generator : public Event {
	void Behavior() {
		
  	}
};

int main(int argc, char const *argv[])
{
	SetOutput("data.dat");
	_Print("# IMS - DOL projekt\n");

	return 0;
}