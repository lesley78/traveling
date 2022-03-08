
#include <iostream>
#include <stdio.h>              //
#include <windows.h>            //!_убрать_!
#include <conio.h>              //

#pragma warning(disable:4996)       //!_убрать_!

using namespace std;


    //Фунции

int powInt(int N1, int N2) // Без негативных степеней
{
    int ret = N1;
    for (int i = N2; i > 1; i--)
    {
        ret = ret * N1;
    }
    return ret;
}


int getData(FILE* file_)    //добавить ссылки на массивы из main
{
    char str[3];
    char finder[7];
    bool isGettingId = false;
    bool isGettingType = false;
    bool isGettingX = false;
    bool isGettingY = false;
    int id = -1;
    int i = -1;
    int objType[10];    //
    int objX[10];       // !_перенести в main_!  (и оставить на них ссылку в параметрах функции)
    int objY[10];       //

    while (fgets(str, 2, file_))
    {
        for (int i = 0; i < 5; i++)
        {
            finder[i] = finder[i+1];
        }
        finder[5] = str[0];

        if (isGettingId)
        {

            if (i == -1 && finder[5] != '"')
            {
                i = 1;
                id = (finder[5] - '0');
            }
            else if  (finder[5] != '"')
            {
                id = id * 10;
                id = id + (finder[5] - '0');
            }

            if (finder[5] == '"')
            {
                i = -1;
                isGettingId = false;
            }
        }

        if (isGettingType && id != -1)
        {

            objType[id] = (finder[5] - '0');
            isGettingType = false;
        }

        if (isGettingX)
        {
            if (i == -1 && finder[5] != '"')
            {
                i = 1;
                objX[id] = (finder[5] - '0');
            }
            else if (finder[5] != '"')
            {
                objX[id] = objX[id] * 10;
                objX[id] = objX[id] + (finder[5] - '0');
            }

            if (finder[5] == '"')
            {
                i = -1;
                isGettingX = false;
            }
        }

        if (isGettingY)
        {
            if (i == -1 && finder[5] != '"')
            {
                i = 1;
                objY[id] = (finder[5] - '0');
            }
            else if (finder[5] != '"')
            {
                objY[id] = objY[id] * 10;
                objY[id] = objY[id] + (finder[5] - '0');
            }

            if (finder[5] == '"')
            {
                i = -1;
                isGettingY = false;
            }
        }

        if (finder[2] == 'I' && finder[3] == 'D' && finder[4] == ':' && finder[5] == '"')
        {
            isGettingId = true;
            isGettingType = false;
            isGettingX = false;
            isGettingY = false;
        }

        if (finder[0] == 'T' && finder[1] == 'Y' && finder[2] == 'P' && finder[3] == 'E' && finder[4] == ':' && finder[5] == '"')
        {
            isGettingId = false;
            isGettingType = true;
            isGettingX = false;
            isGettingY = false;
        }

        if (finder[3] == 'X' && finder[4] == ':' && finder[5] == '"')
        {
            isGettingId = false;
            isGettingType = false;
            isGettingX = true;
            isGettingY = false;
        }

        if (finder[3] == 'Y' && finder[4] == ':' && finder[5] == '"')
        {
            isGettingId = false;
            isGettingType = false;
            isGettingX = false;
            isGettingY = true;
        }
        
    }

    for (int i = 0; i < 10; i++)
    {
        cout << "\n" << objType[i] << "\t" << objX[i] << "\t" << objY[i];
    }


    return 0;
}


void A1();
void A2();



int main()
{
    //A1();

    //int objType[10];
    //int objX[10];
    //int objY[10];

    A2();
    //cout << powInt(10, 3);

}

    //Пункты

void A1()
{
    char str[3];
    FILE* file;
    fopen_s(&file, "1.txt", "rt");

    while (fgets(str, 2, file))
    {

        cout << str;
        Sleep(1000);
    }
    fclose(file);

    while (true)
    {
        Sleep(100);
        if (kbhit() != 0) break;    //!_убрать_!
    }
}

void A2()
{
    FILE* file;
    fopen_s(&file, "2.txt", "rt");
    cout << "\n\n" << getData(file);

}
