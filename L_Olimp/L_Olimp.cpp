
#include <iostream>
#include <stdio.h>              //
#include <windows.h>            //!_убрать_!
#include <conio.h>              //

#pragma warning(disable:4996)       //!_убрать_!

using namespace std;

void A1();
void A2();

    //Фунции


int main()
{
    A1();
    while (true)                        
    {
        Sleep(5);
        if (kbhit() != 0) break;    //!_убрать_!
    }

    A2();
}

    //Пункты

void A1()
{
    char str[5];
    FILE* file;
    fopen_s(&file, "1.txt", "rt");

    while (fgets(str, 4, file))
    {
        cout << str;
    }
    fclose(file);
}

void A2()
{

}
