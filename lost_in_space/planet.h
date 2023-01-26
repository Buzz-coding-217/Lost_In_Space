#ifndef PLANETS
#define PLANETS
#include "splashkit.h"
#include <vector>
using namespace std;

//enum of all the different planets
enum planet_names
{
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    PLUTO
};

//struct which contain all the information of planets
struct planet_data 
{
    planet_names planet_name;
    sprite planet_sprite;
};

planet_data new_planet(double x, double y,int planet_number);
void draw_planet(planet_data draw);
void update_planet(planet_data update);
#endif