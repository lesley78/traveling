#include <iostream>
#include <stdio.h>              //
#include <windows.h>            //!_??????_!
#include <conio.h>              //
#include <locale.h>
#include <clocale>
#include <cwchar>
#include <iomanip>

#pragma warning(disable:4996)       //!_??????_!

using namespace std;

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define clearBot() printf("\033[%d;%dH\033[J", 4 + ((objCount * 2) - 1) + 3, 0)

const int objMaxNameLength = 20;
const int objMaxIndexLength = 5;
const int objCount = 20; // ?????????? ID ? ????? 
const int maxCharge = 140;

const unsigned int X = 2 + ((objCount + 1) * 4) + 2 + 32;                                                 // ??????
const unsigned int Y = 2 + ((objCount * 2) - 1) + 3 + objCount + 2;        // ???????

    // ????? //

int powInt(int N1, int N2) // ??? ?????????? ????????
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

void coutLV(const char out[])
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void coutLV8(const char out[])
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "lv_LV.UTF-8");
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

void setTextColor(int Color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
}

void gotoxyTop(int x, int y)
{
    gotoxy(x + 2, y + 2);
}

void gotoxyBot(int x, int y)
{
    gotoxy(x + 2, y + 4 + ((objCount * 2) - 1) + 3);
}

void music()
{
    while (1)
    {
        Beep(392, 350);
        Sleep(120);
        Beep(392, 350);
        Sleep(120);
        Beep(392, 350);
        Sleep(70);
        Beep(311, 250);
        Sleep(70);
        Beep(466, 100);
        Sleep(70);
        Beep(392, 350);
        Sleep(70);
        Beep(311, 250);
        Sleep(70);
        Beep(466, 100);
        Sleep(70);
        Beep(392, 700);
        Sleep(120);
        Beep(587, 350);
        Sleep(120);
        Beep(587, 350);
        Sleep(120);
        Beep(587, 350);
        Sleep(80);
        Beep(622, 250);
        Sleep(70);
        Beep(466, 100);
        Sleep(70);
        Beep(369, 350);
        Sleep(70);
        Beep(311, 250);
        Sleep(70);
        Beep(466, 100);
        Sleep(200);
        Beep(392, 700);
        Sleep(70);
        Beep(784, 350);
        Sleep(50);
        Beep(392, 250);
        Sleep(50);
        Beep(392, 100);
        Sleep(50);
        Beep(784, 350);
        Sleep(30);
        Beep(739, 250);
        Sleep(30);
        Beep(698, 100);
        Sleep(30);
        Beep(659, 100);
        Sleep(30);
        Beep(622, 100);
        Sleep(30);
        Beep(659, 450);
        Sleep(120);
        Beep(415, 150);
        Sleep(70);
        Beep(554, 350);
        Sleep(70);
        Beep(523, 250);
        Sleep(30);
        Beep(493, 100);
        Sleep(30);
        Beep(466, 100);
        Sleep(30);
        Beep(440, 100);
        Sleep(30);
        Beep(466, 450);
        Sleep(30);
        Beep(311, 150);
        Sleep(70);
        Beep(369, 350);
        Sleep(70);
        Beep(311, 250);
        Sleep(30);
        Beep(466, 100);
        Sleep(70);
        Beep(392, 750);
        Sleep(1000);
    }
}

