#include "splashkit.h"
#include "power_up.h"
using namespace std;

// creating a function for creating bitmaps for different power ups
bitmap power_bitmap(power_ups powers)
{
    switch (powers)
    {
    case SHIELD:
        return bitmap_named("shield");
    case FUEL:
        return bitmap_named("fuel");
    case DIAMOND:
        return bitmap_named("diamond");
    case CASH:
        return bitmap_named("cash");
    case COIN:
        return bitmap_named("coin");
    case HEART:
        return bitmap_named("heart");
    case BATTERY:
        return bitmap_named("battery");
    }
}

// creating a function for displaying the power up and making it move
power_up_data new_power_up(double x, double y)
{   power_ups kind;
    power_up_data result;
    kind = static_cast<power_ups>(rnd(7));
    result.power_up_sprite = create_sprite(power_bitmap(kind));
    result.power_up = kind;

    //setting the location of a new power up
    sprite_set_x(result.power_up_sprite, x);
    sprite_set_y(result.power_up_sprite, y);

    //moving the power up at random direction and velocity
    sprite_set_dx(result.power_up_sprite, rnd() * 4 - 2);
    sprite_set_dy(result.power_up_sprite, rnd() * 4 - 2);
    
    return result;
};
// displaying the power up
void draw_power_up(const power_up_data &draw)
{
    draw_sprite(draw.power_up_sprite);
};

// updating the power up for moving
void update_power_up(power_up_data &update)
{
    update_sprite(update.power_up_sprite);
};