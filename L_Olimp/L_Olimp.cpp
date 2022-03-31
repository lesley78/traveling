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

const int objMaxNameLength = 48;    // максимальная длинна имени объекта
const int objMaxIndexLength = 3;    // максимальная длинна индекса объекта
const int objCount = 21;        // количество ID в файле (потом переделать на автоподщёт или другой способ получения данных) 
const int maxCharge = 140;      // максимальный заряд машины (в км)
const int maxChargeRain = 80;
const int chargePrice = 10;
const int evacuatorMinPrice = chargePrice + 40;

const unsigned int X = 2 + ((objCount + 1) * 4) + 2 + 5 + 1;               // размер экрана по X   
const unsigned int Y = 2 + ((objCount * 2) - 1) + 3 + objCount + 2;     // размер экрана по Y

#define clear() printf("\033[H\033[J")       // очистить всю консоль
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))     // переместить "курсор" на координату (x;y) (консоль)
#define coutxy(x,y,text) printf("\033[%d;%dH%s", (y), (x) , (text))     // вывести текст в консоль на координате (x;y) 
#define coutxyBot(x,y,text) printf("\033[%d;%dH%s", (y + 4 + ((objCount * 2) - 1) + 3), (x + 2) , (text))   // вывести текст в консоль на координате (x;y) в нижней части игрового экрана

    // Функции //

int random(int from, int to) {
    return (rand() % (to + 1 - from)) + from;
}

int orRand(int num1, int num2) {
    if (random(0, 1)) return num1;
    else return num2;
}

// очистить консоль С координаты (y) ДО конца в нижней части игрового экрана
void clearBotY(int y, int X)
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
    if (N2 == 0) return 1;
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
void makeFrame(const int X)
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

void fillQuestionsNumbers(int questionNum[])
{
    for (int i = 0; i < objCount; i++)
    {
        questionNum[i] = random(1, 3);
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

// узнаёт id объекта ипользуя его индекс
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
    for (int i = 1; true; i++)
    {
        if (curPos + i < objCount && objDistanse[curPos][curPos + i] <= charge) return true;
        if (curPos - i >= 0 && objDistanse[curPos][curPos - i] <= charge) return true;
        if (curPos - i < 0 && curPos + i >= objCount) return false;
    }
}

int nearestChargeStationId(char objIndex[][objMaxIndexLength], float objDistanse[][objCount], int curPos)
{
    float distance = -1;
    for (int i = 0; i < objCount; i++)
    {
        if (canCharge(i, objIndex) && (distance == -1 || distance > objDistanse[curPos][i])) distance = objDistanse[curPos][i];
    }
    return distance;
}

bool namePrice(int price, int Xx)
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    char answer[8];
    clearBotY(1, Xx);
    gotoxyBot(1, 1); cout << "Evakuвcija cita punktв maksвs " << price << "BP.";
    gotoxyBot(1, 2); cout << "Vai jыs esat apmierinвti ar cenu?(Y / N)";
    while (1) {
        gotoxyBot(1, 4);
        cin >> answer;
        if (answer[0] == 'Y')
        {
            return true;
        }
        else if (answer[0] == 'N') return false;
        else { clearBotY(4, Xx); coutxyBot(1, 4 + 2, "Nepielaujamie dati"); }
    }

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

int maxLengthInCharArray(char objNames[][objMaxNameLength])
{
    int outMax = 0;
    for (int i = 0; i < objCount; i++)
    {
        for (int j = 0; ; j++)
        {
            if ((int)objNames[i][j] == 0) { if (j > outMax) outMax = j; break; }
        }
    }
    return outMax;
}

// ????????? ?????????? ?????? ? ?????????? ??????? ?? ??????
void updInfo(float charge, int points, int X, int weather)
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 1);
    cout << "[";
    for (int i = 0; i < X - (((objCount + 1) * 4) + 3 + 1) - 5; i++)
    {
        if (i <= ((charge / (int)maxCharge)) * (X - (((objCount + 1) * 4) + 3 + 1) - 5)) cout << "|";
        else cout << " ";
    }
    cout << "]";

    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 3);
    cout << "                                      ";
    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 3);
    cout << "Lвdiтр: " << 100 * (int)charge / (int)maxCharge << "% (" << (int)charge << "km) " << "(" << (int)(charge / maxCharge * maxChargeRain) << "km negaisв)";

    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 5);
    cout << "Bonuspunkti: " << setw(length(points)) << points << "     ";

    gotoxyTop(((objCount + 1) * 4) + 3 + 1, 7);
    cout << "Laikapstвkпi: ";
    if (weather == 1) cout << setw(5) << "skaidrs laiks";
    else cout << setw(5) << "lietus       ";

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void coutMiddled(int x, int y, const char str[])
{
    int leng = 0;
    int i = 0;
    while (true) {
        if (str[i] != (char)0) leng = leng + 1;
        else break;
        i = i + 1;
    }
    gotoxy(x - (leng / 2), y);
    cout << str;
}