void music2()
{
    while (1)
    {
        Beep(220, 300);
        Beep(294, 300);
        Beep(294, 300);
        Beep(370, 300);
        Beep(494, 300);
        Beep(370, 300);
        Beep(440, 800);
        /* */
        Beep(440, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(370, 300);
        Beep(392, 300);
        Beep(370, 300);
        Beep(330, 800);
        /* */
        Beep(247, 300);
        Beep(330, 300);
        Beep(330, 300);
        Beep(370, 300);
        Beep(555, 300);
        Beep(555, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(392, 800);
        Beep(392, 300);
        Beep(370, 300);
        Beep(247, 800);
        Beep(278, 300);
        Beep(294, 300);
        Beep(330, 2600);
        /* */
        Beep(220, 300);
        Beep(294, 300);
        Beep(294, 300);
        Beep(370, 300);
        Beep(494, 300);
        Beep(370, 300);
        Beep(440, 800);
        /* */
        Beep(440, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(370, 300);
        Beep(392, 300);
        Beep(370, 300);
        Beep(330, 800);
        /* */
        Beep(247, 300);
        Beep(330, 300);
        Beep(330, 300);
        Beep(370, 300);
        Beep(555, 300);
        Beep(555, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(392, 800);
        Beep(392, 300);
        Beep(370, 300);
        Beep(278, 600);
        Beep(330, 600);
        Beep(294, 2600);
        /*Reff : */
        Beep(494, 300);
        Beep(494, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(392, 200);
        Beep(440, 200);
        Beep(494, 200);
        Beep(440, 800);
        Beep(330, 300);
        Beep(370, 300);
        Beep(416, 300);
        Beep(330, 300);
        Beep(440, 2000);
        /* */
        Beep(494, 800);
        Beep(440, 800);
        Beep(392, 1600);
        /* */
        Beep(555, 300);
        Beep(555, 300);
        Beep(555, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(392, 1400);
        /* */
        Beep(440, 300);
        Beep(494, 300);
        Beep(370, 1100);
        Beep(330, 300);
        Beep(294, 1800);
        /* */
        Beep(494, 800);
        Beep(440, 800);
        Beep(392, 1600);
        /* */
        Beep(555, 300);
        Beep(555, 300);
        Beep(555, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(494, 300);
        Beep(440, 300);
        Beep(392, 1400);
        /* */
        Beep(440, 300);
        Beep(494, 300);
        Beep(370, 1100);
        Beep(330, 300);
        Beep(294, 1800);
    }
}

int findIndexId(char Index[], char objIndex[][objMaxIndexLength])
{
    for (int i = 0; i < objCount; i++) 
    {
        if (Index[0] == objIndex[i][0] && Index[1] == objIndex[i][1]) return i;
    }
    return -1;
}

void updCharge(float charge)
{
    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 3);
    cout << "Charge: " << setw(4) << 100 * (int)charge / (int)maxCharge << "%  " << "(" << (int)charge << "km) ";
    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 1);
    cout << "[";
    for (int i = 0; i < X - (((objCount + 1) * 4) + 3 + 1) - 5; i++)
    {
        if (i <= ((charge / (int)maxCharge)) * (X - (((objCount + 1) * 4) + 3 + 1) - 5)) cout << "|";
        else cout << " ";
    }
    cout << "]";
}

void updBot()
{
    clearBot();
    gotoxy(0, 4 + ((objCount * 2) - 1) + 3);
    for (int iy = 1 + ((Y * 2) / 3); iy < Y; iy++)
    {
        for (int ix = 0; ix < X; ix++)
        {
            if ((ix != 0 && ix != X - 1) && (iy == Y - 1)) cout << (char)205;
            else if ((iy != 0 && iy != Y - 1) && (ix == 0 || ix == X - 1)) cout << (char)186;
            else if (ix == X - 1 && iy == Y - 1) cout << (char)188;
            else if (ix == 0 && iy == Y - 1) cout << (char)200;
            else cout << ' ';
        }
    }

}

void A1();
void A2(int[], float[], float[], char[][objMaxNameLength], char[][objMaxIndexLength]);
void A3(int[], float[][objCount], char[][objMaxIndexLength], int, float);
void displayNames(char[][objMaxNameLength], char[][objMaxIndexLength]);
int question(char[][objMaxIndexLength]);
float distance(int* , int, float, char[][objMaxIndexLength], float[][objCount]);

void write(const int type, int* pinputY, bool* pdoCharge)
{
    switch (type) 
    {
        case 1:
            gotoxyBot(1, 1);
            cout << "Izvelieties galamerki";
            break;
        case 2:
            gotoxyBot(1, 1);
            cout << "vai velaties uzladeties?";
            break;
        case 3:
            gotoxyBot(1, 1);
            cout << "gfedjhtylkrsjhoitrjhrtj 3"; //для викторины
            break;
    }
    *pinputY = 2;
}

int main()
{
    setTextColor(15);

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

    int curPos = 0;
    float charge = maxCharge;
    A3(objType, objDistanse, objIndex, curPos, charge);

    //music2();

    displayNames(objNames, objIndex);

    int bonusP = 50;
    int* pCurPos = &curPos;

    int inputY = 0;
    int* pinputY = &inputY;

    bool doCharge = false;
    bool* pdoCharge = &doCharge;

    int typeOfWrite = -1;

    updCharge(charge);
    while (1) 
    {
        updBot();
        write(1, pinputY, pdoCharge);
        charge = charge - distance(pCurPos, curPos, charge, objIndex, objDistanse);
        //gotoxy(2, 2); cout << curPos; gotoxyBot(1, 1);
        A3(objType, objDistanse, objIndex, curPos, charge);
        updBot();
        updCharge(charge);
        write(3, pinputY, pdoCharge);
        bonusP = bonusP + question(objIndex);
        if (cin.get() == 'S') break;
    }

}

// ?????? //

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

    cout << "";

    while (!kbhit());       //!_??????_!
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
    while (fgets(str, 2, file) && id <= objCount)
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
        //cout << id << "\t" << objIndex[id] << "\t" << finder << endl;

    }
    fclose(file);

    //???/???? ???????? 1/0
    for (int i = 0; i < 14 && 0; i++)
    {
        if (objType[i] == 0) continue;
        cout << "\n" << objType[i] << "\t" << objLongitude[i] << "\t" << objLatitude[i] << "\t" << objNames[i] << "\t" << objIndex[i];
    }

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);

}

void A3(int objType[], float objDistanse[][objCount], char objIndex[][objMaxIndexLength], int curPos, float charge)
{

    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    gotoxyTop(1, 1);
    for (int iy = -1; iy < objCount; iy++)
    {
        for (int ix = -1; ix < objCount; ix++)
        {
            //Sleep(100);
            /*if (iy == ix) { printf(" %c  ", 'X'); continue; }
            if (iy == -1) { printf("%s  ", objIndex[ix]); continue; }
            if (ix == -1) { printf("%s  ", objIndex[iy]); continue; }
            printf("%.0f ", objDistanse[ix][iy]); continue;*/
            if (iy == ix) { if (iy == curPos && ix == curPos)  setTextColor(11); cout << setw(4) << "X "; if (iy == curPos && ix == curPos)  setTextColor(15); continue; }
            if (iy == -1) { if (ix == curPos)  setTextColor(11); cout << setw(4) << objIndex[ix]; if (ix == curPos)  setTextColor(15); continue; }
            if (ix == -1) { if (iy == curPos)  setTextColor(11); cout << setw(4) << objIndex[iy]; if (iy == curPos)  setTextColor(15); continue; }
            if (charge - objDistanse[curPos][iy] > 0 && charge - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos))  setTextColor(10);
            cout << setw(4) << (int)objDistanse[ix][iy];
            if (charge - objDistanse[curPos][iy] > 0 && charge - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos))  setTextColor(15);
        }
        gotoxyTop(1, 1 + (2 * (iy + 2)));
    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void displayNames(char objNames[][objMaxNameLength], char objIndex[][objMaxIndexLength])
{
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    int x = ((objCount + 1) * 4) + 3 + 1;
    int y = 2 + 2 + 1;
    gotoxyTop(x, y);
    coutLV8("99 - uzlвdзрanas stacija");
    for (int i = 0, counter = 0; i < objCount; i++)
    {
        gotoxyTop(x, 2 + y + (counter * 2));

        if ((int)objIndex[i][0] >= (int)'0' && (int)objIndex[i][0] <= (int)'9' && (int)objIndex[i][1] >= (int)'0' && (int)objIndex[i][1] <= (int)'9') continue;
        counter = counter + 1;
        cout << objIndex[i] << " - " << objNames[i];

    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

int question(char objIndex[][objMaxIndexLength])
{
    int points = 10;
    char answer[8];
    while (1) {

        gotoxyBot(1, 3);
        cin >> answer;
        if (answer[0] == '3') return points;
        else if (points <= 0) points = points - 5;
    }
}

float distance(int* pCurPos, int curPos, float charge, char objIndex[][objMaxIndexLength], float objDistanse[][objCount])
{
    gotoxyBot(2, 2);
    char answer[8];
    int id;
    int bufferCurPos = curPos;
    
    while (1) {
        gotoxyBot(1, 3);
        cin >> answer;
        id = findIndexId(answer, objIndex);
        if (id == -1) continue;
        //gotoxy(2, 2);cout << charge - objDistanse[bufferCurPos][id];gotoxyBot(1, 1);
        if (charge - objDistanse[bufferCurPos][id] > 0)
        {
            gotoxyBot(1, 3);
            *pCurPos = id;
            //gotoxy(2, 2); cout << curPos; gotoxyBot(1, 1);
            return objDistanse[bufferCurPos][id];
        }
        else cout << "Nepielaujamie dati";
    }
}
