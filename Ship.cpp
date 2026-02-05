#include "Ship.h"

Ship::Ship()
{
    this->_x = 1;
    this->_y = 1;
}

long int Ship::getX() {
    return this->_x;
}

long int Ship::getY() {
    return this->_y;
}

SHIP_ST Ship::getShipState() {
    return this->_state;
}

SHIP_MOVE_TYPE Ship::getType() {
    return this->_type;
}

void Ship::setShipState(SHIP_ST state) {
    this->_state = state;
}
bool Ship::valid_coord(){
    if(this->_x > FIELD_SIZE -1 ) {
        return false;
    }
    if(this->_x < 0) {
        return false;
    }
    if(this->_y > FIELD_SIZE - 1) {
        return false;
    }
    if(this->_y < 0) {
        return false;
    }
    return true;
}

void Ship::setMovementType(SHIP_MOVE_TYPE type) {
    this->_type = type;
}


void Ship::increaseFish(uint8_t dfish) {
    this->_fish_count += dfish;
    this->_increase = dfish;
}

uint8_t Ship::getIncrease() {
    return this->_increase;
}


void Ship::setEndFlag(bool flag){
    this->_end_flag = flag;
}

void Ship::setShip(size_t x, size_t y) {
    this->_x = x;
    this->_y = y;
    // this->_initialize = true;
    for(ui i = 0; i<7; ++i) {
        this->_name += char(65 + rand()%12);
    }
    char filename[16];
    snprintf(filename, sizeof(filename), "%s", this->_name.c_str());
    string filepath = this->_logdir + string(filename);
    if(createFileWithPath(filepath, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n")) {
    } else {
        cout<<"error "<<this->_x<<" "<<this->_y;
    }
    this->_filepath = filepath;
    ui type = rand() % 10;
    if(type <=6) {
        this->_type = SHIP_MOVE_TYPE::GREEDY;
    } else if(type >=5 && type<=7) {
        this->_type = SHIP_MOVE_TYPE::LAZY;

    } else if(type >= 8) {
        this->_type = SHIP_MOVE_TYPE::CRAZY;
    }
}

void Ship::setLockTick(ui tick) {
    this->_local_tick = tick;
}

void Ship::moveTo(int8_t dx, int8_t dy) {

    if(this->_x + dx >= FIELD_SIZE || this->_x + dx < 0) {
        dx = 0;
    }
    if(this->_y + dy >= FIELD_SIZE || this->_y + dy < 0) {
        dy = 0;
    }
    this->_x += dx;
    this->_y += dy;
}
unsigned int Ship::getFishCount() {
    return this->_fish_count;
}
void Ship::rand_move() {
    if(this->_end_flag) {
        if(this->_x >= FIELD_SIZE - this->_x) {
            this->_x++;
        }else {
            this->_x--;
        }
        return;

    }
    if(this->_type == SHIP_MOVE_TYPE::CRAZY) {
        if(this->_y + 1 < FIELD_SIZE) {
            this->_y++;
        } else {
            if(this->_x + 1 < FIELD_SIZE) {
                this->_x++;
                this->_y = 0;
            } else {
                this->_y = 0;
                this->_x = 0;
            }
        }
        return;
    }
    if(this->_state == SHIP_ST::DEFAULT || this->_state == SHIP_ST::MOVE) {
        int8_t d = rand() % 3;
        int8_t dx = -1 + d;
        d = rand()%3;
        int8_t dy = -1 + d;
        moveTo(dx, dy);
    }
}

bool Ship::catch_a_fish(ui tick){
    ui delta_tick = CATCHED_FISH_TIMER_LOW + rand()%CATCHED_FISH_TIMER_HIGH;
    if(tick - this->_local_tick >= delta_tick) {
        return true;
    }
    return false;
}

void Ship::write_logs(ui tick) {
    stringstream ss;
    ss << "<tick id=\"" << tick << "\">\n";
    ss << "    <ptr>"<<this<<"</ptr>\n";
    ss << "    <name>"<<this->_name<<"</name>\n";
    ss << "    <x>" << this->_x << "</x>\n";
    ss << "    <y>" << this->_y << "</y>\n";
    ss << "    <type>" << ship_type[int(this->_type)] << "</type>\n";
    ss << "    <ship_state>" << ship_stat[int(this->_state)] << "</ship_state>\n";
    ss << "    <fish_count>" << this->_fish_count << "</fish_count>\n";
    ss << "    <lock_tick>" << this->_local_tick << "</lock_tick>\n";
    ss << "</tick>\n";
    createFileWithPath(this->_filepath, ss.str());
}

Ship::~Ship()
{
    //dtor
}

ostream &operator << (ostream &os, Ship &sh) {
    os << "\033[" << sh._x+BEGIN_X << ";" << sh._y+BEGIN_Y<< "H";
    os<<'#';
    os.flush();
    return os;
}
