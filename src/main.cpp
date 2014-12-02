//============================================================================
// Name        : IMS.cpp
// Author      : xjanou14
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


/*
cesko 	import 	transportort 	produkce 	spotreba 	zasobniky 	maximalniOdber
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
using namespace std;

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
	long spotreba = 0;
	long produkce = 0;
	long zasoba = 0;
	long kapacita = 0;
}stat;

void insertStat( stat staty[],int position, string jmeno, long sportreba, long kapacita, long zasoba, long produkce){

	stat pom;
	pom.jmeno = jmeno;
	pom.spotreba = sportreba;
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

bool transportuj(stat staty[], sTransport * transport){
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

bool importFromTerminal(stat staty[], sTerminal * terminal){
	if((staty[terminal->dst].kapacita >= (staty[terminal->dst].zasoba + terminal->mnozstvi))){
		cout<<"import from terminal mnozstvi:" << terminal->mnozstvi << " do:" << terminal->dst <<endl;
		staty[terminal->dst].zasoba += terminal->mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
		return false;
	}
}

bool spotrebuj(stat * s){
	//produkce
	s->zasoba += s->produkce;

	//spotreba
	if(s->zasoba > s->spotreba){
		s->zasoba -= s->spotreba;
		return true;
	}else{
		s->zasoba = 0;
		return false;
	}
}


int main() {

	stat staty[5];
	sTransport transporty[2];
	sTerminal terminaly[3];

    int position = 0;
    int positiontransportort = 0;
    int positionTerminal = 0;

//	INSERT 			terminaly, position, 			dst, 	mnozstvi
//  NEMECKO
    insertTerminal(	terminaly, positionTerminal++, 	2, 		259);
//  Kazachstan
    insertTerminal(	terminaly, positionTerminal++, 	4, 		62);
//  Norsko
    insertTerminal(	terminaly, positionTerminal++, 	0, 		1);


//	INSERT 			transporty, position, 				src, 	dst,  	mnozstvi
    insertTransport(transporty, positiontransportort++, 3, 		4, 		1108);
	insertTransport(transporty, positiontransportort++, 0, 		2, 		67);

//	INSERT staty, position, jmeno, 		sportreba,  velZasob, zasoby,  produkce
	insertStat(staty,position++, "Cz", 	968, 	3436000, 3436000, 29);			//0
	insertStat(staty,position++, "Sl", 	664, 	3020000, 3020000, 14);			//1
	insertStat(staty,position++, "Po", 	2081, 	2225000, 02225000, 708);		//2
	insertStat(staty,position++, "Md",	1053, 	6330000, 6330000, 223);			//3
	insertStat(staty,position++, "Ua", 	5882, 	3195000, 3195000, 2288);		//4


	int i = 0;
	while(i < 10){

		cout<< "---------------" <<endl;
		stat a;

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

		}


		for (int k = 0; k < positiontransportort; k++) {
			if(!transportuj(staty, &transporty[k])){
				cout << "Zasoba byla spotrebovana nebo plny zasobnik" <<endl;
			}
		}

		++i;
	}






	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
