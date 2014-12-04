//============================================================================
// Name        : IMS.cpp
// Author      : xjanou14
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


/*
cesko 	import 	export	produkce 	spotreba 	zasobniky 	maximalniOdber
cesko 	968 	1 		29 			968 		3436000
polsko 	1424 	11 		708 		2081 		2225000
slov 	637 	0 		14 			664 		3020000
mad 	933 	165 	223 		1053 		6330000
ukr 	3139 	0 		2288 		5882 		3195000

*/

/*



*/

#include <iostream>
#include <list>
#include <iterator>
#include <stdio.h>
#include <vector>

using namespace std;

//TE - terminal
enum nameCountry { CZ, SK, PL, HU, UA, TE };
float nextSeason = 90.0;
//rocni obdobi
int RO = 1;
//procenta Rusko
float PR = 1;

typedef struct STerminal{
//	int zasoba = 0;
//	int maxVydej = 0;
	long mnozstvi = 0;
	int time = 0;
	int dst;
}sTerminal;

typedef struct STransport{
	int src;
	int dst;
	int time;
	long mnozstvi;
}sTransport;

typedef struct STransakce{

	int dst;
	long mnozstvi;
	int time;
}sTransakce;

typedef struct Stat{
	//jednotky 100 m^3
	string jmeno;
	long spotreba[2];
	long produkce = 0;
	long zasoba = 0;
	long kapacita = 0;
}sCountry;

//vektor transakci
vector<sTransakce> transakce;

//zeme
sCountry 	staty[5];
//transporty
sTransport 	transporty[2];
//terminaly
sTerminal 	terminaly[8];

int position = 0;
int positiontransportort = 0;
int positionTerminal = 0;

void insertStat(int position, string jmeno, long sportrebaLeto, long sportrebaZima, long kapacita, long zasoba, long produkce){

	sCountry pom;
	pom.jmeno = jmeno;
	pom.spotreba[0] = sportrebaLeto;
	pom.spotreba[1] = sportrebaZima;
	pom.zasoba = zasoba;
	pom.produkce = produkce;
	pom.kapacita = kapacita;

	staty[position] = pom;
}

void insertTransport(int position, int src, int dst, long mnozstvi, int time){

	sTransport pom;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;
	pom.time = time;

	transporty[position] = pom;
}

void insertTerminal(int position, int dst, long mnozstvi, int time){

	sTerminal pom;
	pom.dst = dst;
	pom.time = time;
	pom.mnozstvi = mnozstvi;

	terminaly[position] = pom;
}

bool transportuj(sTransport * transport){
//	TODO zohlednit transport do jinych zemi ktere nemapujeme! dat podminku
//	ze se nebude nic ukladat jen se odebere
//	TODO zkontrolovat podminku
	if((staty[transport->src].zasoba > transport->mnozstvi) && (staty[transport->dst].kapacita >= (staty[transport->dst].zasoba + transport->mnozstvi))){
		cout<<"Spotrebovavam"<<endl;
		staty[transport->src].zasoba -= transport->mnozstvi;
		staty[transport->dst].zasoba += transport->mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
//	TODO tady to ma byt jinak, zde je moynost i ze byl pouze plny zasobnik takze se musi vracet neco jineho
//		nez ze to dopadlo spatne
		return false;
	}
}

void transakceTerminal(sTerminal * terminal){
	sTransakce pom;
	pom.dst = terminal->dst;
	pom.mnozstvi = terminal->mnozstvi;
	//pridat delku k transakci
	pom.time = terminal->time;
	transakce.push_back(pom);
}

bool transakceTransport(sTransport * transport){
	if(staty[transport->src].zasoba > transport->mnozstvi){
		sTransakce pom;
		pom.dst = transport->dst;
		pom.mnozstvi = transport->mnozstvi;
		//pridat delku k transakci
		pom.time = transport->time;
		transakce.push_back(pom);
		return true;
	}else{
		cout << staty[transport->src].jmeno << ":Nelze provest transport, v zasobniku je malo plynu.";
		return false;
	}
}

bool transakceProved(sTransakce transakce){
	if((staty[transakce.dst].kapacita >= (staty[transakce.dst].zasoba + transakce.mnozstvi))){
		cout<<"transakce:" << transakce.mnozstvi << " do:" << transakce.dst <<endl;
		staty[transakce.dst].zasoba += transakce.mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
		return false;
	}
}


void transakceCheck(){
	for (unsigned int i = 0; i < transakce.size(); ++i) {
		if(transakce.at(i).time == 0){
			sTransakce t = transakce.at(i);
			transakceProved(t);
			transakce.erase(transakce.begin()+i);
		}else{
//			cout<< transakce.at(i).time<< ", " ;
			transakce.at(i).time-=1;
		}
	}
}
bool importFromTerminal(sTerminal * terminal){
	if((staty[terminal->dst].kapacita >= (staty[terminal->dst].zasoba + terminal->mnozstvi))){
		cout<<"import from terminal mnozstvi:" << terminal->mnozstvi << " do:" << terminal->dst <<endl;
		staty[terminal->dst].zasoba += terminal->mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
		return false;
	}
}

