#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/**
* @brief Количество клеток (размерность)
*/
#define FIELD_SIZE 20 

/**
* @brief Таймер улова рыбы. Нижняя и верхняя граница
*/
#define CATCHED_FISH_TIMER_LOW 1
#define CATCHED_FISH_TIMER_HIGH 3

/**
* @brief Количество выловленной рыбы. Нижняя и верхняя граница
*/
#define CATCHED_FISH_COUNT_LOW 1
#define CATCHED_FISH_COUNT_HIGH 10

/**
* @brief Количество количество рыбы при генерации в клетке. Нижняя и верхняя граница
*/
#define GENERATE_FISH_COUNT_LOW 0
#define GENERATE_FISH_COUNT_HIGH 15 + 1

/**
* @brief Таймер актуализации клетки. Нижняя и верхняя граница
*/
#define GENERATE_FISH_TIMER_LOW 15
#define GENERATE_FISH_TIMER_HIGH 30 + 1

/**
* @brief Максимальное количество выловленной рыбы
*/
#define MAXFISH 20

/**
* @brief Смещение матрицы вывода Х
*/
#define BEGIN_X 5

/**
* @brief Смещение матрицы вывода У
*/
#define BEGIN_Y 15

using namespace std;
typedef unsigned int ui;


static const char *ship_stat[] = {"DEFAULT","MOVE", "FISHING", "END", "DELETED"};
enum class SHIP_ST{
    DEFAULT = 0,
    MOVE,
    FISHING,
    END,
    DELETED
};


static const char *ship_type[] = {"DEFAULT", "GREEDY", "LAZY", "CRAZY", "END"};
enum class SHIP_MOVE_TYPE {
    DEFAULT = 0,
    GREEDY,
    LAZY,
    CRAZY,
    END
};

static const char *fishpoint_state[] = {"UNACTIVE", "LOCK", "ACTIVE"};
enum class FISHPOINT_ST {
    UNACTIVE = 0,
    LOCK,
    ACTIVE
};

/**
* @brief Функиця вывода для таблицы в терминал
*/
static string centerInStars(const string& text, uint8_t width = 14) {
    if (text.length() >= width) {
        return text.substr(0, width);
    }

    int padding = width - text.length();
    int left_padding = padding / 2;
    int right_padding = padding - left_padding;
    string res = string(left_padding, ' ') + text + string(right_padding, ' ');
    return res;
}
#endif // GLOBALS_H_INCLUDED
