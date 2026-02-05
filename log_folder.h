#ifndef LOG_FOLDER_H_INCLUDED
#define LOG_FOLDER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <direct.h>    // для _mkdir на Windows
    #include <windows.h>
    #include <io.h>
    #define MKDIR(path) _mkdir(path)
    #define ACCESS _access
#else
#include <sys/stat.h>  // для mkdir на Linux/Mac
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define ACCESS access
#endif

/**
 * @brief Создает все директории в пути до файла
 * @param path Полный путь к файлу (включая имя файла)
 * @return true если директории созданы или уже существуют
 */
static bool createDirectoriesForFile(const std::string& filePath) {
    // Находим позицию последнего разделителя
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return true; // Нет директорий, только имя файла
    }

    // Извлекаем путь директории (без имени файла)
    std::string directoryPath = filePath.substr(0, lastSlash);
    if (directoryPath.empty()) {
        return true; // Файл в корневой директории
    }

    #ifdef _WIN32
        const char separator = '\\';
        // Обработка диска C: и т.д.
        if (directoryPath.size() >= 2 && directoryPath[1] == ':') {
            // Проверяем существование диска
            if (ACCESS(directoryPath.substr(0, 2).c_str(), 0) != 0) {
                std::cerr << "Диск не существует: " << directoryPath.substr(0, 2) << std::endl;
                return false;
            }
        }
    // #else
    //     // const char separator = '/';
    #endif

    // Создаем директории рекурсивно
    size_t pos = 0;
    std::string dir;

    while (pos <= directoryPath.length()) {
        // Находим следующий разделитель
        size_t nextPos = directoryPath.find_first_of("/\\", pos);

        if (nextPos == std::string::npos) {
            dir = directoryPath; // Последняя директория
            pos = directoryPath.length() + 1;
        } else {
            dir = directoryPath.substr(0, nextPos);
            pos = nextPos + 1;
        }

        // Пропускаем пустые директории (например, начало с / на Unix)
        if (dir.empty() || dir == ".") {
            continue;
        }

        #ifdef _WIN32
            // На Windows добавляем :\ для первой части (C:)
            if (dir.size() == 2 && dir[1] == ':') {
                // Это уже проверяли выше, пропускаем
                continue;
            }
        #endif

        // Пытаемся создать директорию
        if (MKDIR(dir.c_str()) != 0) {
            // Проверяем, существует ли уже директория
            #ifdef _WIN32
                if (GetLastError() != ERROR_ALREADY_EXISTS) {
                    std::cerr << "Не удалось создать директорию: " << dir << std::endl;
                    return false;
                }
            #else
                if (errno != EEXIST) {
                    std::cerr << "Не удалось создать директорию: " << dir << std::endl;
                    return false;
                }
            #endif
        }

        // Если это был последний разделитель
        if (nextPos == std::string::npos) {
            break;
        }
    }

    return true;
}

/**
 * @brief Записывает в файл сообщение. Если файла нет то создает его
 * @param filePath Полный путь к файлу (включая имя файла)
 * @param content Сообщение для записи в файл
 * @return true если удалось записать сообщение
 */
static bool createFileWithPath(const std::string& filePath, const string &content,
                       bool overwrite = false) {

    // Проверяем, существует ли уже файл (если не нужно перезаписывать)
    if (!overwrite && ACCESS(filePath.c_str(), 0) == 0) {
//        std::cerr << "Файл уже существует: " << filePath << std::endl;
//        return true;
    }

    // Создаем директории для файла
    if (!createDirectoriesForFile(filePath)) {
        std::cerr << "Не удалось создать директории для: " << filePath << std::endl;
        return false;
    }

    // Создаем файл
    std::ofstream file(filePath, ios::app);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать файл: " << filePath << std::endl;
        return false;
    }

    // Записываем содержимое
    if (!content.empty()) {
        file << content;
    }
//
    file.close();
//    std::cout << "Файл успешно создан: " << filePath << std::endl;
    return true;
}


#endif // LOG_FOLDER_H_INCLUDED
