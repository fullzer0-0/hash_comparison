#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

void hash_menu() {
    printf_s("Подсчет хеша файла...");
}

void comparison_menu() {
    printf_s("Сравнение файла...");
}

int main()
{
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    
    printf_s("Приветствую!\n");

    char command;

    while (1) {
        printf_s("Меню:\n");
        printf_s("1) Вычислить хеш файла:\n");
        printf_s("2) Сравнить файлы:\n");
        printf_s("0) Выход:\n");
        printf_s("Выберите действие: ");
        scanf_s("%c", &command, 1);
        while (getchar() != '\n');

        if (command == '1') {
            hash_menu();
        }
        else if (command == '2') {
            comparison_menu();
        }
        else if (command == '0') {
            printf_s("Выход из программы...");
            return 0;
        }
        else {
            printf_s("Введена неверная команда. Повторите ввод заново.");
        }

        printf_s("\n");
    }



    return 0;
}
