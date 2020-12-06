#include "simlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define SIMULATION_DAYS 365
#define PRUPLAV_RYCHLE 10
#define PRUPLAV_POMALU_A 14
#define PRUPLAV_POMALU_B 18

#define PROFIT 0
#define ZBOZI_TRIDA_1 0.81 // podil zbozi 10%
#define ZBOZI_TRIDA_2 0.81 // podil zbozi 10%
#define ZBOZI_TRIDA_3 0.68 // podil zbozi 15%
#define ZBOZI_TRIDA_4 0.66 // podil zbozi 15%
#define ZBOZI_TRIDA_5 0.57 // podil zbozi 20%
#define ZBOZI_TRIDA_6 0.40 // podil zbozi 30%

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

void lod::Behavior() {
	if (!Plavebni_komora.Full()) {
		Enter(Plavebni_komora);
		double random_cislo = Random();
		_Print("random %d\n", random_cislo);
		if (random_cislo < 0.1) {
			Wait(Uniform(10, 14)); // cekame nez lod proplave
		}
		else if (random_cislo < 0.2 && random_cislo > 0.1) {
			Wait(Uniform(PRUPLAV_RYCHLE, PRUPLAV_POMALU_B)); // cekame nez lod proplave
		}
		else if (random_cislo < 0.35 && random_cislo > 0.2) {
			Wait(Uniform(PRUPLAV_RYCHLE, PRUPLAV_POMALU_B)); // cekame nez lod proplave
		}
		else if (random_cislo < 0.5 && random_cislo > 0.35) {
			Wait(Uniform(PRUPLAV_RYCHLE, PRUPLAV_POMALU_A)); // cekame nez lod proplave
		}
		else if (random_cislo < 0.7 && random_cislo > 0.5) {
			Wait(Uniform(PRUPLAV_RYCHLE, PRUPLAV_POMALU_A)); // cekame nez lod proplave
		}
		else if (random_cislo < 1.0 && random_cislo > 0.7) {
			Wait(Uniform(PRUPLAV_RYCHLE, PRUPLAV_POMALU_A)); // cekame nez lod proplave
		}
		Leave(Plavebni_komora);
	}
	else {
		Wait(15);
	}
}

void generator::Behavior() {
	_Print("generator\n");
	for (int i = 0; i < pocet_lodi; i++) {
		(new lod)->Activate();
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
	Init(0, SIMULATION_DAYS);
	(new generator)->Activate();
	Run();

	return 0;
}