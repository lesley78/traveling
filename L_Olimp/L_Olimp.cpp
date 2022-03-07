
#include <iostream>

using namespace std;

int main()
{

    char str[9];

    FILE* file;

    fopen_s(&file, "1.txt", "r");



    while (fgets(str, 8, file))
    {
        cout << str;
    }
    fclose(file);

}
