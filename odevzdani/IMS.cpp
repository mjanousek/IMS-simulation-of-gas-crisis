//
// Author      : code & analysis: xjanou14 Martin Janousek, documentation & analysis: xfiala46 Marek Fiala
// Version     : 1.5
// Description : IMS projekt - plynova krize
//============================================================================

#include <iostream>
#include <list>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

//TE - terminal
enum nameCountry {
	CZ, SK, PL, HU, UA, TE
};
float nextSeason = 90.0;
//rocni obdobi
int RO = 1;
//procenta Rusko
float PR = 1;

int timeTable[5][5] = { { 0, 4, 6, 7, 16 }, { 4, 0, 2, 8, 14 },
		{ 6, 2, 0, 8, 13 }, { 7, 8, 8, 0, 10 }, { 16, 14, 13, 10, 0 } };

typedef struct STerminal {
//	int zasoba = 0;
//	int maxVydej = 0;
	long mnozstvi = 0;
	int time = 0;
	int dst;
} sTerminal;

typedef struct STransport {
	int src;
	int dst;
	int time;
	long mnozstvi;
} sTransport;

typedef struct STransakce {

	int dst;
	long mnozstvi;
	int time;
} sTransakce;

typedef struct Stat {
	//jednotky 100 m^3
	string jmeno;
	long spotreba[2];
	long produkce = 0;
	long sExport = 0;
	long zasoba = 0;
	long kapacita = 0;
} sCountry;

typedef struct SStatistika {
	//jednotky 100 m^3
	long pocZasoba = 0;
	int dniNazivu = 0;
	bool nazivu = true;
	long celkImport = 0;
	long celkSpotreba = 0;
	long celkProdukce = 0;
} sStatistika;

//vektor transakci
vector<sTransakce> transakce;

//zeme
sCountry staty[5];
//transporty
sTransport transporty[6];
//terminaly
sTerminal terminaly[11];

int position = 0;
int positiontransportort = 0;
int positionTerminal = 0;

sStatistika statistika[5];

void printHelp() {
	printf("Nápověda:\n"
			"\n"
			"pro spusteni zadejte parametr test=n\n"
			"kde n znaci typ testu (n je v rozmezi 1 - 5)\n"
			""
			"priklad spusteni:\n"
			"./IMS test=2\n\n");
}

void printStat() {
	printf("==========================\n"
			"======  Statistika  ======\n");
	for (int x = 0; x < 5; ++x) {
		printf("--------------------------\n");
		cout << "stat: " << staty[x].jmeno << endl;

		cout << "celkova spotreba:  " << statistika[x].celkSpotreba << endl;
		cout << "celkovy   import:  " << statistika[x].celkImport << endl;
		cout << "celkova produkce:  " << statistika[x].celkProdukce << endl;

		cout << "pocatecni zasoba:  " << statistika[x].pocZasoba << endl;
//			cout << "konecna   zasoba: "<<staty[x].zasoba <<endl;
		cout << "konecna   zasoba:  " << staty[x].zasoba << endl;

		long r = -statistika[x].pocZasoba + staty[x].zasoba;
		if (r < 0) {
			cout << "zasoba-rozdil   : " << r << endl;
		} else {
			cout << "zasoba-rozdil   : +" << r << endl;
		}
		if (statistika[x].nazivu) {

		} else {
			cout << "zasoby vycerpany po " << statistika[x].dniNazivu / 24 / 365
					<< " letech, " << (statistika[x].dniNazivu / 24) % 365
					<< " dnech, " << statistika[x].dniNazivu % 24 << " hodinach"
					<< endl;
		}

	}
	printf("==========================\n");
}

void insertStat(int position, string jmeno, long sportrebaLeto,
		long sportrebaZima, long kapacita, long zasoba, long produkce,
		long sExport) {

	sCountry pom;
	pom.jmeno = jmeno;
	pom.spotreba[0] = sportrebaLeto;
	pom.spotreba[1] = sportrebaZima;
	pom.zasoba = zasoba;
	pom.produkce = produkce;
	pom.sExport = sExport;
	pom.kapacita = kapacita;

	statistika[position].pocZasoba = zasoba;
	staty[position] = pom;
}

void insertTransport(int position, int src, int dst, long mnozstvi, int time) {

	sTransport pom;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;
	pom.time = time;
	pom.src = src;

	transporty[position] = pom;
}

