#include <iostream>
#include <locale.h>
#include <Windows.h>
using namespace std;
int main()
{
	setlocale(LC_ALL, "Latvian");
	SetConsoleCP(1257);
	SetConsoleOutputCP(1257);
	cout << "Sp�l�t�js dodas braucien� ar elektroma��nu. Lai ce�ojums veiksm�gi paietu , j�r�p�jas par akumulatoru un j�iebrauc  uzpildes stacij�s." << endl;
	return 0;
}