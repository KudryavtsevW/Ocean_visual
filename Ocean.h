#ifndef OCEAN_H
#define OCEAN_H
#include <thread>
#include <omp.h>
#include <chrono>
#include "Fish.h"
#include "Ship.h"
#include "globals.h"
#include "fishmatrix.h"
class Ocean
{
    public:
        /**
        * @brief Конструктор по умолчанию
        * @note Для задания используется ocean_init
        */
        Ocean();

        /**
        * @brief Инициализация системы точек и лодок
        * @param pfish Указатель на мтарицу 
        * @param pship Указатель на вектор лодок
        * @note Для взаимодействия лодок и точек используется класс Ocean. Сохраняем указатели на точки и лодки
        */
        void ocean_init(FishMatrix* pfish, std::vector<Ship>* pship);
        virtual ~Ocean();
        

        /**
        * @brief Вывод матрицы точек и вектора лодок
        */
        void print_ocean();

        /**
        * @brief Симуляция процесса ловли. Основная функция
        */
        void simulation();

        /**
        * @brief Перемещение лодки по точкам
        */
        void rand_movement();

        /**
        * @brief Покидание лодкой точки
        * @param Указатель на лодку
        */
        void leave_fishpoint(Ship *ship);

        /**
        * @brief Занятие лодкой точки
        * @param ship Указатель на лодку 
        */
        void set_fishpoint(Ship *ship);

        /**
        * @brief Функция обработки состояний лодки
        * @param start Начало обработки вектора лодок 
        * @param end Конец обработки вектора лодок
        * @note Используется потоковая обработка. Массив разбивается на равные части, которые обрабатываются параллельно
        */
        void action(size_t start, size_t end);

        /**
        * @brief Вывод статистики по заданию
        */
        void print_statistics();

        /**
        * @brief Удаление лодки, которые перешли в состояние DELETED
        */
        void remove_ships();

    protected:

    private:
        /**
        * @brief Указатель на матрицу точек с рыбой
        */
        FishMatrix *_pFishpoint = nullptr;

        /**
        * @brief Указатель на вектор лодок
        */
        vector<Ship> *_pShips = nullptr;

        /**
        * @brief Счетчик тиков
        */
        unsigned int _counter = 0;

        /**
        * @brief Тик первого удаления
        */
        unsigned int _first_remove = 0;

        /**
        * @brief Тик последнего удаления
        */
        unsigned int _last_remove = 0;

        /**
        * @brief Файл логов
        */
        string _filepath = "statistic.xml";

};

#endif // OCEAN_H
