#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include <windows.h>

#define BUFFER_SIZE (64 * 1024)

const uint64_t GOLDEN_RATIO = 0x9e3779b97f4a7c15ULL;

char* convert_encoding(const char* input, UINT from_cp, UINT to_cp) {
    int wide_size = MultiByteToWideChar(from_cp, 0, input, -1, NULL, 0);
    if (wide_size == 0) return NULL;

    wchar_t* wide_str = (wchar_t*)malloc(wide_size * sizeof(wchar_t));
    if (!wide_str) return NULL;

    MultiByteToWideChar(from_cp, 0, input, -1, wide_str, wide_size);

    int result_size = WideCharToMultiByte(to_cp, 0, wide_str, -1, NULL, 0, NULL, NULL);
    if (result_size == 0) {
        free(wide_str);
        return NULL;
    }

    char* result = (char*)malloc(result_size);
    if (result) {
        WideCharToMultiByte(to_cp, 0, wide_str, -1, result, result_size, NULL, NULL);
    }

    free(wide_str);
    return result;
}

char* get_hash(const char* filepath, char* hash_str, int hash_size) {
    if (!hash_str || hash_size < 2) {
        return 0;
    }

    char* converted_path = convert_encoding(filepath, 866, 1251);
    if (!converted_path) {
        printf("Ошибка конвертации пути\n");
        return 0;
    }

    FILE* file = fopen(converted_path, "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filepath);
        return 0;
    }
    free(converted_path);


    uint64_t hash = GOLDEN_RATIO;
    uint64_t length = 0;

    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        printf("Ошибка выделения памяти для буфера\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            hash ^= buffer[i];
            hash *= GOLDEN_RATIO;
            hash ^= (hash << 7);
            hash += length;
        }
        length += bytes_read;
    }

    hash ^= length;
    hash ^= (length << 32);

    free(buffer);
    fclose(file);

    snprintf(hash_str, hash_size, "%016llX", hash);
    hash_str[hash_size - 1] = '\0';
    return hash_str;
}

void hash_menu() {
    char filepath[512];
    char hash[64];

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
        printf_s("1) Вычислить хеш файла\n");
        printf_s("2) Сравнить файлы\n");
        printf_s("0) Выход\n");
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
