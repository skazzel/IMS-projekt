#include "simlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define SIMULATION_DAYS 365
#define PRUPLAV_RYCHLE 10
#define PRUPLAV_POMALU_A 14
#define PRUPLAV_POMALU_B 18
#

#define PROFIT 0
#define ZBOZI_TRIDA_1 0.81 // podil zbozi 10%
#define ZBOZI_TRIDA_2 0.81 // podil zbozi 10%
#define ZBOZI_TRIDA_3 0.68 // podil zbozi 15%
#define ZBOZI_TRIDA_4 0.66 // podil zbozi 15%
#define ZBOZI_TRIDA_5 0.57 // podil zbozi 20%
#define ZBOZI_TRIDA_6 0.40 // podil zbozi 30%
#define STREDNI_LOZENI_PLAVIDLA 3088 // hmostnost v tunach

class generator: public Event {
public:
	void Behavior();
};
class lod: public Process {
public:
	void Behavior();
};

int pocet_lodi = 40;
Store Plavebni_komora("Plavebni_komora", 1);

void print_help() {
	_Print("pomoc is here\n");
}

int get_time() {
	return (int)(Time)%(DAY);
}

void generator::Behavior() {
	(new lod)->Activate();

	int time_of_day = get_time();
	double time_to_next_request = 0;
	if (time_of_day < (12*DAY)) {

	}
}

int main(int argc, char const *argv[])
{

	SetOutput("data.dat");
	_Print("# IMS - DOL projekt\n");

	for (int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0){
			print_help();
			exit(EXIT_SUCCESS);
		}
		if (strcmp(argv[i], "-n") == 0) {
			pocet_lodi = atol(argv[i+1]);
			_Print("Pocet lodi %d\n", pocet_lodi);
		}
	}

	RandomSeed(time(NULL));
	Init(0, WEEK);
	(new generator)->Activate();
	Run();

	return 0;
}