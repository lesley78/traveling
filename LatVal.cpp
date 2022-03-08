#include <iostream>
#include <locale.h>
#include <Windows.h>
using namespace std;
int main()
{
	setlocale(LC_ALL, "Latvian");
	SetConsoleCP(1257);
	SetConsoleOutputCP(1257);
	cout << "Spçlçtâjs dodas braucienâ ar elektromaðînu. Lai ceïojums veiksmîgi paietu , jârûpçjas par akumulatoru un jâiebrauc  uzpildes stacijâs." << endl;
	return 0;
}