#include "splashkit.h"
#include "planet.h"
using namespace std;

//drawing different bitmap for differenet planet
bitmap bitmap_planet(planet_names draw)
{
    switch (draw)
    {
    case MERCURY:
        return bitmap_named("mercury");
    case VENUS:
        return bitmap_named("venus");
    case EARTH:
        return bitmap_named("earth");
    case MARS:
        return bitmap_named("mars");
    case JUPITER:
        return bitmap_named("jupiter");
    case SATURN:
        return bitmap_named("saturn");
    case URANUS:
        return bitmap_named("uranus");
    case NEPTUNE:
        return bitmap_named("neptune");
    case PLUTO:
        return bitmap_named("pluto");
    }
}

//function which gives information for new planet
planet_data new_planet(double x, double y,int planet_name)
{
    planet_names name;
    planet_data planet;
    name = static_cast<planet_names>(planet_name);
    planet.planet_sprite = create_sprite(bitmap_planet(name));
    planet.planet_name = name;

    sprite_set_x(planet.planet_sprite, x);
    sprite_set_y(planet.planet_sprite, y);

    return planet;
}

//drawing the new planet
void draw_planet(planet_data draw)
{
    draw_sprite(draw.planet_sprite);
}

//updating all the planet
void update_planet(planet_data update)
{
    update_sprite(update.planet_sprite);
}