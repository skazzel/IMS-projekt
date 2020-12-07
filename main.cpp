#include <simlib.h>
#include <string>
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>

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

long long int profit = 0;
std::vector<long long int> soucet_profit;
int simulation_time = 0;
int pocet_lodi = 5000;
int STREDNI_LOZENI_PLAVIDLA = 3088; 
float ZBOZI_TRIDA_cena = 0;
const int MINUTE = 60;
const int HOUR = 3600;
const int DAY = 86400;
const int WEEK = 604800;
int YEAR_SECOND = DAY*365;
//koridor
enum Corridor_type {
    DOL,
    DO,
    DL,
    LO
};
Corridor_type corridor = DOL;
//cas v rocich
int year = 1;
//celkove naklady
double costs;
int pocet_dnu = 1;
//pocet komor
int chamber_number = 30;

Store Plavebni_komora("Plavebni_komora", 1);

void print_help() {
	_Print("Spusteni: \n");
    _Print("./main [PREPINAC] <HODNOTA> \n");
    _Print("Prepinace: \n");
    _Print("-c, --corridor      prida moznost si vybrat koridor DOL, DO, DL nebo LO\n");
    _Print("-h, --help          vypise napovedu\n");
    _Print("-n, --number        znaci pocet lodi ktere propluji danym koridorem za dany cas\n");
    _Print("-y, --year          pocet let, pro ktery se pocitat\n");
    _Print("--------------------------------------------------------------------------------\n");
    _Print("Bez specifikovani hodnot jsou hodnoty nastavene na -c DOL, -n 5000 a -y 1\n");
}

void get_cost(){
    //osobni naklady
    double personal_costs;
    //provozni naklady
    double operating_costs;
    switch (corridor){
    case DOL:
        //178 400 000 Kč
        personal_costs = 178400000;
        //364 300 000 Kč
        operating_costs = 364300000;
        break;
    case DO:
        //122 400 000 Kč
        personal_costs = 122400000;
        //227 300 000 Kč
        operating_costs = 227300000;
        break;
    case DL:
        //113 000 000 Kč
        personal_costs = 113000000;
        //250 600 000 Kč
        operating_costs = 250600000;
        break;
    case LO:
        //121 500 000 Kč
        personal_costs = 121500000;
        //2506 600 000 Kč
        operating_costs = 250600000;
        break;
    default:
        break;
    }
    costs = (personal_costs + operating_costs) * year;
}

void get_chamber() {
    switch (corridor){
    case DOL:
        chamber_number = 30;
        break;
    case DO:
        chamber_number = 19;
        break;
    case DL:
        chamber_number = 19;
        break;
    case LO:
        chamber_number = 19;
        break;
    default:
        break;
    }
}

class Lod: public Process {
public:
	void Behavior() {
		if (!Plavebni_komora.Full()) {
			Wait(Exponential(20*MINUTE));
			Enter(Plavebni_komora, 1);
			double random_cislo = Random();
			if (random_cislo < 0.1) {
				Wait(Exponential(10*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_1;
			}
			else if (random_cislo < 0.2 && random_cislo > 0.1) {
				Wait(Exponential(10*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_2;
			}
			else if (random_cislo < 0.35 && random_cislo > 0.2) {
				Wait(Exponential(8*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_3;
			}
			else if (random_cislo < 0.5 && random_cislo > 0.35) {
				Wait(Exponential(7*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_4;
			}
			else if (random_cislo < 0.7 && random_cislo > 0.5) {
				Wait(Exponential(5*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_5;
			}
			else if (random_cislo < 1.0 && random_cislo > 0.7) {
				Wait(Exponential(4*MINUTE)); // cekame nez lod proplave
				ZBOZI_TRIDA_cena = ZBOZI_TRIDA_6;
			}
			if (Time > (86400*pocet_dnu)) {
				pocet_dnu++;
			}
			if (Time >= 0) {
				double cas = Time / (86400 * pocet_dnu);
				cas *= 24;
				if (cas >= 6 && cas <= 19) {
					soucet_profit.push_back(STREDNI_LOZENI_PLAVIDLA * ZBOZI_TRIDA_cena * chamber_number);
				}
				else if (cas > 19 && cas <= 24) {
					soucet_profit.push_back(chamber_number * STREDNI_LOZENI_PLAVIDLA * ZBOZI_TRIDA_cena + 25);
				}
				else if (cas < 6) {
					soucet_profit.push_back(chamber_number * STREDNI_LOZENI_PLAVIDLA * ZBOZI_TRIDA_cena + 60);
				}
			}
			Leave(Plavebni_komora, 1);
		}
	}
};

void generator::Behavior() {
	for (int i = 0; i < pocet_lodi; i++) {
		(new Lod)->Activate();
	}
}

int main(int argc, char const *argv[])
{
	SetOutput("data.dat");
	_Print("# IMS - DOL projekt\n");
	simulation_time = YEAR_SECOND;

	for (int i = 1; i < argc; i++) {
        //vypis napovedy
		if((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)){
			print_help();
			exit(EXIT_SUCCESS);
		}
        //pocet lodi
		if(strcmp(argv[i], "-n") == 0) {
            i++;
            if(argc == i){
                std::cerr << "Nebyl zadan pocet lodi" << std::endl;
                exit(EXIT_FAILURE);
            }else{
                try {
                    pocet_lodi = std::stoi(argv[i]);
                }
                catch (std::exception const &e) {
                    std::cerr << "Pocet lodi musi byt cele cislo" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
		}
		//vyber koridoru
		if((strcmp(argv[i], "--corridor") == 0) || (strcmp(argv[i], "-c") == 0)){
            i++;
            if (argc == i) {
                std::cerr << "Nebyl zadan koridor" << std::endl;
                exit(EXIT_FAILURE);
            }
            else {
                if(strcmp(argv[i], "DOL") == 0){
                    corridor = DOL;
                }else if(strcmp(argv[i], "DO") == 0){
                    corridor = DO;
                }else if(strcmp(argv[i], "DL") == 0){
                    corridor = DL;
                }else if(strcmp(argv[i], "LO") == 0){
                    corridor = LO;
                }else{
                    std::cerr << "Spatne zadany koridor" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
		}
        //vyber poctu let
        if((strcmp(argv[i], "--year") == 0) || (strcmp(argv[i], "-y") == 0)){
            i++;
            if (argc == i) {
                std::cerr << "Nebyl zadan rok" << std::endl;
                exit(EXIT_FAILURE);
            }
            else {
                try {
				    year = std::stoi(argv[i]);
				    simulation_time = YEAR_SECOND * year;
                }
                catch (std::exception const &e) {
                    std::cerr << "Rok musi byt cele cislo" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
		}
	}
	get_chamber();
    get_cost();
	RandomSeed(time(NULL));
	Init(0, simulation_time);

	(new generator)->Activate();
	Run();

	for (long long int x : soucet_profit) {
		profit += x;
	}
	_Print("Celkove naklady: %.0lf Kc\n", costs);
	_Print("Vynosy z lodni prepravy: %lld Kc\n", profit);
	if ((profit-costs) > 0) {
		_Print("Celkovy vydelek: %.2lf Kc\n", profit - costs);
		_Print("Celkovy vydelek v mil: %.2lf Mil. Kc\n", (profit - costs)/1000000);
	}
	else {
		_Print("Celkova ztrata: %.2lf Kc\n", profit - costs);
		_Print("Celkova ztrata v mil: %.2lf Mil. Kc\n", (profit - costs)/1000000);
	}

	return 0;
}