// ????????? ???????? ???????

void A1();
void A2(int[], float[], float[], char[][objMaxNameLength], char[][objMaxIndexLength]);
void A3(int[], float[][objCount], char[][objMaxIndexLength], int, float, int);
void A3b(int[], float[][objCount], char[][objMaxIndexLength], int, int);
void displayNames(char[][objMaxNameLength], char[][objMaxIndexLength]);
int question(int, char[][objMaxIndexLength], int*, int[], int, int[objCount]);
float distance(int*, float, char[][objMaxIndexLength], float[][objCount], int, int);
void write(const int, int*);
void tryCharge(int, int*, float*, int);
int evacuator(int*, int, char[][objMaxIndexLength], float[][objCount], int, int);
void endGame(bool, int, int, int, int, int);

int main()
{
    int objType[objCount + 1] = { 0 };
    int answers[3] = { -1 , -1, -1 };
    int curPos = 0;
    int bonusP = 50;
    int inputY = 1;
    int typeOfWrite = -1;
    int weather = rand() % 2;
    int old_weather = 0;
    int counter = 0;
    int questionNum[objCount];
    float objLongitude[objCount + 1] = { 0 };
    float objLatitude[objCount + 1] = { 0 };
    float objDistanse[objCount][objCount] = { 0. };
    float charge = maxCharge;
    float chance = 50;
    char objNames[objCount][objMaxNameLength] = { (char)0 };
    char objIndex[objCount][objMaxIndexLength] = { (char)0 };
    bool doCharge = false;
    int startTime = time(NULL);


    srand(time(NULL));
    setTextColor(15);
    setFontSize(0, 12);

    fillQuestionsNumbers(questionNum);
    A2(objType, objLongitude, objLatitude, objNames, objIndex);
    const int Xx = X + maxLengthInCharArray(objNames);

    A1();
    SetWindow(Xx, Y);
    makeFrame(Xx);
    fillDistanse(objDistanse, objLongitude, objLatitude, objType);
    A3(objType, objDistanse, objIndex, curPos, charge, weather);
    displayNames(objNames, objIndex);


    updInfo(charge, bonusP, Xx, weather);

    write(1, &inputY);
    charge = charge - distance(&curPos, charge, objIndex, objDistanse, inputY, Xx);
    A3(objType, objDistanse, objIndex, curPos, charge, weather);
    clearBotY(1, Xx);
    updInfo(charge, bonusP, Xx, weather);

    while (1)
    {
        if (curPos == objCount - 1) endGame(1, bonusP, bonusP, charge, charge, startTime);

        answers[0] = -1; answers[1] = -1; answers[2] = -1;
        while (true) {
            if (answers[0] == -1) answers[0] = random(0, 2);
            if (answers[1] == -1 || answers[1] == answers[0] || answers[1] == answers[2]) answers[1] = random(0, 2);
            if (answers[2] == -1 || answers[2] == answers[0] || answers[2] == answers[1]) answers[2] = random(0, 2);
            if (answers[0] != -1 && answers[1] != -1 && answers[2] != -1 && answers[0] != answers[1] && answers[1] != answers[2] && answers[2] != answers[0]) break;
        }
        bonusP = bonusP + question(curPos, objIndex, &inputY, answers, Xx, questionNum);
        clearBotY(1, Xx);
        updInfo(charge, bonusP, Xx, weather);

        old_weather = weather;

        if (random(1, 100) < chance) weather = 1;
        else weather = 0;

        if (old_weather == weather) counter = counter + 1;
        else counter = 1;

        if (weather == 1) chance = chance - 15. * (counter / (1. + (abs(50 - chance) / 25.)));
        else chance = chance + 30. * (counter / (1. + (abs(50 - chance) / 25.)));

        updInfo(charge, bonusP, Xx, weather);

        if (canCharge(curPos, objIndex) && bonusP >= chargePrice)
        {
            if (canDrive(objDistanse, curPos, charge))
            {
                write(2, &inputY);
                tryCharge(inputY, &bonusP, &charge, Xx);
                clearBotY(1, Xx);
                updInfo(charge, bonusP, Xx, weather);
                A3(objType, objDistanse, objIndex, curPos, charge, weather);

                write(1, &inputY);
                charge = charge - distance(&curPos, charge, objIndex, objDistanse, inputY, Xx);
                A3(objType, objDistanse, objIndex, curPos, charge, weather);
                clearBotY(1, Xx);
                updInfo(charge, bonusP, Xx, weather);
            }
            else
            {
                write(4, &inputY);
                bonusP = bonusP - chargePrice;
                charge = maxCharge;
                gotoxyBot(1, inputY);
                getch();
                clearBotY(1, Xx);
                updInfo(charge, bonusP, Xx, weather);
                A3(objType, objDistanse, objIndex, curPos, charge, weather);

                write(1, &inputY);
                charge = charge - distance(&curPos, charge, objIndex, objDistanse, inputY, Xx);
                A3(objType, objDistanse, objIndex, curPos, charge, weather);
                clearBotY(1, Xx);
                updInfo(charge, bonusP, Xx, weather);
            }
        }
        else if (!canDrive(objDistanse, curPos, charge) && bonusP >= evacuatorMinPrice)
        {
            A3b(objType, objDistanse, objIndex, curPos, bonusP);
            bonusP = bonusP - evacuator(&curPos, bonusP, objIndex, objDistanse, inputY, Xx);
            charge = maxCharge;
            clearBotY(1, Xx);
            updInfo(charge, bonusP, Xx, weather);
            A3(objType, objDistanse, objIndex, curPos, charge, weather);

        }
        else if (!canDrive(objDistanse, curPos, charge)) endGame(0, bonusP, bonusP, charge, charge, startTime);
        else
        {
            write(1, &inputY);
            charge = charge - distance(&curPos, charge, objIndex, objDistanse, inputY, Xx);
            A3(objType, objDistanse, objIndex, curPos, charge, weather);
            clearBotY(1, Xx);
            updInfo(charge, bonusP, Xx, weather);
        }


    }

}

