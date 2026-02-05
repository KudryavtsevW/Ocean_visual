#include "Ocean.h"

Ocean::Ocean()
{
    this->_pFishpoint = nullptr;
    this->_pShips = nullptr;
}

Ocean::~Ocean()
{
    //dtor
}

void Ocean::ocean_init(FishMatrix* pfish, std::vector<Ship>* pship) {
    this->_pFishpoint = pfish;
    this->_pShips = pship;
    for(auto it = this->_pShips->begin(); it!=this->_pShips->end(); ++it) {
        this->_pFishpoint->at(it->getX()).at(it->getY())->addShip(&(*it));
    }

   if(createFileWithPath(this->_filepath, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n")) {
   } else {
       cerr<<"error create log ocean file\n";
   }

}

void Ocean::print_ocean() {
    cout<<"\033[?25h";
    for(size_t i = 0; i<FIELD_SIZE; ++i) {
        for(size_t j = 0; j<FIELD_SIZE; ++j) {
            unique_ptr<Fish>& fish_ptr = this->_pFishpoint->at(i).at(j);
            cout<<fish_ptr;
        }
    }
    for(auto it = this->_pShips->begin(); it != this->_pShips->end(); ++it) {
        cout<<*it;
    }
}

//Удаление корабля из вектора кораблей в Fish
void Ocean::leave_fishpoint(Ship *ship) {
    unique_ptr<Fish> &pfish = this->_pFishpoint->at(ship->getX()).at(ship->getY());
    pfish->remove_ship(ship);
}

void Ocean::set_fishpoint(Ship *ship) {
    auto& fish_ptr = this->_pFishpoint->at(ship->getX()).at(ship->getY());
    fish_ptr->addShip(ship);
}

void Ocean::action(size_t start, size_t end) {
    for(size_t i = start; i<end; ++i) {

        auto &buf = this->_pShips->at(i);
        Ship *it = &buf;

        it->increaseFish(0);
        if(it->getShipState() == SHIP_ST::DELETED){
            continue;
        }
        auto &cur_fishpoint = this->_pFishpoint->at(it->getX()).at(it->getY());

        cur_fishpoint->try_unlock(this->_counter);
        cur_fishpoint->try_lock(this->_counter);

        if(cur_fishpoint->getState() == FISHPOINT_ST::UNACTIVE) {
            cur_fishpoint->generateFish(this->_counter);
        }

        if(it->getFishCount() >= MAXFISH) {
            it->setShipState(SHIP_ST::END);
            it->setEndFlag(true);
        }
        
        switch(it->getShipState()) {
            case SHIP_ST::DEFAULT:
                it->setShipState(SHIP_ST::MOVE);
                break;
            case SHIP_ST::END:
                leave_fishpoint(&(*it));
                it->rand_move();
                if(it->valid_coord()) {
                    set_fishpoint(&(*it));
                } else {
                    // it->terminate();
                    it->setShipState(SHIP_ST::DELETED);
                }
                break;
            case SHIP_ST::DELETED:
                break;

            case SHIP_ST::MOVE:
                if(it->getType() == SHIP_MOVE_TYPE::GREEDY || it->getType() == SHIP_MOVE_TYPE::CRAZY) {
                    if(cur_fishpoint->getAvailable() == false) {
                        leave_fishpoint(&(*it));
                        it->rand_move();
                        set_fishpoint(&(*it));
                        break;
                    }
                    //иначе остаемся на точке и рыбачим
                    it->setShipState(SHIP_ST::FISHING);
                    it->setLockTick(this->_counter);
                    //здесь нет брейка т к если лодка стоит на одном месте то ей мув не нужен. она сразу в фишинг перейдет
                }

                if(it->getType() == SHIP_MOVE_TYPE::LAZY) {
                    if(cur_fishpoint->getAvailable() == false){
                        it->setShipState(SHIP_ST::MOVE);
                        break;
                    } else {
                        it->setShipState(SHIP_ST::FISHING);
                        it->setLockTick(this->_counter);//lock time
                    }
                }

            case SHIP_ST::FISHING:
                if(it->getType() == SHIP_MOVE_TYPE::GREEDY || it->getType() == SHIP_MOVE_TYPE::CRAZY) {
                    if(it->catch_a_fish(this->_counter)) {
                        int maxfish = cur_fishpoint->getFish();
                        if(maxfish < 0){
                            maxfish = 0;
                        }
                        int how_i_wanna_take = CATCHED_FISH_COUNT_LOW + rand()%CATCHED_FISH_COUNT_HIGH;
                        ui how_i_get = min(maxfish, how_i_wanna_take);
                        it->increaseFish(how_i_get);
                        cur_fishpoint->killFish(how_i_get);
                        it->setShipState(SHIP_ST::MOVE);
                    }
                }
                
                if(it->getType() == SHIP_MOVE_TYPE::LAZY){
                    if(it->catch_a_fish(this->_counter)) {
                        int maxfish = cur_fishpoint->getFish();
                        if(maxfish< 0) {
                            maxfish = 0;
                        }
                        int how_i_wanna_take = CATCHED_FISH_COUNT_LOW + rand()%CATCHED_FISH_COUNT_HIGH;
                        ui how_i_get = min(maxfish, how_i_wanna_take);
                        it->increaseFish(how_i_get);
                        cur_fishpoint->killFish(how_i_get);
                        it->setShipState(SHIP_ST::MOVE);
                    }
                }
                break;
            }
            cur_fishpoint->changeState(this->_counter);
        }
    }
void Ocean::rand_movement() {
        for(auto it = this->_pShips->begin(); it!=this->_pShips->end(); ++it) {
        leave_fishpoint(&(*it));
        it->rand_move();
        set_fishpoint(&(*it));
    }
}

void Ocean::remove_ships() {
    for(auto it = this->_pShips->begin(); it!=this->_pShips->end();) {
        if(it->getShipState() == SHIP_ST::DELETED) {
            it = this->_pShips->erase(it);
        } else {
            ++it;
        }
    }
}

void Ocean::print_statistics() {
    uint8_t minim = 255;
    uint8_t maxim = 0;
    ui avg = 0;
    size_t cnt = 0;

    ui greedy_count = 0;
    ui crazy_count = 0;
    ui lazy_count = 0;
    for(auto it = this->_pShips->begin(); it!= this->_pShips->end(); ++it) {
        if((*it).getIncrease() > 0) {
            minim = min((*it).getIncrease(), minim);
            maxim = max((*it).getIncrease(), maxim);
            avg += (*it).getIncrease();
            cnt++;
        }

        if(it->getType() == SHIP_MOVE_TYPE::GREEDY) {
            greedy_count++;
        }

        if(it->getType() == SHIP_MOVE_TYPE::LAZY) {
            lazy_count++;
        }

        if(it->getType() == SHIP_MOVE_TYPE::CRAZY) {
            crazy_count++;
        }
        if(it->getShipState() == SHIP_ST::DELETED) {
            this->_last_remove = this->_counter;
            if(this->_first_remove == 0) {
                this->_first_remove = this->_counter;
            }
        }

    }
    if(cnt != 0) {
        avg /= cnt;
    }
    cout << "\033[" << 1 << ";" << 1 << "H"<<"|"
    <<centerInStars("Tick")<<"|"
    <<centerInStars("GREEDY")<<"|"
    <<centerInStars("CRAZY")<<"|"
    <<centerInStars("LAZY")<<"|"
    <<centerInStars("min")<<"|"
    <<centerInStars("max")<<"|"
    <<centerInStars("avg")<<"|"
    <<centerInStars("first remove")<<"|"
    <<centerInStars("last remove")<<"|";
    string s_minim;
    if(minim == 255) {
        s_minim = "NaN";
    } else {
        s_minim = to_string(minim);
    }
    string s_maxim;
    if(maxim == 0) {
        s_maxim = "NaN";
    } else {
        s_maxim = to_string(maxim);
    }
    string s_avg;
    if(avg == 0) {
        s_avg = "NaN";
    } else {
        s_avg = to_string(avg);
    }
//    stringstream ss;

        cout << "\033[" << 2 << ";" << 1 << "H"<<"|"
    <<centerInStars(to_string(this->_counter))<<"|"
    <<centerInStars(to_string(greedy_count))<<"|"
    <<centerInStars(to_string(crazy_count))<<"|"
    <<centerInStars(to_string(lazy_count))<<"|"
    <<centerInStars(s_minim)<<"|"
    <<centerInStars(s_maxim)<<"|"
    <<centerInStars(s_avg)<<"|"
    <<centerInStars(to_string(this->_first_remove))<<"|"
    <<centerInStars(to_string(this->_last_remove))<<"|";
}



void Ocean::simulation() {

    const size_t NUM_THREADS = std::thread::hardware_concurrency();
    while(this->_pShips->size()>0){
        cout<<"\033[H";
        // Настройка OpenMP
        omp_set_num_threads(NUM_THREADS);
        size_t total_ships = this->_pShips->size();
        #pragma omp parallel
        {
            uint8_t thread_id = omp_get_thread_num();
            uint8_t num_threads = omp_get_num_threads();
            size_t chunk_size = total_ships / num_threads;
            
            size_t start = thread_id * chunk_size;
            size_t end = (thread_id == num_threads - 1) ? total_ships : (thread_id + 1) * chunk_size;

            // Вызываем ваш метод action с параметрами
            this->action(start, end);
        }
        print_statistics();
        remove_ships();
        print_ocean();

        cout.flush(); 
        
        this->_counter++;
        for(auto it = this->_pShips->begin(); it!=this->_pShips->end(); ++it) {
            it->write_logs(this->_counter);
        }
        for(ui i = 0; i<FIELD_SIZE; ++i) {
            for(ui j = 0; j<FIELD_SIZE; ++j) {
                auto &cur_fishpoint = this->_pFishpoint->at(i).at(j);
                cur_fishpoint->write_logs(this->_counter);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    print_statistics();
}
