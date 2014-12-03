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

using namespace std;

enum nameCountry { CZ, SK, PL, HU, UA };
float nextSeason = 90.0;
int RO = 1;

typedef struct STerminal{
//	int zasoba = 0;
//	int maxVydej = 0;
	long mnozstvi = 0;
	int dst;
}sTerminal;

typedef struct STransport{
	int src;
	int dst;
	long mnozstvi;
}sTransport;

typedef struct Stat{
	//jednotky 100 m^3
	string jmeno;
	long spotreba[2];
	long produkce = 0;
	long zasoba = 0;
	long kapacita = 0;
}sCountry;



void insertStat( sCountry staty[],int position, string jmeno, long sportrebaLeto, long sportrebaZima, long kapacita, long zasoba, long produkce){

	sCountry pom;
	pom.jmeno = jmeno;
	pom.spotreba[0] = sportrebaLeto;
	pom.spotreba[1] = sportrebaZima;
	pom.zasoba = zasoba;
	pom.produkce = produkce;
	pom.kapacita = kapacita;

	staty[position] = pom;
}

void insertTransport(sTransport transport[], int position, int src, int dst, long mnozstvi){

	sTransport pom;
	pom.src = src;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;

	transport[position] = pom;
}

void insertTerminal(sTerminal terminaly[],int position, int dst, long mnozstvi){

	sTerminal pom;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;

	terminaly[position] = pom;
}

bool transportuj(sCountry staty[], sTransport * transport){
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

bool importFromTerminal(sCountry staty[], sTerminal * terminal){
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
	cout << s->spotreba[RO] <<endl;
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
	if(timeHours/24.0 == nextSeason) {
		printf("Summer comes\n");
		nextSeason += 182;
		getchar();
		revertSeason();
	}
}

int main() {

	sCountry staty[5];
	sTransport transporty[2];
	sTerminal terminaly[8];

    int position = 0;
    int positiontransportort = 0;
    int positionTerminal = 0;

    int b;
    scanf("%d",&b);
    if(b == 1) {
    	//
		////  Rusko
		insertTerminal(	terminaly, positionTerminal++, 	CZ, 	967);
		insertTerminal(	terminaly, positionTerminal++, 	SK, 	629);
		insertTerminal(	terminaly, positionTerminal++, 	PL, 	1097);
		insertTerminal(	terminaly, positionTerminal++, 	HU, 	933);
		insertTerminal(	terminaly, positionTerminal++, 	UA, 	2949);
    }


//	INSERT 			terminaly, position, 			dst, 	mnozstvi
//  NEMECKO
    insertTerminal(	terminaly, positionTerminal++, 	PL,		259);
//  Kazachstan
    insertTerminal(	terminaly, positionTerminal++, 	UA,		62);
//  Norsko
    insertTerminal(	terminaly, positionTerminal++, 	CZ,		1);


//	INSERT 			transporty, position, 				src, 	dst,  	mnozstvi
    insertTransport(transporty, positiontransportort++, HU,		UA,		126);
	insertTransport(transporty, positiontransportort++, CZ,		PL,		67);


//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(staty,position++, "Cz", 	270, 		600, 	3436000, 2000000, 29);			//0
	insertStat(staty,position++, "Sl", 	172, 		352, 	3020000, 1500000, 14);			//1
	insertStat(staty,position++, "Pl", 	688, 		974, 	2225000, 1000000, 708);		//2
	insertStat(staty,position++, "Hu",	287, 		673, 	6330000, 2000000, 223);			//3
	insertStat(staty,position++, "Ua", 	1637, 		4323, 	3195000, 1000000, 2288);		//4



	int i = 0;
	while(i < 365*24){

		cout<< "---------------" <<endl;
		printTime(i);
		sCountry a;

		//tady vlozit z terminalu nebo ze zdroju

		for (int l = 0; l < positionTerminal; l++) {
			if(!importFromTerminal(staty, &terminaly[l])){
				cout << "Full zasobnik" <<endl;
			}
		}


		for (int j = 0; j < position; j++) {

			if(!spotrebuj(&staty[j])){
				cout << "Zasoba byla spotrebovana" <<endl;
				//odstranit stat ze struktury statu
			}
			cout << "stat:" << staty[j].jmeno << " stav:" << staty[j].zasoba << endl;
			if(staty[j].zasoba == 0) {
				staty[j].zasoba = 100000000; 
				getchar();
			}

		}


		for (int k = 0; k < positiontransportort; k++) {
			if(!transportuj(staty, &transporty[k])){
				cout << "Zasoba byla spotrebovana nebo plny zasobnik" <<endl;
			}
		}



		++i;

	}
	return 0;
}
