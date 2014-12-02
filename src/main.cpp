//============================================================================
// Name        : IMS.cpp
// Author      : xjanou14
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


/*
cesko 	import 	export 	produkce 	spotreba 	zasobniky 	maximalniOdber
cesko 	968 	1 		29 			968 		3436000
polsko 	1424 	11 		708 		2081 		2225000
slov 	637 	0 		14 			664 		3020000
mad 	933 	165 	223 		1053 		6330000
ukr 	3139 	0 		2288 		5882 		3195000

*/
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

typedef struct Terminal{
	int zasoba = 0;
	int maxVydej = 0;
}terminal;

typedef struct SExport{
	int src;
	int dst;
	int mnozstvi;
}sExport;

typedef struct Stat{
	//jednotky 100 m^3
	string jmeno;
	int spotreba = 0;
	int produkce = 0;
	int zasoba = 0;
	int max = 5000;
}stat;

void insertStat( stat staty[],int position, string jmeno, int sportreba, int zasoba, int produkce){

	stat pom;
	pom.jmeno = jmeno;
	pom.spotreba = sportreba;
	pom.zasoba = zasoba;
	pom.produkce = produkce;

	staty[position] = pom;
}

void insertExport(sExport exp[], int position, int src, int dst, int mnozstvi){

	sExport pom;
	pom.src = src;
	pom.dst = dst;
	pom.mnozstvi = mnozstvi;

	exp[position] = pom;
}

bool exportuj(stat staty[], sExport * exp){
//	TODO zkontrolovat podminku
	if((staty[exp->src].zasoba > exp->mnozstvi) && (staty[exp->dst].zasoba <= (staty[exp->dst].zasoba + exp->mnozstvi))){
		cout<<"Spotrebovavam"<<endl;
		staty[exp->src].zasoba -= exp->mnozstvi;
		staty[exp->dst].zasoba += exp->mnozstvi;
		return true;
	}else{
//	TODO action co se ma stat
		return false;
	}
}

bool spotrebuj(stat * s){
	if(s->zasoba > s->spotreba){
		s->zasoba -= s->spotreba;
		return true;
	}else{
		s->zasoba = 0;
		return false;
	}
}


int main() {

	stat staty[6];
	sExport exp[2];

    int position = 0;
    int positionExport = 0;


    insertExport(exp, positionExport++, 0, 3, 5);
	insertExport(exp, positionExport++, 0, 1, 5);



	insertStat(staty,position++, "Cesko",5,200, 0);		//1
	insertStat(staty,position++, "Slovensko",5,100, 0);	//2
	insertStat(staty,position++, "Polsko",5,60, 0);		//3
	insertStat(staty,position++, "Madarsko",5,30, 0);	//4
	insertStat(staty,position, "Ukrajina",5,10, 0);		//5


	int i = 0;
	while(i < 10){

		cout<< "---------------" <<endl;
		stat a;

		//tady vlozit z terminalu nebo ze zdroju

		for (int j = 0; j < position; j++) {
			cout << "stat:" << staty[j].jmeno << " stav:" << staty[j].zasoba << endl;

			if(!spotrebuj(&staty[j])){
				cout << "Zasoba byla spotrebovana" <<endl;
				//odstranit stat ze struktury statu
			}
		}

		for (int k = 0; k < positionExport; k++) {
			if(!exportuj(staty, &exp[k])){
				cout << "Zasoba byla spotrebovana" <<endl;
			}
		}

		++i;
	}






	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