// ???????? ??????? //
void endGame(bool win, int bonusP, int bonusPall, int charge, int chargeall, int startTime)
{
    int y = 30;
    int x = 3 * y;
    int newTime = time(NULL);
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    clear();
    setFontSize(0, 18);
    SetWindow(x, y);

    if (win)
    {
        coutMiddled(x / 2, 3, "Jыs uzvarejвt! :)");
        Beep(130, 100);
        Beep(262, 100);
        Beep(330, 100);
        Beep(392, 100);
        Beep(523, 100);
        Beep(660, 100);
        Beep(784, 300);
        Beep(660, 300);
        Beep(146, 100);
        Beep(262, 100);
        Beep(311, 100);
        Beep(415, 100);
        Beep(523, 100);
        Beep(622, 100);
        Beep(831, 300);
        Beep(622, 300);
        Beep(155, 100);
        Beep(294, 100);
        Beep(349, 100);
        Beep(466, 100);
        Beep(588, 100);
        Beep(699, 100);
        Beep(933, 300);
        Beep(933, 100);
        Beep(933, 100);
        Beep(933, 100);
        Beep(1047, 400);
    }
    else
    {
        coutMiddled(x / 2, 3, "Jыs zaudejвt! :(");
    }

    coutxy(2, 6, "Liels paldies jums, ka spзlзjвt mыsu spзli! ");
    coutxy(2, 8, "Statistika:");
    coutxy(2, 9,  "Palika bonuspunktu:............."); cout << bonusP << "BP";
    coutxy(2, 10, "Palika lвdiņa:.................."); cout << 100. * charge / maxCharge << "% (" << charge << "km)";
    coutxy(2, 11 "Laiks pavadоts spзlз:..........."); cout << (newTime - startTime) / 60 << "min " << (newTime - startTime) % 60 << "sek";
    char name[32];
    coutxy(2, 13, "Ievadiet savu vвrdu (lоdz 16 simboliem) - "); cin >> name;


    char wNames[10][16];
    int Stats[10][3];

    FILE* file;
    fopen_s(&file, "Program_Info/TOP.txt", "rt");
    if (!file) return;
    fscanf_s(file, "1( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n2( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n3( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n4( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n5( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n6( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n7( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n8( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n9( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)\n10( NAME:\x22%[^\x22]\x22 BP:\x22%i\x22 TIMEM:\x22%i\x22 TIMES:\x22%i\x22)", &wNames[0], 16, &Stats[0][0], &Stats[0][1], &Stats[0][2], &wNames[1], 16, &Stats[1][0], &Stats[1][1], &Stats[1][2], &wNames[2], 16, &Stats[2][0], &Stats[2][1], &Stats[2][2], &wNames[3], 16, &Stats[3][0], &Stats[3][1], &Stats[3][2], &wNames[4], 16, &Stats[4][0], &Stats[4][1], &Stats[4][2], &wNames[5], 16, &Stats[5][0], &Stats[5][1], &Stats[5][2], &wNames[6], 16, &Stats[6][0], &Stats[6][1], &Stats[6][2], &wNames[7], 16, &Stats[7][0], &Stats[7][1], &Stats[7][2], &wNames[8], 16, &Stats[8][0], &Stats[8][1], &Stats[8][2], &wNames[9], 16, &Stats[9][0], &Stats[9][1], &Stats[9][2]);
    fclose(file);

    if (win)
    {
        for (int i = 0; i < 10; i++)
        {
            if (bonusP > Stats[i][0])
            {

                for (int j = 9; j > i; j--)
                {
                    for (int g = 0; g < 16; g++) wNames[j][g] = wNames[j - 1][g];
                    Stats[j][0] = Stats[j - 1][0];
                    Stats[j][1] = Stats[j - 1][1];
                    Stats[j][2] = Stats[j - 1][2];
                }
                for (int g = 0; g < 16; g++) wNames[i][g] = name[g];
                Stats[i][0] = bonusP;
                Stats[i][1] = (newTime - startTime) / 60;
                Stats[i][2] = (newTime - startTime) % 60;

                break;
            }
        }

        fopen_s(&file, "Program_Info/TOP.txt", "w");
        if (!file) return;

        for (int i = 0; i < 10; i++)
        {
            fprintf(file, "%i(NAME:\x0022%s\x0022 BP:\x0022%i\x0022 TIMEM:\x0022%i\x0022 TIMES:\x0022%i\x0022)\n", i + 1, wNames[i], Stats[i][0], Stats[i][1], Stats[i][2]);
        }


        fclose(file);
    }


    coutxy(2, 15, "Nospiediet Enter ,lai redzзtu labвko 10 spзļu topu"); getch();

    clear();
    for (int i = 0; i < 10; i++)
    {
        gotoxy(2, 2 + i); cout << i + 1 << " vieta"; gotoxy(12 + 3, 2 + i); cout << wNames[i]; gotoxy(12 + 18 + 3, 2 + i); cout << Stats[i][0]; cout << "\t"; cout << Stats[i][1]; cout << "min\t"; cout << Stats[i][2]; cout << "sek";
    }

    coutxy(2, 14, "Nospiediet Enter"); getch();
}

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
    char finder[12] = { };
    int id = 0;
    bool reading[8] = { false };
    bool isFract = false;
    int counter = 0;

    FILE* file;
    fopen_s(&file, "Open_Info/2.txt", "rt");
    if (!file) return;
    while (fgets(str, 2, file) && id <= objCount)
    {
        for (int i = 0; i < 11; i++)
        {
            finder[i] = finder[i + 1];
        }
        finder[11] = str[0];

        //cout << finder << endl;Sleep(100);

        if (reading[0]) { id = id * powInt(10, counter) + ((int)finder[3] - (int)'0'); counter = counter + 1;  if (finder[4] == '\x22') { reading[0] = false; counter = 0; } }
        else if (finder[0] == 'I' && finder[1] == 'D' && finder[2] == ':' && finder[3] == '"') { reading[0] = true; id = 0; }

        if (reading[1]) { objType[id] = ((int)finder[5] - (int)'0'); counter = counter + 1; if (finder[6] == '\x22') { reading[1] = false; counter = 0; } }
        else if (finder[0] == 'T' && finder[1] == 'Y' && finder[2] == 'P' && finder[3] == 'E' && finder[4] == ':' && finder[5] == '"')reading[1] = true;

        if (reading[2]) {
            if (finder[5] == ',' || finder[5] == '.') { isFract = true; counter = 1; continue; }
            if (!isFract) {
                objLongitude[id] = objLongitude[id] * powInt(10, counter) + (float)finder[5] - (float)'0';
                counter = counter + 1;
            }
            else {
                objLongitude[id] = objLongitude[id] + ((float)finder[5] - (float)'0') / powInt(10, counter);
                counter = counter + 1;
            }
            if (finder[6] == '\x22') { reading[2] = false; counter = 0; isFract = false; }
        }
        else if (finder[0] == 'L' && finder[1] == 'O' && finder[2] == 'N' && finder[3] == 'T' && finder[4] == ':' && finder[5] == '"')reading[2] = true;

        if (reading[3]) {
            if (finder[5] == ',' || finder[5] == '.') { isFract = true; counter = 1; continue; }
            if (!isFract) {
                objLatitude[id] = objLatitude[id] * powInt(10, counter) + (float)finder[5] - (float)'0';
                counter = counter + 1;
            }
            else {
                objLatitude[id] = objLatitude[id] + ((float)finder[5] - (float)'0') / powInt(10, counter);
                counter = counter + 1;
            }
            if (finder[6] == '\x22') { reading[3] = false; counter = 0; isFract = false; }
        }
        else if (finder[0] == 'L' && finder[1] == 'A' && finder[2] == 'T' && finder[3] == 'T' && finder[4] == ':' && finder[5] == '"')reading[3] = true;

        if (reading[4]) { objNames[id][counter] = finder[5]; counter = counter + 1; if (finder[6] == '\x22') { reading[4] = false; counter = 0; } }
        else if (finder[0] == 'N' && finder[1] == 'A' && finder[2] == 'M' && finder[3] == 'E' && finder[4] == ':' && finder[5] == '"')reading[4] = true;

        if (reading[5]) { objIndex[id][counter] = finder[4]; counter = counter + 1; if (finder[5] == '\x22') { reading[5] = false; counter = 0; } }
        else if (finder[0] == 'I' && finder[1] == 'N' && finder[2] == 'D' && finder[3] == ':' && finder[4] == '"')reading[5] = true;

        //cout << id << "\t" << objIndex[id] << "\t" << finder << endl; Sleep(10);

    }
    fclose(file);

    //???/???? ???????? 1/0
    for (int i = 0; i < objCount && 0; i++)
    {
        if (objType[i] == 0) continue;
        cout << "\n" << objType[i] << "\t" << objLongitude[i] << "\t" << objLatitude[i] << "\t" << objNames[i] << "\t" << objIndex[i];
    }

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);

}

