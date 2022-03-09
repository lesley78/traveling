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
    SetConsoleCP(1257);SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void A1();
void A2(int[], int[], int[]);

int main()
{
    
    SetWindow(X, Y);

    A1();

    makeFrame();

    int objType[10] = { 0 };
    int objX[10] = { 0 };
    int objY[10] = { 0 };

    A2(objType, objX, objY);

    gotoxy(3, ((Y * 2) / 3) + 3);
    cout << "How are you? ";
    cin.get();

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
    char finder[6] = "     ";
    bool isGettingId = false;
    bool isGettingType = false;
    bool isGettingX = false;
    bool isGettingY = false;
    int id = -1;
    int i = -1;


    FILE* file;
    fopen_s(&file, "2.txt", "rt");
    if (!file) return;
    while (fgets(str, 2, file))
    {
        for (int i = 0; i < 5; i++)
        {
            finder[i] = finder[i + 1];
        }
        finder[5] = str[0];

        if (isGettingId)
        {
            if (i == -1 && finder[5] != '"')
            {
                i = 1;
                id = (finder[5] - '0');
            }
            else if (finder[5] != '"')
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
            isGettingId = true;
        if (finder[0] == 'T' && finder[1] == 'Y' && finder[2] == 'P' && finder[3] == 'E' && finder[4] == ':' && finder[5] == '"')
            isGettingType = true;
        if (finder[3] == 'X' && finder[4] == ':' && finder[5] == '"') 
            isGettingX = true;
        if (finder[3] == 'Y' && finder[4] == ':' && finder[5] == '"')
            isGettingY = true;
    }
    fclose(file);

         //вкл/выкл проверку 1/0
    for (int i = 0; i < 10 && 0; i++)
    {
        if (objType[i] == 0) continue;
        cout << "\n" << objType[i] << "\t" << objX[i] << "\t" << objY[i];
    }

}
