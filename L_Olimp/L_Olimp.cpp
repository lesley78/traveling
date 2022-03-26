#include <iostream>
#include <stdio.h>              
#include <windows.h>            
#include <conio.h>              
#include <locale.h>
#include <clocale>
#include <cwchar>
#include <iomanip>
#include <ctime>

#pragma warning(disable:4996)       //!_Убрать_!

using namespace std;

const int objMaxNameLength = 20;    // максимальная длинна имени объекта
const int objMaxIndexLength = 5;    // максимальная длинна индекса объекта
const int objCount = 20;        // количество ID в файле (потом переделать на автоподщёт или другой способ получения данных) 
const int maxCharge = 140;      // максимальный заряд машины (в км)

const unsigned int X = 2 + ((objCount + 1) * 4) + 2 + 32;               // размер экрана по X   
const unsigned int Y = 2 + ((objCount * 2) - 1) + 3 + objCount + 2;     // размер экрана по Y

#define clear() printf("\033[H\033[J")       // очистить всю консоль
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))     // переместить "курсор" на координату (x;y) (консоль)
#define coutxy(x,y,text) printf("\033[%d;%dH%s", (y), (x) , (text))     // вывести текст в консоль на координате (x;y) 
#define coutxyBot(x,y,text) printf("\033[%d;%dH%s", (y + 4 + ((objCount * 2) - 1) + 3), (x + 2) , (text))   // вывести текст в консоль на координате (x;y) в нижней части игрового экрана

    // Функции //

// очистить консоль С координаты (y) ДО конца в нижней части игрового экрана
void clearBotY(int y)
{
    for (int i = 0; y + i + 2 + ((objCount * 2) - 1) + 3 < Y - 2; i++)
    {
        for (int j = 0; j < X - 4; j++)
        {
            coutxy(3 + j, y + i + 4 + ((objCount * 2) - 1) + 3, " ");
        }
    }
}       

// вычеслить степень (неиспользуется)
int powInt(int N1, int N2) // ??? ?????????? ????????
{
    int ret = N1;
    for (int i = N2; i > 1; i--)
    {
        ret = ret * N1;
    }
    return ret;
}

// установить размер экрана (в символах)
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

// создать рамку
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

// cout но с локалью Latvian и возвратом на локаль C (неиспользуется)
void coutLV(const char out[])
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

// cout но с локалью Latvian и возвратом на локаль lv_LV.UTF-8 (используется но 1 раз)
void coutLV8(const char out[])
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    cout << out;
    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

// возвращает расстоякие (км) между 2-мя объектами, используя их реальные координаты
float getDistance(float latitude1, float longitude1, float latitude2, float longitude2)
{
    float radius = 6371, rad = 3.14159265 / 180;
    float latitude = abs((latitude2 - latitude1)) * rad;
    float longitude = abs((longitude2 - longitude1)) * rad;
    float rez = sin(latitude / 2) * sin(latitude / 2) + cos(latitude1 * rad) * cos(latitude2 * rad) * sin(longitude / 2) * sin(longitude / 2);
    float d = 2 * radius * atan2(sqrt(rez), sqrt(1 - rez));
    return d;
}

// заполняет массив данными расстояний между объектами
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

// устанавливает цвет текста (int)
void setTextColor(int Color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
}

// переместить "курсор" на координату (x;y) в верхней части игрового экрана
void gotoxyTop(int x, int y)
{
    gotoxy(x + 2, y + 2);
}

// переместить "курсор" на координату (x;y) в нижней части игрового экрана
void gotoxyBot(int x, int y)
{
    gotoxy(x + 2, y + 4 + ((objCount * 2) - 1) + 3);
}

// музыка (есть проблема)
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

// музыка (есть проблема)
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

// унаёт id объекта ипользуя его индекс
int findIndexId(char Index[], char objIndex[][objMaxIndexLength])
{
    for (int i = 0; i < objCount; i++) 
    {
        if (Index[0] == objIndex[i][0] && Index[1] == objIndex[i][1]) return i;
    }
    return -1;
}

int length(int num)
{
    int counter = 0;
    do
    {
        counter = counter + 1;
        num = num / 10;
    } while (num != 0);
    return counter;
}

// обновляет показатели заряда и количество пунктов на экране
void updInfo(float charge, int points)
{
    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 1);
    cout << "[";
    for (int i = 0; i < X - (((objCount + 1) * 4) + 3 + 1) - 5; i++)
    {
        if (i <= ((charge / (int)maxCharge)) * (X - (((objCount + 1) * 4) + 3 + 1) - 5)) cout << "|";
        else cout << " ";
    }
    cout << "]";

    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 3);
    cout << "Charge: " << setw(3) << 100 * (int)charge / (int)maxCharge << "% (" << (int)charge << "km) ";
   
    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 5);
    cout << "Bonus Points: " << setw(length(points)) << points;
}

//
void setFontSize(int x, int y)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = x;                   // Width of each character in the font
    cfi.dwFontSize.Y = y;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

bool canCharge(int curPos, char objIndex[][objMaxIndexLength])
{
    if ((int)objIndex[curPos][0] >= (int)'0' && (int)objIndex[curPos][0] <= (int)'9' && (int)objIndex[curPos][1] >= (int)'0' && (int)objIndex[curPos][1] <= (int)'9') return true;
    else return false;
}

bool canDrive(float objDistanse[][objCount], int curPos, int charge)
{
    for (int i = 1;true;i++)
    {
        if (curPos + i < objCount && objDistanse[curPos][curPos + i] <= charge) return true;
        if (curPos - i >= 0 && objDistanse[curPos][curPos - i] <= charge) return true;
        if (curPos - i < 0 && curPos + i >= objCount) return false;
    }
}