void A3(int objType[], float objDistanse[][objCount], char objIndex[][objMaxIndexLength], int curPos, float charge, int weather)
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

            if (((ix != -1) && (iy == -1)) && (((charge - objDistanse[ix][curPos] > 0) && weather == 1) || (((charge / maxCharge * maxChargeRain) - objDistanse[ix][curPos] > 0) && weather == 0)))  setTextColor(10);
            if (iy == -1) { if (ix == curPos)  setTextColor(11); cout << setw(4) << objIndex[ix]; if (ix == curPos || (((ix != -1) && (iy == -1)) && (((charge - objDistanse[ix][curPos] > 0) && weather == 1) || (((charge / maxCharge * maxChargeRain) - objDistanse[ix][curPos] > 0) && weather == 0))))  setTextColor(15); continue; }

            if (((iy != -1) && (ix == -1)) && (((charge - objDistanse[iy][curPos] > 0) && weather == 1) || (((charge / maxCharge * maxChargeRain) - objDistanse[iy][curPos] > 0) && weather == 0)))  setTextColor(10);
            if (ix == -1) { if (iy == curPos)  setTextColor(11); cout << setw(4) << objIndex[iy]; if (iy == curPos || (((iy != -1) && (ix == -1)) && (((charge - objDistanse[iy][curPos] > 0) && weather == 1) || (((charge / maxCharge * maxChargeRain) - objDistanse[iy][curPos] > 0) && weather == 0))))  setTextColor(15); continue; }

            if ((charge - objDistanse[curPos][iy] > 0 && charge - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos) && weather == 1) || ((charge / maxCharge * maxChargeRain) - objDistanse[curPos][iy] > 0 && (charge / maxCharge * maxChargeRain) - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos) && weather == 0))  setTextColor(10);
            cout << setw(4) << (int)objDistanse[ix][iy];
            if ((charge - objDistanse[curPos][iy] > 0 && charge - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos) && weather == 1) || ((charge / maxCharge * maxChargeRain) - objDistanse[curPos][iy] > 0 && (charge / maxCharge * maxChargeRain) - objDistanse[ix][curPos] > 0 && (iy == curPos || ix == curPos) && weather == 0))  setTextColor(15);
        }
        gotoxyTop(1, 1 + (2 * (iy + 2)));
    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void A3b(int objType[], float objDistanse[][objCount], char objIndex[][objMaxIndexLength], int curPos, int bonusP)
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
            if ((canCharge(ix, objIndex) || canCharge(iy, objIndex)) && bonusP - (int)(((objDistanse[curPos][iy] / 80) * chargePrice) + evacuatorMinPrice - chargePrice) >= 0 && bonusP - (int)(((objDistanse[curPos][ix] / 80) * chargePrice) + evacuatorMinPrice - chargePrice) >= 0 && (iy == curPos || ix == curPos))  setTextColor(13);
            cout << setw(4) << (int)objDistanse[ix][iy];
            if ((canCharge(ix, objIndex) || canCharge(iy, objIndex)) && bonusP - (int)(((objDistanse[curPos][iy] / 80) * chargePrice) + evacuatorMinPrice - chargePrice) >= 0 && bonusP - (int)(((objDistanse[curPos][ix] / 80) * chargePrice) + evacuatorMinPrice - chargePrice) >= 0 && (iy == curPos || ix == curPos))  setTextColor(15);
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
    int y = 2 + 2 + 5;
    gotoxyTop(x, y);
    coutLV8("99 - uzpildes stacija");
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

