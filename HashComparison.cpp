#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

#define BUFFER_SIZE (64 * 1024)

char* get_hash(const char* filepath, char* hash, int hash_size) {
    if (!hash || hash_size < 2) {
        return NULL;
    }

    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filepath);
        return NULL;
    }

    int hash_int = 0;
    char* buffer = (char*)malloc(BUFFER_SIZE*sizeof(char));
    if (buffer == NULL) {
        printf("Ошибка выделения памяти для буфера\n");
        fclose(file);
        return NULL;
    }
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            hash_int ^= buffer[i];
        }
        printf_s("%zu\n", bytes_read);
    }

    free(buffer);
    fclose(file);

    snprintf(hash, hash_size, "%d", hash_int);
    return hash;
}

void hash_menu() {
    char filepath[512];
    char hash[32];

    printf_s("Подсчет хеша файла...\n");
    printf_s("Введите абсолютный путь файла: ");
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\n")] = 0;

    printf_s("Полученный хеш %s", get_hash(filepath, hash, sizeof(hash)));
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