void insertTerminal(int position, int dst, long mnozstvi, int time) {

	sTerminal pom;
	pom.dst = dst;
	pom.time = time;
	pom.mnozstvi = mnozstvi;

	terminaly[position] = pom;
}

void transakceTerminal(sTerminal * terminal) {
	sTransakce pom;
	pom.dst = terminal->dst;
	pom.mnozstvi = terminal->mnozstvi;
	//pridat delku k transakci
	pom.time = terminal->time;
	transakce.push_back(pom);
}

bool transakceTransport(sTransport * transport) {
	if (staty[transport->src].zasoba > transport->mnozstvi) {

		staty[transport->src].zasoba -= transport->mnozstvi;
		sTransakce pom;
		pom.dst = transport->dst;
		pom.mnozstvi = transport->mnozstvi;
		//pridat delku k transakci
		pom.time = transport->time;
		transakce.push_back(pom);
		return true;
	} else {
//		cout<<"transakceTransport2"<<endl;
		if (statistika[transport->src].nazivu) {
			cout << staty[transport->src].jmeno
					<< ":Nelze provest transport, v zasobniku je malo plynu."
					<< endl;
		}
		return false;
	}
}

bool transakceProved(sTransakce transakce) {
	if ((staty[transakce.dst].kapacita
			>= (staty[transakce.dst].zasoba + transakce.mnozstvi))) {

		staty[transakce.dst].zasoba += transakce.mnozstvi;
		statistika[transakce.dst].celkImport += transakce.mnozstvi;
		return true;
	} else {
		int min = ((transakce.dst != 0) ? 1 : 0);
		for (int j = 0; j < position; j++) {

			if ((staty[j].zasoba < staty[min].zasoba) && (transakce.dst != j))
				min = j;

		}



		transakce.time = timeTable[min][transakce.dst];
		transakce.dst = min;

		return false;
	}
}

void transakceCheck() {
	for (unsigned int i = 0; i < transakce.size(); ++i) {
		if (transakce.at(i).time == 0) {
			sTransakce t = transakce.at(i);
			transakceProved(t);
			transakce.erase(transakce.begin() + i);
		} else {

			transakce.at(i).time -= 1;
		}
	}
}
bool importFromTerminal(sTerminal * terminal) {
	if ((staty[terminal->dst].kapacita
			>= (staty[terminal->dst].zasoba + terminal->mnozstvi))) {
		cout << "import from terminal mnozstvi:" << terminal->mnozstvi << " do:"
				<< terminal->dst << endl;
		staty[terminal->dst].zasoba += terminal->mnozstvi;
		return true;
	} else {
		return false;
	}
}

bool spotrebuj(sCountry * s, int p) {
	//produkce
	s->zasoba += s->produkce;
	statistika[p].celkProdukce += s->produkce;

	//spotreba
	if (s->zasoba > s->spotreba[RO]) {
		s->zasoba -= s->spotreba[RO];
		statistika[p].celkSpotreba += s->produkce;
		return true;
	} else {
		s->zasoba = 0;
		return false;
	}
}

bool exportuj(sCountry * s) {

	if (s->zasoba > s->sExport) {
		s->zasoba -= s->sExport;
		return true;
	} else {
		s->zasoba = 0;
		return false;
	}
}

void revertSeason() {
	RO = (RO + 1) % 2;
}

void printTime(int timeHours) {
	printf("Day: %d Hours: %d\n", timeHours / 24, timeHours % 24);
	if (timeHours / 24.0 >= nextSeason) {
		printf("Summer comes\n");
		nextSeason += 182;
		revertSeason();
		if (RO == 0)
			cout << "::::::::LETO:::::::" << endl;
		else
			cout << "::::::::ZIMA:::::::" << endl;
	}
}

