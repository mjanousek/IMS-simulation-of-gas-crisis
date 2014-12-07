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
float nextSeason = 1.0;
//rocni obdobi
int RO = 1;
//procenta Rusko
float PR = 1;

int timeTable[5][5] = {
		{0,4,6,7,16},
		{4,0,2,8,14},
		{6,2,0,8,13},
		{7,8,8,0,10},
		{16,14,13,10,0}
};

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
	long sExport = 0;
	long zasoba = 0;
	long kapacita = 0;
}sCountry;

//vektor transakci
vector<sTransakce> transakce;

//zeme
sCountry 	staty[5];
//transporty
sTransport 	transporty[6];
//terminaly
sTerminal 	terminaly[11];

int position = 0;
int positiontransportort = 0;
int positionTerminal = 0;

void insertStat(int position, string jmeno, long sportrebaLeto, long sportrebaZima, long kapacita, long zasoba, long produkce, long sExport){

	sCountry pom;
	pom.jmeno = jmeno;
	pom.spotreba[0] = sportrebaLeto;
	pom.spotreba[1] = sportrebaZima;
	pom.zasoba = zasoba;
	pom.produkce = produkce;
	pom.sExport = sExport;
	pom.kapacita = kapacita;

	staty[position] = pom;
}

void insertTransport(int position, int src, int dst, long mnozstvi, int time){

	sTransport pom;
	pom.src = src;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;
	pom.time = time;
	pom.src = src;

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
<<<<<<< HEAD
	cout<<"transakceTransport"<<endl;
	printf("Zdroj %d\n",transport->src);
	printf("Mnozstvni %ld\n",transport->mnozstvi);
	printf("Zasoba %ld\n",staty[transport->src].zasoba);
=======
>>>>>>> ec81b3663787f92914aae1bc9010740088ced9bf
	if(staty[transport->src].zasoba > transport->mnozstvi){

		staty[transport->src].zasoba -= transport->mnozstvi;
		sTransakce pom;
		pom.dst = transport->dst;
		pom.mnozstvi = transport->mnozstvi;
		//pridat delku k transakci
		pom.time = transport->time;
		transakce.push_back(pom);
		return true;
	}else{
//		cout<<"transakceTransport2"<<endl;
		cout << staty[transport->src].jmeno << ":Nelze provest transport, v zasobniku je malo plynu.";
		return false;
	}
}

