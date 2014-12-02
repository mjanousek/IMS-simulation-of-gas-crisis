//============================================================================
// Name        : IMS.cpp
// Author      : xjanou14
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <list>
#include <iterator>
using namespace std;

typedef struct Terminal{
	int zasoba = 0;
	int maxVydej = 0;
}terminal;

typedef struct Stat{
	string jmeno;
	int spotreba = 0;
	int pExport = 0;
	int pImport = 0;
	int zasoba = 0;
}stat;

void insertStat(list<stat> * staty, string jmeno, int sportreba, int pExport, int pImport, int zasoba){

	stat pom;
	pom.jmeno = jmeno;
	pom.spotreba = sportreba;
	pom.pExport = pExport;
	pom.pImport = pImport;
	pom.zasoba = zasoba;

	staty->push_back(pom);
}

bool spotrebuj(stat * s){
	if(s->zasoba > s->spotreba){
		s->zasoba -= s->spotreba;
		cout << "spotrebovavam " << s->zasoba <<endl;
		return true;
	}else{
		s->zasoba = 0;
		cout << "neni co" <<endl;

		return false;
	}
}

int main() {

	list<stat> staty;

	insertStat(&staty,"Cesko",5,0,0,50);
	insertStat(&staty,"Slovensko",5,0,0,100);
	insertStat(&staty,"Polsko",5,0,0,60);
	insertStat(&staty,"Madarsko",5,0,0,30);
	insertStat(&staty,"Ukrajina",5,0,0,10);


	int i = 0;
	while(i < 10){

		cout<< "---------------" <<endl;
		stat a;

		for (int j = 0; j < staty.size(); j++, a = staty.next) {
			cout << "stat:" << a.jmeno << " stav:" << a.zasoba << endl;

			if(!spotrebuj(a)){
				cout << "Zasoba byla spotrebovana" <<endl;
				//odstranit stat ze struktury statu
			}
		}




		++i;
	}






	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
