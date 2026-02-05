#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <cstdlib>
#include <ctime>
#include "Fish.h"
#include "Ship.h"
#include "Ocean.h"
#include "globals.h"
#include "fishmatrix.h"

void fill_ocean(FishMatrix &fishmap) {
    cout<<"\nИнициализация клеток с рыбами...\n";
    const size_t NUM_THREADS = std::thread::hardware_concurrency();
    omp_set_num_threads(NUM_THREADS);
    fishmap.resize(FIELD_SIZE);
    ui board = FIELD_SIZE / 50;
    if(board == 0) {
        board = 1;

    }
    ui count = 0;
    ui delimetr = FIELD_SIZE;
    if(delimetr > 50) {
        delimetr = 50;
    }
    #pragma omp parallel
    {
        uint8_t thread_id = omp_get_thread_num();
        uint8_t num_threads = omp_get_num_threads();
        size_t chunk_size = FIELD_SIZE / num_threads;
        size_t start = thread_id * chunk_size;
        size_t end = (thread_id == num_threads - 1) ? FIELD_SIZE : (thread_id + 1) * chunk_size;

        for(ui i = start; i < end; ++i) {
            fishmap[i].reserve(FIELD_SIZE);
            for(ui j = 0; j < FIELD_SIZE; ++j) {
                // Для C++11 используем std::unique_ptr<Fish>(new Fish(i, j))
                fishmap[i].emplace_back(std::unique_ptr<Fish>(new Fish()));
                fishmap[i][j]->setPoint(i, j);
            }
            if(i%board == 0) {
                cout.flush();
                count++;
                cout << "\033[" << 4 << ";" << 0 << "H"<<(double(count)/delimetr)*100<<"%";
                cout << "\033[" << 5 << ";" << count << "H"<<"#";
                cout.flush();
            }

        }
    }
    cout << "\033[" << 4 << ";" << 0 << "H"<<(double(count)/delimetr)*100<<"%";
    cout.flush();
    cout<<"\nИнициализация завершена\n";
}

void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        // Предполагаем POSIX (Linux, macOS, BSD)
        std::system("clear");
    #endif
}

void initialize_ship(vector<Ship> &ships) {
    for(ui i = 0; i<FIELD_SIZE; ++i) {
        Ship tmp;
        tmp.setShip(rand()%FIELD_SIZE, rand()%FIELD_SIZE);
        ships.push_back(tmp);
    }
}


void check_memory_requirements() {
    unsigned long long int total_fish = FIELD_SIZE * FIELD_SIZE;
    size_t fish_size = sizeof(Fish);
    size_t total_memory = total_fish * fish_size;

    std::cout << "FIELD_SIZE = " << FIELD_SIZE << std::endl;
    std::cout << "Всего рыб: " << total_fish << std::endl;
    std::cout << "Размер одной рыбы: " << fish_size << " байт" << std::endl;
    std::cout << "Общая память для рыб: " << total_memory / 1024 / 1024 << " MB" << std::endl;
    std::cout << "Плюс память для указателей: "
            << (total_fish * sizeof(Fish*)) / 1024 / 1024 << " MB" << std::endl;
    std::cout << "Итого: "
            << (total_memory + total_fish * sizeof(Fish*)) / 1024 / 1024
            << " MB" << std::endl;

    std::cout<<"Размер одного корабля " <<sizeof(Ship)<<" байт\n";
    std::cout<<"Общая память для кораблей с указателями"<<(FIELD_SIZE*(sizeof(Ship) + sizeof(Ship*))) / 1024 / 1024 <<" MB\n";
    std::cout<<"Всего памяти на корабли и точки рыбы "<<FIELD_SIZE*(sizeof(Ship) + sizeof(Ship*)) / 1024 / 1024 + (total_memory + total_fish * sizeof(Fish*)) / 1024 / 1024<<" MB\n";
}

int main() {
    clearScreen();
    check_memory_requirements();
    cout<<"\nНажимте, для того чтобы продолжить\n";
    getchar();
    clearScreen();
    FishMatrix fishmap;
    
    
    fill_ocean(fishmap);
    vector<Ship> ships;
    initialize_ship(ships);
    cout<<"Нажимте, для того чтобы продолжить\n";
    getchar();
    clearScreen();

    Ocean ocean;
    ocean.ocean_init(&fishmap, &ships);
    ocean.simulation();

    std::cout << "\nКонец\nНажмите, чтобы продолжить\n" << std::endl;
    getchar();

    return 0;
}