int question(int curPos, char objIndex[][objMaxIndexLength], int* inputY, int answers[], int Xx, int questionNum[objCount])
{
    if (questionNum[curPos] == -1) return 0;

    setlocale(LC_ALL, "lv_LV.UTF-8");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    int points = 10;
    char answer[8];
    char str[3];
    char finder[12] = { };
    bool reading[8] = { false };
    bool idFinded = false;
    char charAnswers[3][64] = { 0 };
    int counter = 0;
    bool doQuestionsScan = false;

    FILE* file;
    fopen_s(&file, "Open_Info/questions.txt", "rt");
    if (!file) return -1;

    *inputY = 1;
    while (fgets(str, 2, file))
    {
        for (int i = 0; i < 11; i++)
        {
            finder[i] = finder[i + 1];
        }
        finder[11] = str[0];


        if (finder[0] == 'I' && finder[1] == 'N' && finder[2] == 'D' && finder[3] == ':' && finder[4] == '"' && finder[5] == objIndex[curPos][0] && finder[6] == objIndex[curPos][1] && finder[7] == '"') idFinded = true;



        if (idFinded)
        {
            //cout << idFinded << "\t" << finder << endl; Sleep(10);

            if (reading[0]) { cout << finder[7]; if (finder[7] == ' ' && counter >= Xx - 4 - 10) { gotoxyBot(1, *inputY + 1); counter = 0; *inputY = *inputY + 1; } counter = counter + 1; if (finder[8] == '\x22') { reading[0] = false; counter = 0; *inputY = *inputY + 2; doQuestionsScan = true; } }
            else if (finder[0] == (char)((int)questionNum[curPos] + (int)'0') && finder[1] == '_' && finder[2] == 'J' && finder[3] == 'A' && finder[4] == 'U' && finder[5] == 'T' && finder[6] == ':' && finder[7] == '"') { reading[0] = true; gotoxyBot(1, *inputY); }


            if (reading[1]) { charAnswers[0][counter] = finder[6]; counter = counter + 1; if (finder[7] == '\x22') { reading[1] = false; counter = 0; } }
            else if (doQuestionsScan && finder[0] == 'P' && finder[1] == '_' && finder[2] == 'A' && finder[3] == 'T' && finder[4] == 'B' && finder[5] == ':' && finder[6] == '"')reading[1] = true;


            if (reading[2]) { charAnswers[1][counter] = finder[6]; counter = counter + 1; if (finder[7] == '\x22') { reading[2] = false; counter = 0; } }
            else if (doQuestionsScan && finder[0] == '2' && finder[1] == '_' && finder[2] == 'A' && finder[3] == 'T' && finder[4] == 'B' && finder[5] == ':' && finder[6] == '"')reading[2] = true;;


            if (reading[3]) { charAnswers[2][counter] = finder[6]; counter = counter + 1; if (finder[7] == '\x22') { reading[3] = false; counter = 0; } }
            else if (doQuestionsScan && finder[0] == '3' && finder[1] == '_' && finder[2] == 'A' && finder[3] == 'T' && finder[4] == 'B' && finder[5] == ':' && finder[6] == '"')reading[3] = true;

            if (doQuestionsScan && finder[0] != (char)((int)questionNum[curPos] + (int)'0') && finder[1] == '_' && finder[2] == 'J' && finder[3] == 'A' && finder[4] == 'U' && finder[5] == 'T' && finder[6] == ':' && finder[7] == '"') { fclose(file); }
        }


    }
    fclose(file);

    gotoxyBot(1, *inputY);
    cout << "a: ";
    cout << charAnswers[answers[0]];

    gotoxyBot(1, *inputY + 2);
    cout << "b: ";
    cout << charAnswers[answers[1]];

    gotoxyBot(1, *inputY + 4);
    cout << "c: ";
    cout << charAnswers[answers[2]];

    *inputY = *inputY + 6;

    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);

    while (1) {

        gotoxyBot(1, *inputY);
        cin >> answer;
        if ((answer[0] == 'a' && answers[0] == 0) || (answer[0] == 'b' && answers[1] == 0) || (answer[0] == 'c' && answers[2] == 0))
        {
            questionNum[curPos] = -1;
            return points;
        }
        else if (points > 0) points = points - 5;
        if (answer[0] == 'a' || answer[0] == 'b' || answer[0] == 'c') { clearBotY(*inputY, Xx); coutxyBot(1, *inputY + 2, "Nepareizi, pamзмiniet velreiz!"); }
        if (answer[0] != 'a' && answer[0] != 'b' && answer[0] != 'c') { clearBotY(*inputY, Xx); coutxyBot(1, *inputY + 2, "Nepieпaujamie dati"); }
    }

    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

