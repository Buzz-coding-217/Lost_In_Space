#ifndef LOST_IN_SPACE_POWER_UP
#define LOST_IN_SPACE_POWER_UP
#include "splashkit.h"
using namespace std;

//creating a enum for different power-ups
enum power_ups {
    SHIELD,
    FUEL,
    DIAMOND,
    CASH,
    COIN,
    HEART,
    BATTERY
};

//creating a struct for power up data
struct power_up_data {
    power_ups power_up;
    sprite power_up_sprite;
};

//calling the functions in our power_up.cpp
power_up_data new_power_up(double x,double y);
void draw_power_up(const power_up_data &draw);
void update_power_up(power_up_data &update);

#endif