/************************************************************************************
 *
 * 								Testy
 *
 ************************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////
//								Test 2												//
//////////////////////////////////////////////////////////////////////////////////////
void testOdpojeniRuska() {

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259, 9);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 42);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 17);

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

//		INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Cesko republika", 553, 1382, 3436000, 3436000, 29,
			0);			//0
	insertStat(position++, "Slovensko", 386, 943, 3020000, 3020000, 14, 0);	//1
	insertStat(position++, "Polsko", 1638, 2524, 2225000, 2225000, 708, 10);//2
	insertStat(position++, "Madarsko", 578, 1527, 6330000, 6330000, 223, 39);//3
	insertStat(position++, "Ukrajina", 3229, 8535, 31950000, 31950000, 2288, 0);

}

//////////////////////////////////////////////////////////////////////////////////////
//								Test 5												//
//////////////////////////////////////////////////////////////////////////////////////

void testZasobnikyDoBudoucna() {

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259, 9);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 42);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 17);

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

//		INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Ceska republika", 553, 1382, 8255000, 8255000, 29,
			0);			//0
	insertStat(position++, "Slovensko", 386, 943, 5500000, 5331000, 14, 0);	//1
	insertStat(position++, "Polsko", 1638, 2524, 8680000, 8680000, 708, 10);//2
	insertStat(position++, "Madarsko", 578, 1527, 8085500, 8085500, 223, 39);//3
	insertStat(position++, "Ukrajina", 3229, 8535, 26340000, 26340000, 2288, 0);

}
//////////////////////////////////////////////////////////////////////////////////////
//								Test 1												//
//////////////////////////////////////////////////////////////////////////////////////
void testSoucasnyStav() {

//	//
//	////  Rusko: pos, dest, time
	insertTerminal(positionTerminal++, CZ, 967, 78);
	insertTerminal(positionTerminal++, SK, 629, 74);
	insertTerminal(positionTerminal++, PL, 1097, 47);
	insertTerminal(positionTerminal++, HU, 933, 67);
	insertTerminal(positionTerminal++, UA, 2949, 51);

	//celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259, 9);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 42);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 17);

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

//		INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Ceska republika", 553, 1382, 3436000, 3436000, 29,
			0);			//0
	insertStat(position++, "Slovensko", 386, 943, 3020000, 3020000, 14, 0);	//1
	insertStat(position++, "Polsko", 1638, 2524, 2225000, 2225000, 708, 10);//2
	insertStat(position++, "Madarsko", 578, 1527, 6330000, 6330000, 223, 39);//3
	insertStat(position++, "Ukrajina", 3229, 8535, 31950000, 31950000, 2288, 0);

}

//////////////////////////////////////////////////////////////////////////////////////
//								Test 3												//
//////////////////////////////////////////////////////////////////////////////////////

void testPripojeniUSA() {

	PR = 0.60;
	//celkem rusko posila: 6834, takze musime z usa dovezt: cca 2800

	//
	////  Rusko: pos, dest, time
	insertTerminal(positionTerminal++, CZ, 967 * PR, 78);
	insertTerminal(positionTerminal++, SK, 629 * PR, 74);
	insertTerminal(positionTerminal++, PL, 1097 * PR, 47);
	insertTerminal(positionTerminal++, HU, 933 * PR, 67);
	insertTerminal(positionTerminal++, UA, 2949 * PR, 51);

	//celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259 * PR, 9);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 42);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 17);

	//pridat terminaly/pristavy
//  chorvatsko
	insertTerminal(positionTerminal++, HU, 970, 6);
//  Polsko
	insertTerminal(positionTerminal++, PL, 800, 7);
//  Ukrajina
	insertTerminal(positionTerminal++, UA, 984, 6); //SK

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

	//pridat transporty mezi zememi
	insertTransport(positiontransportort++, PL, CZ, 250, 7);
	insertTransport(positiontransportort++, HU, SK, 393, 2);
	insertTransport(positiontransportort++, SK, CZ, 140, 4);
	insertTransport(positiontransportort++, HU, UA, 200, 13);

//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Ceska republika", 553, 1382, 3436000, 3436000, 29,
			0);			//0
	insertStat(position++, "Slovensko", 386, 943, 3020000, 3020000, 14, 0);	//1
	insertStat(position++, "Polsko", 1638, 2524, 2225000, 2225000, 708, 10);//2
	insertStat(position++, "Madarsko", 578, 1527, 6330000, 6330000, 223, 39);//3
	insertStat(position++, "Ukrajina", 3229, 8535, 31950000, 31950000, 2288, 0);//4

}

//////////////////////////////////////////////////////////////////////////////////////
//								Test 4												//
//////////////////////////////////////////////////////////////////////////////////////
void testPripojeniUSALeto() {

	PR = 0.80;
	//celkem rusko posila: 6834, takze musime z usa dovezt: cca 2800

	//
	////  Rusko: pos, dest, time
	insertTerminal(positionTerminal++, CZ, 967 * PR, 78);
	insertTerminal(positionTerminal++, SK, 629 * PR, 74);
	insertTerminal(positionTerminal++, PL, 1097 * PR, 47);
	insertTerminal(positionTerminal++, HU, 933 * PR, 67);
	insertTerminal(positionTerminal++, UA, 2949 * PR, 51);

	//celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259 * PR, 9);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 42);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 17);

	//pridat terminaly/pristavy
	//  chorvatsko
	insertTerminal(positionTerminal++, HU, 970, 6);
	//  Polsko
	insertTerminal(positionTerminal++, PL, 800, 7);
	//  Ukrajina
	insertTerminal(positionTerminal++, UA, 984, 6); //SK

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

	//pridat transporty mezi zememi
	insertTransport(positiontransportort++, PL, CZ, 250, 7);
	insertTransport(positiontransportort++, HU, SK, 393, 2);
	insertTransport(positiontransportort++, SK, CZ, 140, 4);
	insertTransport(positiontransportort++, HU, UA, 200, 13);

//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Ceska republika", 553, 1382, 6006000, 3436000, 29,
			0);			//0
	insertStat(position++, "Slensko", 386, 943, 3020000, 3020000, 14, 0);	//1
	insertStat(position++, "Polsko", 1638, 2524, 4445000, 2225000, 708, 10);//2
	insertStat(position++, "Madarsko", 578, 1527, 6330000, 6330000, 223, 39);//3
	insertStat(position++, "Ukrajina", 3229, 8535, 31950000, 31950000, 2288, 0);//4

}

int main(int argc, char * argv[]) {

	if (argc != 2) {
		cerr
				<< "Zadali jste spatny pocet prametru, pouzijte --help pro napovedu."
				<< endl;
		return 1;
	}

	//nastaveni testu
	int b = 0;

	if (strcmp(argv[1], "--help") == 0) {
		printHelp();
		return 0;
	} else if (strcmp(argv[1], "test=1") == 0) {
		cout << "Test1" << endl;
		b = 1;
	} else if (strcmp(argv[1], "test=2") == 0) {
		cout << "Test2" << endl;
		b = 2;
	} else if (strcmp(argv[1], "test=3") == 0) {
		cout << "Test3" << endl;
		b = 3;
	} else if (strcmp(argv[1], "test=4") == 0) {
		cout << "Test4" << endl;
		b = 4;
	} else if (strcmp(argv[1], "test=5") == 0) {
		cout << "Test5" << endl;
		b = 5;
	} else {
		cerr
				<< "Zadali jste spatny pocet prametru, pouzijte --help pro napovedu."
				<< endl;
		return 1;
	}

//    scanf("%d",&b);
	if (b == 1) {
		testSoucasnyStav();
	}
	if (b == 2) {
		testOdpojeniRuska();
	}
	if (b == 3) {
		testPripojeniUSA();
	}
	if (b == 4) {
		testPripojeniUSALeto();
	}
	if (b == 5) {
		testZasobnikyDoBudoucna();
	}
//    testZasobnikyDoBudoucna()

	int i = 0;
	bool con = true;
	while ((i < 5 * 365 * 24) && con) {
		con = false;
		if ((i % (24 * 30)) == 0) {
			cout << "---------------" << endl;
			printTime(i);
		}
		sCountry a;

		for (int l = 0; l < positionTerminal; l++) {
			if ((l >= 8) && (RO == 1) && (b == 4)) {
				continue;
			}
			transakceTerminal(&terminaly[l]);
		}

		for (int j = 0; j < position; j++) {
			if (!spotrebuj(&staty[j], j) || !exportuj(&staty[j])) {
				if (statistika[j].nazivu) {
					cout << "Zasoba byla spotrebovana u " << staty[j].jmeno
							<< endl;
				}
			}
//			if((i % (24)) == 0){
//				cout << "stat:" << staty[j].jmeno << " stav:" << staty[j].zasoba << endl;
//			}
			if ((staty[j].zasoba == 0) && statistika[j].nazivu) {
				staty[j].zasoba = 0;
				statistika[j].nazivu = false;
				statistika[j].dniNazivu = i;
			}
			if (statistika[j].nazivu)
				con = true;

		}

//		cout<<"transporty"<<endl;
		for (int k = 0; k < positiontransportort; k++) {
//			if(!transportuj(&transporty[k])){
//				cout << "Zasoba byla spotrebovana nebo plny zasobnik" <<endl;
//			}
			if ((k >= 2) && (RO == 1) && (b == 4)) {
				continue;
			}
			transakceTransport(&transporty[k]);
		}
//		cout<<"transakceCheck"<<endl;
		transakceCheck();

		++i;

	}

	printStat();
	return 0;
}
