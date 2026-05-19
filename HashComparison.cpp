#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    printf("Приветствую!\n");

    return 0;
}
