#ifndef FISH_H
#define FISH_H
#include <iostream>
#include <array>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "Ship.h"
#include "globals.h"
#include "log_folder.h"
/**
 * @class Fish
 * @brief Класс рыбной точки
 */
class Fish
{
    public:
        /**
        * @brief Конструктор по умолчанию
        * @note Для задания используется setPoint
        */
        Fish();

        /**
        * @brief Геттер для переменной _x
        */
        size_t getX();

        /**
        * @brief Геттер для переменной _y
        */
        size_t getY();
        /**
        * @brief Геттер для количества рыбы в клетке Fish
        */
        uint8_t getFish();

        /**
        * @brief Геттер для состояния клетки
        * @return {"UNACTIVE", "LOCK", "ACTIVE"}
        */
        FISHPOINT_ST getState();

        /**
        * @brief Возвращает признак доступности точки для рыбалки
        * @note Когда точка находится в состоянии  LOCK то она является не доступной для рыбалки. 
        */
        bool getAvailable();

        /**
        * @brief Функция разблокировки точки из состояния LOCK в UNACTIVE
        * @param tick Значение тика
        * @note Если прошло время, необходимое для генерации, то точка переходит в состояние UNACTIVE
        */
        void try_unlock(ui counter);

        /**
        * @brief Функция разблокировки точки из состояния LOCK 
        * @param tick Значение тика
        * @note Если прошло время, необходимое для генерации, то точка переходит в состояние UNACTIVE
        */
        // void try_active(ui counter);
        /**
        * @brief Функция блокировки точки из состояния ACTIVE 
        * @param tick Значение тика
        * @note Если вся рыба из точки выловлена, а время для генерации новой не подошло, то точка блокируется - LOCK
        */
        void try_lock(ui counter);


        /**
        * @brief Изменение состояния точки с учетом текущего тика и количества рыбы в точке 
        * @param tick Значение тика
        * @note Точку могут занимать 2 и более лодки. Когда 1 лодка производит улов, точка может изменить свое состояние.
        * Поэтому изменение состояния точки производится как до улова (по времени), так и после (по количеству рыбы) 
        */
        void changeState(ui tick);

        /**
        * @brief Генерирование рыбы в точке
        * @param tick Значение тика
        * @note Запоминается тик, в котором произошла генерация рыбы 
        */
        void generateFish(ui tick);
        
        /**
        * @brief Присваивание точке информации о ее положении, файле логгирования
        * @param x Координата x
        * @param y Координата у
        */
        void setPoint (size_t x, size_t y);

        /**
        * @brief Добавление в точку лодки
        * @param ship Адрес лодки
        * @note Используется для логов
        */
        void addShip(Ship* ship);

        /**
        * @brief Улов рыбы
        * @param fish_count Количество выловленной рыбы
        * @note Выловленная рыба вычитается из общего количества
        */
        void killFish(ui fish_count);

        /**
        * @brief Удаление адреса лодки, которая занимает точку, из вектора лодок
        * @param ship Адрес лодки
        * @note Используется для логов
        */
        void remove_ship(Ship *ship);

        /**
        * @brief Запись логов точки
        * @param tick Значение тика
        * @note Используется для логов
        */
        void write_logs(ui tick);

        /**
        * @brief Переопределение оператора вывода для точки. Выводится 0
        */
        friend ostream &operator <<(ostream &os, unique_ptr<Fish>&fp);
        virtual ~Fish();


    protected:

    private:
        /** 
        * @brief Координата х
        */
        size_t _x;

        /** 
        * @brief Координата у
        */
        size_t _y;

        /** 
        * @brief Тик, при котором произошла генерация рыбы
        */
        ui _local_tick = 0;

        /** 
        * @brief Состояние точки
        */
        FISHPOINT_ST _state = FISHPOINT_ST::UNACTIVE;

        // /** 
        // * @brief Состояние точки
        // */
        // array<Ship, FIELD_SIZE> *_pships = nullptr;

        /** 
        * @brief Количество рыбы в точки
        */
        uint8_t _fish_count = 0;

        /** 
        * @brief Адреса кораблей в точке
        */
        vector <Ship*> _currents_ships;

        /** 
        * @brief Директория для логов
        */
        string _logdir = "logs/fish/";

        /** 
        * @brief Название файла для лога точки
        * При инициализации название файла меняется на координату точки
        */
        string _filepath = "empty";
};

#endif // FISHPOINT_H