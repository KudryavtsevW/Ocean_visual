#include "Fish.h"

Fish::Fish()
{
    //ctor
}

Fish::~Fish()
{
    //dtor
}

size_t Fish::getX() {
    return this->_x;
}
size_t Fish::getY() {
    return this->_y;
}

//Возвращает количество рыбы в клетке
uint8_t Fish::getFish() {
    return this->_fish_count;
}


bool Fish::getAvailable() {
    if(this->_state == FISHPOINT_ST::UNACTIVE) {
        return true;
    }

    if(this->_state == FISHPOINT_ST::ACTIVE) {
        return true;
    }

    if(this->_state == FISHPOINT_ST::LOCK) {
        return false;
    }
    return true;
}

void Fish::generateFish(ui tick) {
    this->_fish_count = GENERATE_FISH_COUNT_LOW + rand()%(GENERATE_FISH_COUNT_HIGH - GENERATE_FISH_COUNT_LOW);
    this->_local_tick = tick;
    this->_state = FISHPOINT_ST::ACTIVE;
}

void Fish::changeState(ui tick) {
    ui time_activation = GENERATE_FISH_TIMER_LOW + rand()%(GENERATE_FISH_TIMER_HIGH + GENERATE_FISH_TIMER_LOW);
    if(tick - this->_local_tick < time_activation && this->_fish_count <=0) {
        this->_state = FISHPOINT_ST::LOCK;
        this->_fish_count = 0;
    }
    
    else if (tick - this->_local_tick < time_activation && this->_fish_count > 0) {
        this->_state = FISHPOINT_ST::ACTIVE;
    }
    
    else if (tick - this->_local_tick >= time_activation) {
        this->_state = FISHPOINT_ST::UNACTIVE;
    }
}

void Fish::try_lock(ui counter) {
    if(this->_state == FISHPOINT_ST::ACTIVE) {
        ui time_activation = GENERATE_FISH_TIMER_LOW + rand()%(GENERATE_FISH_TIMER_HIGH + GENERATE_FISH_TIMER_LOW);
        if(counter - this->_local_tick < time_activation && this->_fish_count <= 0) {
            this->_state = FISHPOINT_ST::LOCK;
            this->_fish_count = 0;
        }
    }
}


void Fish::try_unlock(ui counter) {
    if(this->_state == FISHPOINT_ST::LOCK) {
        ui time_activation = GENERATE_FISH_TIMER_LOW + rand()%(GENERATE_FISH_TIMER_HIGH + GENERATE_FISH_TIMER_LOW);
        if(counter - this->_local_tick > time_activation) {
            this->_state = FISHPOINT_ST::UNACTIVE;
        }
    }
}




FISHPOINT_ST Fish::getState() {
    return this->_state;
}

void Fish::killFish(ui fish_count) {
    this->_fish_count -= fish_count;
}
ostream &operator <<(ostream &os, unique_ptr<Fish> &fp) {
    os << "\033[" << fp->_x+BEGIN_X << ";" << fp->_y+BEGIN_Y << "H";
    os<<'0';
    os.flush();
    return os;
}

void Fish::setPoint (size_t x, size_t y) {
    this->_x = x;
    this->_y = y;
    char filename[16];
    snprintf(filename, sizeof(filename), "point(%lu,%lu)", this->_x, this->_y);
    string filepath = this->_logdir + string(filename);
    if(createFileWithPath(filepath, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n")) {
    } else {
        cout<<"error "<<this->_x<<" "<<this->_y;
    }
    this->_filepath = filepath;
}

void Fish::remove_ship(Ship *ship) {

    for(auto it = this->_currents_ships.begin(); it!=this->_currents_ships.end(); ++it) {

        if(*it == ship) {
            this->_currents_ships.erase(it);
            break;
        }
    }
}

void Fish::write_logs(ui tick) {
    stringstream ss;
    ss << "<tick id=\"" << tick << "\">\n";
    ss << "    <x>" << this->_x << "</x>\n";
    ss << "    <y>" << this->_y << "</y>\n";
    for(auto it = this->_currents_ships.begin(); it!=this->_currents_ships.end(); ++it) {
        ss<<"    <ptr>"<<*it<<"</ptr>\n";
    }
    ss << "    <lock_tick>" << this->_local_tick << "</lock_tick>\n";
    ss << "    <state>" << fishpoint_state[int(this->_state)] << "</state>\n";
    ss << "    <delta_time>" << int(tick - this->_local_tick) << "</delta_time>\n";
    ss << "    <fish_count>" << int(this->_fish_count) << "</fish_count>\n";
    ss << "</tick>\n"; 
    createFileWithPath(this->_filepath, ss.str());
}

void Fish::addShip(Ship* ship) {
    this->_currents_ships.push_back(ship);
}
