#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include <windows.h>


#define BUFFER_SIZE (64 * 1024) // размер буффера (64 кб)
#define HASH_SIZE 17 // размер хеша

// Множитель для хеш функции: Золотое сечение в 64 битах
const uint64_t GOLDEN_RATIO = 0x9e3779b97f4a7c15ULL; 

// Функция конвертации кодировки. Необходима для указания пути файла с кириллицой
// Полностью взята с интернета
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

/*
* Хеш функция
* @param filepath   путь к файлу
* @param hash_str   хеш массив
* @return           хеш строка в 16ричном формате (NULL - если ошибка)
*/

char* get_hash(const char* filepath, char* hash_str) {
    if (!hash_str) {
        return NULL;
    }

    char* converted_path = convert_encoding(filepath, 866, 1251);
    if (!converted_path) {
        printf("Ошибка конвертации пути\n");
        return NULL;
    }

    FILE* file = fopen(converted_path, "rb");
    if (file == NULL) {
        printf("Ошибка открытия файла \"%s\" (Убедитесь, что файл по указанному пути существует)\n", filepath);
        return NULL;
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

    printf_s("Расчет хеша файла \"%s\"...\n", filepath);
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            hash ^= buffer[i];
            hash *= GOLDEN_RATIO;
            hash ^= (hash << 5);
            hash += length;
        }
        length += bytes_read;
    }

    hash ^= length;
    hash ^= (length << 32);

    free(buffer);
    fclose(file);

    snprintf(hash_str, HASH_SIZE, "%016llX", hash);
    hash_str[HASH_SIZE - 1] = '\0';
    return hash_str;
}

// Меню вывода хеша файла
void hash_menu() {
    char filepath[512];
    char hash[HASH_SIZE];

    printf_s("Подсчет хеша файла...\n");
    printf_s("Введите абсолютный путь файла: ");
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\n")] = 0;

    printf_s("Полученный хеш: %s\n", get_hash(filepath, hash));
}

// Меню сравнения файлов
void comparison_menu() {
    printf_s("Сравнение файлов...\n");
    char hash1[HASH_SIZE];
    char hash2[HASH_SIZE];
    char filepath1[512];
    char filepath2[512];

    printf_s("Введите абсолютный путь к первому файлу: ");
    fgets(filepath1, sizeof(filepath1), stdin);
    filepath1[strcspn(filepath1, "\n")] = 0;

    printf_s("Введите абсолютный путь ко второму файлу: ");
    fgets(filepath2, sizeof(filepath2), stdin);
    filepath2[strcspn(filepath2, "\n")] = 0;

    char* result = get_hash(filepath1, hash1);
    if (result == NULL) {
        printf_s("Ошибка просчета хеша. Завершение действия...\n");
        return;
    }

    result = get_hash(filepath2, hash2);
    if (result == NULL) {
        printf_s("Ошибка просчета хеша. Завершение действия...\n");
        return;
    }

    if (strcmp(hash1, hash2) == 0) {
        printf_s("Файлы одинаковые (Хеш = %s).\n", hash1);
    }
    else {
        printf_s("Файлы разные.\n");
        printf_s("Хеш первого файла: %s\n", hash1);
        printf_s("Хеш второго файла: %s\n", hash2);
    }
}


int main()
{
    //SetConsoleOutputCP(65001);
    //SetConsoleCP(65001);
    setlocale(LC_ALL, "");
    
    system("chcp 1251 > nul");
    
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