float distance(int* curPos, float charge, char objIndex[][objMaxIndexLength], float objDistanse[][objCount], int inputY, int Xx)
{
    gotoxyBot(2, 2);
    char answer[8];
    int id;
    int bufferCurPos = *curPos;

    while (1) {
        gotoxyBot(1, inputY);
        cin >> answer;
        id = findIndexId(answer, objIndex);
        if (id == -1) { clearBotY(inputY, Xx); coutxyBot(1, inputY + 2, "Nepielaujamie dati"); continue; }
        if (charge - objDistanse[bufferCurPos][id] > 0 && bufferCurPos != id)
        {
            *curPos = id;
            return objDistanse[bufferCurPos][id];
        }
        else { clearBotY(inputY, Xx); coutxyBot(1, inputY + 2, "Nepieпaujamie dati"); }
    }
}

void write(const int type, int* inputY)
{
    setlocale(LC_ALL, "Latvian");
    SetConsoleCP(1257); SetConsoleOutputCP(1257);
    switch (type)
    {
    case 1:
        gotoxyBot(1, 1);
        cout << "Izvзlaties nвkamвs vietas indeksu.";
        *inputY = 3;
        break;
    case 2:
        gotoxyBot(1, 1);
        cout << "Vai gribat uzlвdзt maроnas akumulatoru? (Y / N) (" << chargePrice << "BP)";
        *inputY = 3;
        break;
    case 3:
        gotoxyBot(1, 1);
        cout << "??????.??????!??????????????! (3)"; //??? ?????????????? ??? ???
        *inputY = 3;
        break;
    case 4:
        gotoxyBot(1, 1); cout << "Jыsu maроnas akumulators izlвdзjas, bet jums ir nauda ,lai uzlвdзtos.";
        gotoxyBot(1, 2); cout << "Nospiediet Enter, lai uzlвdзtos.";
        *inputY = 4;
        break;
    case 5:
        gotoxyBot(1, 1); cout << "Jыsu maроnas akumulators izlвdзjas, bet jums ir nauda ,lai izsauktu evakuatoru.";
        gotoxyBot(1, 2); cout << "Ievadiet uzlвdзрanas stacijas indeksu.";
        *inputY = 4;
        break;
    }
    setlocale(LC_ALL, "C");
    SetConsoleCP(866); SetConsoleOutputCP(866);
}

