#include <iostream>
#include <stdio.h>              //
#include <windows.h>            //!_убрать_!
#include <conio.h>              //
#include <locale.h>
#include <clocale>

#pragma warning(disable:4996)       //!_убрать_!

using namespace std;

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


const unsigned int X = 100;     // Размер
const unsigned int Y = 40;      // консоли

    // Фунции //

int powInt(int N1, int N2) // Без негативных степеней
{
    int ret = N1;
    for (int i = N2; i > 1; i--)
    {
        ret = ret * N1;
    }
    return ret;
}

void SetWindow(int Width, int Height)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE); Sleep(10);     // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord); Sleep(10);           // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);                      // Set Window Size 

}

void makeFrame()
{
    cout << "\n";
    for (int iy = 0; iy < Y; iy++)
    {
        for (int ix = 0; ix < X; ix++)
        {
            if ((ix != 0 && ix != X - 1) && (iy == 0 || iy == Y - 1 || iy == (Y * 2) / 3)) cout << (char)205;
            else if (ix == 0 && iy == (Y * 2) / 3) cout << (char)204;
            else if (ix == X - 1 && iy == (Y * 2) / 3) cout << (char)185;
            else if ((iy != 0 && iy != Y - 1) && (ix == 0 || ix == X - 1)) cout << (char)186;
            else if (ix == 0 && iy == 0) cout << (char)201;
            else if (ix == X - 1 && iy == 0) cout << (char)187;
            else if (ix == X - 1 && iy == Y - 1) cout << (char)188;
            else if (ix == 0 && iy == Y - 1) cout << (char)200;
            else cout << ' ';
        }
    }
}

void coutLV(char out[])
{
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void A1();
void A2(int[], int[], int[]);

int main()
{

    SetWindow(X, Y);

    //A1();

    //makeFrame();

    int objType[10] = { 0 };
    int objX[10] = { 0 };
    int objY[10] = { 0 };

    A2(objType, objX, objY);

    /*gotoxy(3, ((Y * 2) / 3) + 3);
    cout << "How are you? ";
    cin.get();*/

}

// Пункты //

void A1()
{
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    char str[17];
    FILE* file;
    fopen_s(&file, "1.txt", "rt");

    if (!file) return;

    while (fgets(str, 16, file))
    {
        cout << str;
    }
    fclose(file);

    while (!kbhit());       //!_убрать_!
    clear();
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}


void A2(int objType[], int objX[], int objY[])
{
    char str[3];
    char finder[16] = { };
    int id = 0;

    FILE* file;
    fopen_s(&file, "2.txt", "rt");
    if (!file) return;
    while (fgets(str, 2, file))
    {
        for (int i = 0; i < 15; i++)
        {
            finder[i] = finder[i + 1];
        }
        finder[15] = str[0];

        sscanf_s(finder, "ID:\x22%d\x22", &id);
        sscanf_s(finder, "TYPE:\x22%d\x22", &objType[id]);
        sscanf_s(finder, "X:\x22%d\x22", &objX[id]);
        sscanf_s(finder, "Y:\x22%d\x22", &objY[id]);

    }
    fclose(file);

         //вкл/выкл проверку 1/0
    for (int i = 0; i < 10 && 1; i++)
    {
        if (objType[i] == 0) continue;
        cout << "\n" << objType[i] << "\t" << objX[i] << "\t" << objY[i];
    }

}
