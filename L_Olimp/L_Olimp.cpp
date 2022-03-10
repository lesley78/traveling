#include <iostream>
#include <stdio.h>              //
#include <windows.h>            //!_убрать_!
#include <conio.h>              //
#include <locale.h>
#include <clocale>
#include <cwchar>

#pragma warning(disable:4996)       //!_убрать_!

using namespace std;

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

const int objMaxNameLength = 20;
const int objMaxIndexLength = 5;
const int objCount = 20; // количество ID в файле 

const unsigned int X = 160;     // Размер
const unsigned int Y = 2  + ((objCount * 2) - 1) + 3 + objCount + 2;      // консоли

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
    //API
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

float getDistance(float latitude1, float longitude1, float latitude2, float longitude2)
{
    float radius = 6371, rad = 3.14159265 / 180;
    float latitude = abs((latitude2 - latitude1)) * rad;
    float longitude = abs((longitude2 - longitude1)) * rad;
    float rez = sin(latitude / 2) * sin(latitude / 2) + cos(latitude1 * rad) * cos(latitude2 * rad) * sin(longitude / 2) * sin(longitude / 2);
    float d = 2 * radius * atan2(sqrt(rez), sqrt(1 - rez));
    return d;
}

void fillDistanse(float objDistanse[][objCount], float objLongitude[], float objLatitude[], int objType[])
{
    for (int i = 0; i < objCount; i++)
    {
        for (int j = 0; j < objCount; j++)
        {
            if (i != j)
            {
                objDistanse[i][j] = getDistance(objLongitude[i], objLatitude[i], objLongitude[j], objLatitude[j]);
            }
        }
    }
}

void gotoxyTop(int x, int y)
{
    gotoxy(x + 2, y + 2);
}

void gotoxyBot(int x, int y)
{
    gotoxy(x + 2, y + 4 + ((objCount * 2) - 1) + 3);
}

void A1();
void A2(int[], float[], float[], char[][objMaxNameLength], char[][objMaxIndexLength]);
void A3(int, int, int[], float[][objCount], char[][objMaxIndexLength]);

int main()
{


    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = 12;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    A1();

    SetWindow(X, Y);

    makeFrame();

    int objType[objCount + 1] = { 0 };
    float objLongitude[objCount + 1] = { 0 };
    float objLatitude[objCount + 1] = { 0 };
    char objNames[objCount][objMaxNameLength] = { " " };
    char objIndex[objCount][objMaxIndexLength] = { " " };

    A2(objType, objLongitude, objLatitude, objNames, objIndex);

    float objDistanse[objCount][objCount] = { 0. };
    fillDistanse(objDistanse, objLongitude, objLatitude, objType);

    A3(2, 2, objType, objDistanse, objIndex);

    gotoxyBot(1, 1);
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


void A2(int objType[], float objLongitude[], float objLatitude[], char objNames[][objMaxNameLength], char objIndex[][objMaxIndexLength])
{
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    char str[3];
    char finder[32] = { };
    int id = 0;

    FILE* file;
    fopen_s(&file, "2.txt", "rt");
    if (!file) return;
    while (fgets(str, 2, file))
    {
        for (int i = 0; i < 31; i++)
        {
            finder[i] = finder[i + 1];
        }
        finder[31] = str[0];

        sscanf_s(finder, "%*s ID:\x22%d\x22", &id);
        sscanf_s(finder, "%*s TYPE:\x22%d\x22", &objType[id]);
        sscanf_s(finder, "%*s LONT:\x22%f\x22", &objLongitude[id]);
        sscanf_s(finder, "%*s LATT:\x22%f\x22", &objLatitude[id]);
        sscanf_s(finder, "%*s NAME:\x22%[^\x22]\x22", &objNames[id], objMaxNameLength);
        sscanf_s(finder, "%*s IND:\x22%[^\x22]\x22", &objIndex[id], objMaxIndexLength);
        //cout << id << "\t" << objIndex[9] << "\t" << finder << endl;
 
    }
    fclose(file);

    //вкл/выкл проверку 1/0
    for (int i = 0; i < 14 && 0; i++)
    {
        if (objType[i] == 0) continue;
        cout << "\n" << objType[i] << "\t" << objLongitude[i] << "\t" << objLatitude[i] << "\t" << objNames[i] << "\t" << objIndex[i];
    }

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);

}

void A3(int x, int y, int objType[], float objDistanse[][objCount], char objIndex[][objMaxIndexLength])
{
    x = x + 1;
    y = y + 1;
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    gotoxy(x, y);
    for (int iy = -1; iy < objCount ; iy++)
    {
        for (int ix = -1; ix < objCount ; ix++)
        {
            //Sleep(100);
            if (iy == ix) { printf(" %c  ", 'X'); continue; }
            if (iy == -1) { printf("%s  ", objIndex[ix]); continue; }
            if (ix == -1) { printf("%s  ", objIndex[iy]); continue; }
            printf("%.3f ", objDistanse[ix][iy]); continue;
        }
        gotoxy(x, y + 4 + 2 * iy);
    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}