bool transakceProved(sTransakce transakce){
	if((staty[transakce.dst].kapacita >= (staty[transakce.dst].zasoba + transakce.mnozstvi))){
//		cout<<"transakce:" << transakce.mnozstvi << " do:" << transakce.dst <<endl;
		staty[transakce.dst].zasoba += transakce.mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
		int min = ((transakce.dst!=0) ? 1 : 0);
		for (int j = 0; j < position; j++) {

			if(staty[j].zasoba < staty[min].zasoba && min != j)
				min = j;

		}

		cout << "Preposilam z: " << transakce.dst << "do :" << min << endl;
//		int b = 2;
//	    scanf("%d",&b);

		transakce.time = timeTable[min][transakce.dst];
		transakce.dst = min;

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

bool exportuj(sCountry * s){

	if(s->zasoba > s->sExport){
		s->zasoba -= s->sExport;
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
	insertTerminal(positionTerminal++, 	CZ, 	967, 78);
	insertTerminal(positionTerminal++, 	SK, 	629, 74);
	insertTerminal(positionTerminal++, 	PL, 	1097, 47);
	insertTerminal(positionTerminal++, 	HU, 	933, 67);
	insertTerminal(positionTerminal++, 	UA, 	2949, 51);

    //celkove

//	INSERT 			terminaly, position, dst, 	mnozstvi, time
//  Rusko pres NEMECKO
    insertTerminal(positionTerminal++, 	PL,		259, 9);
//  Kazachstan
    insertTerminal(positionTerminal++, 	UA,		62, 42);
//  Norsko
    insertTerminal(positionTerminal++, 	CZ,		1, 17);


//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
    insertTransport(positiontransportort++, HU,		UA,		126, 	13);
	insertTransport(positiontransportort++, CZ,		PL,		67, 	7);


//	//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
//		insertStat(position++, "Cz", 968,  968, 3436000, 3436000, 29, 0);			//0
//		insertStat(position++, "Sk", 664,  664, 3020000, 3020000, 14, 0);			//1
//		insertStat(position++, "Pl", 2081, 2081, 2225000, 2225000, 708, 10);		//2
//		insertStat(position++, "Hu", 1053,  1053, 6330000, 6330000, 223, 39);			//3
//		insertStat(position++, "Ua", 5882, 5882, 31950000, 31950000, 2288, 0);		//4
//	for(int a = 0; a < positionTerminal; a++){
//		cout<<"terminal: mnozstvi, time, detination:" << terminaly[a].mnozstvi <<" "<< terminaly[a].time <<" "<< terminaly[a].dst <<" " <<endl;
//	}
//
//	for(int a = 0; a < position; a++){
//		cout<<"stat:" << staty[a].jmeno <<endl;
//		cout<<"spotrebaL:"<< staty[a].spotreba[0] <<"spotrebaZ:"<< staty[a].spotreba[1]<<"produkce:"<< staty[a].produkce <<"kap:"<< staty[a].kapacita<<"zas:"<< staty[a].zasoba<<"produkce:"<< staty[a].produkce<<"exp:"<< staty[a].sExport<<endl;
//	}
//
//	for(int a = 0; a < positiontransportort; a++){
//		cout<<"transport: mnozstvi, time, src, dst:" << terminaly[a].mnozstvi <<" "<< transporty[a].time <<" "<< transporty[a].src <<" "<< transporty[a].dst <<" " <<endl;
//	}

//			INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
			insertStat(position++, "Cz", 553,  1382, 3436000, 3436000, 29, 0);			//0
			insertStat(position++, "Sl", 386,  943, 3020000, 3020000, 14, 0);			//1
			insertStat(position++, "Pl", 1638, 2524, 2225000, 2225000, 708, 10);		//2
			insertStat(position++, "Hu", 578,  1527, 6330000, 6330000, 223, 39);			//3
			insertStat(position++, "Ua", 3229, 8535, 31950000, 31950000, 2288, 0);

}

//////////////////////////////////////////////////////////////////////////////////////
//								Test 1												//
//////////////////////////////////////////////////////////////////////////////////////

void testPripojeniUSA(){

	PR = 0.60;
	//celkem rusko posila: 6834, takze musime z usa dovezt: cca 2800

	//
	////  Rusko: pos, dest, time
	insertTerminal(positionTerminal++, 	CZ, 	967*PR, 78);
	insertTerminal(positionTerminal++, 	SK, 	629*PR, 74);
	insertTerminal(positionTerminal++, 	PL, 	1097*PR, 47);
	insertTerminal(positionTerminal++, 	HU, 	933*PR, 67);
	insertTerminal(positionTerminal++, 	UA, 	2949*PR, 51);


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
	insertTerminal(positionTerminal++, HU, 1000, 6);
//  Polsko
	insertTerminal(positionTerminal++, PL, 800, 7);
//  Ukrajina
	insertTerminal(positionTerminal++, UA, 1000, 6); //SK


//	INSERT 			transporty, position, 	src, 	dst,  	mnozstvi, doba
	insertTransport(positiontransportort++, HU, UA, 126, 13);
	insertTransport(positiontransportort++, CZ, PL, 67, 7);


	//pridat transporty mezi zememi
	insertTransport(positiontransportort++, PL, CZ, 280, 7);
	insertTransport(positiontransportort++, HU, SK, 430, 2);
	insertTransport(positiontransportort++, SK, CZ, 160, 4);
	insertTransport(positiontransportort++, HU, UA, 280, 13);




//	INSERT staty, position, jmeno, 		sportrebaL, sportrebaZ,  velZasob, zasoby,  produkce
	insertStat(position++, "Cz", 553,  1382, 3436000, 	2000000, 29, 0);			//0
	insertStat(position++, "Sl", 386,  943,  3020000, 	1500000, 14, 0);			//1
	insertStat(position++, "Pl", 1638, 2524, 2225000, 	1000000, 708, 10);			//2
	insertStat(position++, "Hu", 578,  1527, 6330000, 	3000000, 223, 39);			//3
	insertStat(position++, "Ua", 3229, 8535, 31950000, 	 20000000, 2288, 0);			//4



}


int main() {

	int b = 1;
//    scanf("%d",&b);
    if(b == 1) {
    	testOdpojeniRuska();
    }
    if(b == 2) {
    	testPripojeniUSA();
	}



	int i = 0;
	while(i < 5*365*24){

		if((i % 24) == 0){
			cout<< "---------------" <<endl;
			printTime(i);
		}
		sCountry a;

		//tady vlozit z terminalu nebo ze zdroju


		for (int l = 0; l < positionTerminal; l++) {
//			if(!importFromTerminal(&terminaly[l])){
//				cout << "Full zasobnik" <<endl;
//			}
			transakceTerminal(&terminaly[l]);
		}


		for (int j = 0; j < position; j++) {

			if(!spotrebuj(&staty[j]) || !exportuj(&staty[j])){
				cout << "Zasoba byla spotrebovana" <<endl;
				//odstranit stat ze struktury statu
			}
			if((i % 24) == 0){
				cout << "stat:" << staty[j].jmeno << " stav:" << staty[j].zasoba << endl;
			}
			if(staty[j].zasoba == 0) {
				staty[j].zasoba = 0;
			    int b;
			    scanf("%d",&b);
			    staty[j].zasoba = 99999999999999;
			}

		}

//		cout<<"transporty"<<endl;
		for (int k = 0; k < positiontransportort; k++) {
//			if(!transportuj(&transporty[k])){
//				cout << "Zasoba byla spotrebovana nebo plny zasobnik" <<endl;
//			}
			transakceTransport(&transporty[k]);

		}

//		cout<<"transakceCheck"<<endl;
		transakceCheck();


		++i;

	}
	return 0;
}
