#include "simlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

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

int profit = 0;
int simulation_time = 0;
Histogram pozadavky_lode("Pozadavky lode", 0, 1000, 20);
int pocet_lodi = 40;
int STREDNI_LOZENI_PLAVIDLA = 3088; 
int ZBOZI_TRIDA_cena = 0;
const int MINUTE = 60;
const int HOUR = 3600;
const int DAY = 86400;
const int WEEK = 604800;
Store Plavebni_komora("Plavebni_komora", 1);

void print_help() {
	_Print("pomoc is here\n");
}

int get_time() {
	return (int)(Time)%(DAY);
}

class lod: public Process {
	double prichod;
	void Behavior() {
		prichod = Time;
		int time_of_day = get_time();
		if (!Plavebni_komora.Full()) {
			Enter(Plavebni_komora);
			double random_cislo = Random();
			_Print("random %d\n", random_cislo);
			if (random_cislo < 0.1) {
				Wait(Exponential(10*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.81;
			}
			else if (random_cislo < 0.2 && random_cislo > 0.1) {
				Wait(Exponential(10*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.81;
			}
			else if (random_cislo < 0.35 && random_cislo > 0.2) {
				Wait(Exponential(8*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.68;
			}
			else if (random_cislo < 0.5 && random_cislo > 0.35) {
				Wait(Exponential(7*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.66;
			}
			else if (random_cislo < 0.7 && random_cislo > 0.5) {
				Wait(Exponential(5*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.57;
			}
			else if (random_cislo < 1.0 && random_cislo > 0.7) {
				Wait(Exponential(4*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = 0.40;
			}
			profit = STREDNI_LOZENI_PLAVIDLA * ZBOZI_TRIDA_cena;
			_Print("Profit %d\n", profit);
			Leave(Plavebni_komora);
			pozadavky_lode(Time - prichod);
		}
	}
};

void generator::Behavior() {
	(new lod)->Activate();

	//Activate(Time + Exponential(1));
}

int main(int argc, char const *argv[])
{

	SetOutput("data.dat");
	_Print("# IMS - DOL projekt\n");
	simulation_time = WEEK;

	for (int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0){
			print_help();
			exit(EXIT_SUCCESS);
		}
		if (strcmp(argv[i], "-n") == 0) {
			pocet_lodi = atol(argv[i+1]);
			_Print("Pocet lodi %d\n", pocet_lodi);
		}
		if (strcmp(argv[i], "-t") == 0) {
			try {
				simulation_time = std::stoi(argv[i+1]);
			}
			catch (std::exception const &e) {
				_Print("neco se nepovedlo");
				exit(EXIT_FAILURE);
			}
		}
	}
	_Print("%d", simulation_time);
	RandomSeed(time(NULL));
	Init(0, simulation_time);
	(new generator)->Activate();
	Run();

	return 0;
}