bool spotrebuj(sCountry * s){
	//produkce
	s->zasoba += s->produkce;

	//spotreba
//	cout << s->spotreba[RO] <<endl;
	if(s->zasoba > s->spotreba[RO]){
		s->zasoba -= s->spotreba[RO];
		return true;
	}else{
		s->zasoba = 0;
		return false;
	}
}

void revertSeason() {
	RO = (RO+1)%2;
}

void printTime(int timeHours) {
	printf("Day: %d Hours: %d\n", timeHours/24, timeHours%24);
	if(timeHours/24.0 >= nextSeason) {
		printf("Summer comes\n");
		nextSeason += 182;
//	    int b;
//	    scanf("%d",&b);
		revertSeason();
	}
}

/************************************************************************************
 *
 * 								Testy
 *
 ************************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////
//								Test 1												//
//////////////////////////////////////////////////////////////////////////////////////

void testOdpojeniRuska(){

//	sCountry staty[5];
//	sTransport transporty[2];
//	sTerminal terminaly[8];




//	//
//	////  Rusko: pos, dest, time
//	insertTerminal(positionTerminal++, 	CZ, 	967*PR, 70);
//	insertTerminal(positionTerminal++, 	SK, 	629*PR, 66);
//	insertTerminal(positionTerminal++, 	PL, 	1097*PR, 42);
//	insertTerminal(positionTerminal++, 	HU, 	933*PR, 60);
//	insertTerminal(positionTerminal++, 	UA, 	2949*PR, 46);

    //celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
    insertTerminal(positionTerminal++, 	PL,		259 * PR, 7);
//  Kazachstan
    insertTerminal(positionTerminal++, 	UA,		62, 37);
//  Norsko
    insertTerminal(positionTerminal++, 	CZ,		1, 15);


//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
    insertTransport(positiontransportort++, HU,		UA,		126, 	11);
	insertTransport(positiontransportort++, CZ,		PL,		67, 	7);


	//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
		insertStat(position++, "Cz", 	533, 		1186, 	3436000, 3436000, 29);			//0
		insertStat(position++, "Sl", 	340, 		696, 	3020000, 3020000, 14);			//1
		insertStat(position++, "Pl", 	1361, 		1927, 	2225000, 2225000, 708);			//2
		insertStat(position++, "Hu",	568, 		1331, 	6330000, 6330000, 223);			//3
		insertStat(position++, "Ua", 	3238, 		8551, 	31950000, 31950000, 2288);		//4

}

//////////////////////////////////////////////////////////////////////////////////////
//								Test 1												//
//////////////////////////////////////////////////////////////////////////////////////

void testPripojeniUSA(){

	PR = 0.7;

	//
	////  Rusko: pos, dest, time
	insertTerminal(positionTerminal++, 	CZ, 	967*PR, 70);
	insertTerminal(positionTerminal++, 	SK, 	629*PR, 66);
	insertTerminal(positionTerminal++, 	PL, 	1097*PR, 42);
	insertTerminal(positionTerminal++, 	HU, 	933*PR, 60);
	insertTerminal(positionTerminal++, 	UA, 	2949*PR, 46);


	//celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
	insertTerminal(positionTerminal++, PL, 259 * PR, 7);
//  Kazachstan
	insertTerminal(positionTerminal++, UA, 62, 37);
//  Norsko
	insertTerminal(positionTerminal++, CZ, 1, 15);

//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 11);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);

//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Cz", 533, 1186, 3436000, 3436000, 29);			//0
	insertStat(position++, "Sl", 340, 696, 3020000, 3020000, 14);			//1
	insertStat(position++, "Pl", 1361, 1927, 2225000, 2225000, 708);		//2
	insertStat(position++, "Hu", 568, 1331, 6330000, 6330000, 223);			//3
	insertStat(position++, "Ua", 3238, 8551, 31950000, 31950000, 2288);		//4

}


int main() {

	int b = 2;
//    scanf("%d",&b);
    if(b == 1) {
    	testOdpojeniRuska();
    }
    if(b == 2) {
    	testPripojeniUSA();
	}



	int i = 0;
	while(i < 10*365*24){

		cout<< "---------------" <<endl;
		printTime(i);
		sCountry a;

		//tady vlozit z terminalu nebo ze zdroju

		for (int l = 0; l < positionTerminal; l++) {
//			if(!importFromTerminal(&terminaly[l])){
//				cout << "Full zasobnik" <<endl;
//			}
			transakceTerminal(&terminaly[l]);
		}


		for (int j = 0; j < position; j++) {

			if(!spotrebuj(&staty[j])){
				cout << "Zasoba byla spotrebovana" <<endl;
				//odstranit stat ze struktury statu
			}
			cout << "stat:" << staty[j].jmeno << " stav:" << staty[j].zasoba << endl;
			if(staty[j].zasoba == 0) {
				staty[j].zasoba = 0;
			    int b;
			    scanf("%d",&b);
			}

		}


		for (int k = 0; k < positiontransportort; k++) {
//			if(!transportuj(&transporty[k])){
//				cout << "Zasoba byla spotrebovana nebo plny zasobnik" <<endl;
//			}
			transakceTransport(&transporty[k]);

		}


		transakceCheck();


		++i;

	}
	return 0;
}