void tryCharge(int inputY, int* bonusP, float* charge, int Xx)
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
        else { clearBotY(inputY, Xx); coutxyBot(1, inputY + 2, "Nepieпaujamie dati"); }
    }
}

int evacuator(int* curPos, int bonusP, char objIndex[][objMaxIndexLength], float objDistanse[][objCount], int inputY, int Xx)
{
    gotoxyBot(2, 2);
    char answer[8];
    int id = -1;
    int bufferCurPos = *curPos;

    while (1) {
        write(5, &inputY);
        gotoxyBot(1, inputY);
        cin >> answer;
        id = findIndexId(answer, objIndex);
        if (id == -1) { clearBotY(1, Xx); coutxyBot(1, inputY + 2, "Nepieпaujamie dati"); continue; }
        if (id == *curPos) { clearBotY(1, Xx); coutxyBot(1, inputY + 2, "Nepieпaujamie dati"); continue; }
        if ((int)objIndex[id][0] >= (int)'0' && (int)objIndex[id][0] <= (int)'9' && (int)objIndex[id][1] >= (int)'0' && (int)objIndex[id][1] <= (int)'9')
        {
            if (id == nearestChargeStationId(objIndex, objDistanse, *curPos)) { if (namePrice(evacuatorMinPrice, Xx)) { *curPos = id; return evacuatorMinPrice; } }
            else if (objDistanse[*curPos][id] < 80) { if (namePrice(evacuatorMinPrice, Xx)) { *curPos = id; return evacuatorMinPrice; } }
            else if ((int)(((objDistanse[*curPos][id] / 80) * chargePrice) + evacuatorMinPrice - chargePrice) <= bonusP && namePrice((int)(((objDistanse[*curPos][id] / 80) * chargePrice) + evacuatorMinPrice - chargePrice), Xx)) { *curPos = id; return (int)(((objDistanse[bufferCurPos][id] / 80) * chargePrice) + evacuatorMinPrice - chargePrice); }
        }
        clearBotY(1, Xx); coutxyBot(1, inputY + 2, "Nepieпaujamie dati"); continue;
    }
}