bool weather_random() { if (rand() % 10 == 0) return 1; return 0; }

int exercise_random() {
    return (rand() % 3) + 1;
}

// прототипы Основных Функций

void A1();
void A2(int[], float[], float[], char[][objMaxNameLength], char[][objMaxIndexLength]);
void A3(int[], float[][objCount], char[][objMaxIndexLength], int, float);
void displayNames(char[][objMaxNameLength], char[][objMaxIndexLength]);
int question(char[][objMaxIndexLength],int);
float distance(int*, float, char[][objMaxIndexLength], float[][objCount],int);
void write(const int, int*);
void tryCharge(int, int*, float*);

int main()
{

    int objType[objCount + 1] = { 0 };
    int curPos = 0;
    int bonusP = 50;
    int inputY = 1;
    int typeOfWrite = -1;
    float objLongitude[objCount + 1] = { 0 };
    float objLatitude[objCount + 1] = { 0 };
    float objDistanse[objCount][objCount] = { 0. };
    float charge = maxCharge;
    char objNames[objCount][objMaxNameLength] = { " " };
    char objIndex[objCount][objMaxIndexLength] = { " " };
    bool doCharge = false;

    srand(time(NULL));
    //music2();
    setTextColor(15);
    setFontSize(0, 12);
    
    A1();
    SetWindow(X, Y);
    makeFrame();
    A2(objType, objLongitude, objLatitude, objNames, objIndex);
    fillDistanse(objDistanse, objLongitude, objLatitude, objType);
    A3(objType, objDistanse, objIndex, curPos, charge);
    displayNames(objNames, objIndex);

    updInfo(charge, bonusP);
    while (1) 
    {
        write(1, &inputY);
        charge = charge - distance(&curPos, charge, objIndex, objDistanse, inputY);
        A3(objType, objDistanse, objIndex, curPos, charge);
        clearBotY(1);
        updInfo(charge, bonusP);

        write(3, &inputY);
        bonusP = bonusP + question(objIndex, inputY);
        clearBotY(1);
        updInfo(charge, bonusP);

        if (bonusP >= 10) if (canCharge(curPos, objIndex))
        {
            if (canDrive(objDistanse, curPos, charge))
            {
                write(2, &inputY);
                tryCharge(inputY, &bonusP, &charge);
                clearBotY(1);
                updInfo(charge, bonusP);
                A3(objType, objDistanse, objIndex, curPos, charge);
            }
            else
            {
                write(4, &inputY);
                bonusP = bonusP - 10;
                charge = maxCharge;
                //scanf("%с");
                gotoxyBot(1, inputY);
                getch();
                clearBotY(1);
                updInfo(charge, bonusP);
                A3(objType, objDistanse, objIndex, curPos, charge);
            }
        }

        if (cin.get() == 'S') break;
    }

}

// Основные Функции //

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

    getch();
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
    int y = 2 + 2 + 3;
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

int question(char objIndex[][objMaxIndexLength], int inputY)
{
    int points = 10;
    char answer[8];
    while (1) {
        clearBotY(inputY);
        gotoxyBot(1, inputY);
        cin >> answer;
        if (answer[0] == '3') return points;
        else if (points > 0) points = points - 5; 
    }
}

float distance(int* curPos, float charge, char objIndex[][objMaxIndexLength], float objDistanse[][objCount], int inputY)
{
    gotoxyBot(2, 2);
    char answer[8];
    int id;
    int bufferCurPos = *curPos;
    
    while (1) {
        gotoxyBot(1, inputY);
        cin >> answer;
        id = findIndexId(answer, objIndex);
        if (id == -1) continue;
        if (charge - objDistanse[bufferCurPos][id] > 0 && bufferCurPos != id)
        {
            *curPos = id;
            return objDistanse[bufferCurPos][id];
        }
        else { clearBotY(inputY); coutxyBot(1, inputY + 2 ,"Nepielaujamie dati"); }
    }
}

void write(const int type, int* inputY)
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251); SetConsoleOutputCP(1251);
    switch (type)
    {
    case 1:
        gotoxyBot(1, 1);
        cout << "Izvelieties galamerki";
        *inputY = 3;
        break;
    case 2:
        gotoxyBot(1, 1);
        cout << "vai velaties uzladeties? (Y / N) (10BP)";
        *inputY = 3;
        break;
    case 3:
        gotoxyBot(1, 1);
        cout << "gfedjhtylkrsjhoitrjhrtj 3"; //для викторины
        *inputY = 3;
        break;
    case 4:
        gotoxyBot(1, 1); cout << "у вас закончился заряд автомобиля, но есть деньги на зарядку.";
        gotoxyBot(1, 2); cout << "Нажмите Enter что-бы зарядится.";
        *inputY = 4;
        break;
    case 5:
        gotoxyBot(1, 1); cout << "у вас закончился заряд автомобиля, но есть деньги на вызов эвакуатора. ";
        gotoxyBot(1, 2); cout << "Введите индекс ";
        *inputY = 4;
        break;
    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void tryCharge(int inputY, int* bonusP, float* charge)
{
    char answer[8];
    while (1) {
        gotoxyBot(1, inputY);
        cin >> answer;
        if (answer[0] == 'Y')
        {
            *bonusP = *bonusP - 10;
            *charge = maxCharge;
            return;
        }
        else if (answer[0] == 'N') return;
        else { clearBotY(inputY); coutxyBot(1, inputY + 2, "Nepielaujamie dati"); }
    }
}
