
#include <iostream>

using namespace std;

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

int main()
{
    A1();
}
