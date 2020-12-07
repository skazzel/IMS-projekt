#include <simlib.h>
#include <string>
#include <ctime>
#include <cstring>
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

double profit = 0;
int simulation_time = 0;
Histogram pozadavky_lode("Pozadavky lode", 0, 1000, 20);
int pocet_lodi = 40;
int STREDNI_LOZENI_PLAVIDLA = 3088; 
float ZBOZI_TRIDA_cena = 0;
const int MINUTE = 60;
const int HOUR = 3600;
const int DAY = 86400;
const int WEEK = 604800;
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

Store Plavebni_komora("Plavebni_komora", 1);

void print_help() {
	_Print("Spusteni: \n");
    _Print("./projekt [PREPINAC] <HODNOTA> \n");
    _Print("Prepinace: \n");
    _Print("-c, --corridor      prida moznost si vybrat koridor DOL, DO, DL nebo LO\n");
    _Print("-h, --help          vypise napovedu\n");
    _Print("-n, --number        znaci pocet lodi ktere propluji danym koridorem za dany cas\n");
    _Print("-y, --year          pocet let, pro ktery se pocitat\n");
    _Print("--------------------------------------------------------------------------------\n");
    _Print("Bez specifikovani hodnot jsou hodnoty nastavene na -c DOL, -n 40 a -y 1\n");
}

int get_time() {
	return (int)(Time)%(DAY);
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

class Lod: public Process {
	int pracovni_doba[3];
public:
	Lod(int doba1, int doba2, int doba3) : Process() {
		pracovni_doba[0] = doba1;
		pracovni_doba[1] = doba2;
		pracovni_doba[2] = doba3;
	}

	double prichod;

	void Behavior() {
		prichod = Time;
		int time_of_day = get_time();
		//Wait(10);
		//_Print("TIME %f\n", Time);
		if (!Plavebni_komora.Full()) {
			Wait(Exponential(20*MINUTE));
			Enter(Plavebni_komora, 1);
			double random_cislo = Random();
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
			double random_procento = Random();
			if (random_procento <= 0.33) {
				
			}
			_Print("Profit %lf\n", profit);
			//_Print("CENA %f\n", ZBOZI_TRIDA_cena);
			//_Print("plavebni komora11 %d\n", Plavebni_komora.Full());
			_Print("TIME %f\n", Time);
			Leave(Plavebni_komora, 1);
			//pozadavky_lode(Time - prichod);
		}
	}
};

void generator::Behavior() {
	_Print("Pocet lodi %d\n", pocet_lodi);
	for (int i = 0; i < pocet_lodi; i++) {
		Lod* lod = (new Lod(6, 16, 19));
		lod->Activate();
	}
	/*int tod = get_time();
	_Print("CAS %d\n", tod);
	double timeToNextRequest = 0;
		if(tod < (12*HOUR)){
			timeToNextRequest = 2057;
		}else if(tod < (18*HOUR)){
			timeToNextRequest = 280;
		}else if(tod < (22*HOUR)){
			timeToNextRequest = 305;
		}else{
			timeToNextRequest = 500;
		}
		Activate(Time+Normal(timeToNextRequest, 0.1));*/
}

int main(int argc, char const *argv[])
{
	SetOutput("data.dat");
	_Print("# IMS - DOL projekt\n");
	simulation_time = WEEK;

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
		if(strcmp(argv[i], "-t") == 0) {
			try {
				simulation_time = std::stoi(argv[i+1]);
			}
			catch (std::exception const &e) {
				_Print("neco se nepovedlo");
				exit(EXIT_FAILURE);
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
                }
                catch (std::exception const &e) {
                    std::cerr << "Rok musi byt cele cislo" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
		}
	}
    get_cost();
    _Print("Celkove naklady: %.0lf Kc\n", costs);
	_Print("%d \n", simulation_time);
	RandomSeed(time(NULL));
	Init(0, simulation_time);

	(new generator)->Activate();
	Run();

	return 0;
}