/*
Name :- Shravel Sharma
Roll no :- 2110994791
*/

//****   program.cpp   ****

#include "splashkit.h"
#include "player.h"
#include "power_up.h"
#include "lost_in_space.h"
#include "planet.h"
#include <vector>
using namespace std;
using std::vector;

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt");
}


/**
 * Entry point.
 *
 * Manages the initialisation of data, the event loop, and quitting.
 */
int main()
{
    //creating a new window
    open_window("Lost In Space", 800, 800);
    //loading all the resources
    load_resources();

    //displaying the intro of the game untill space key is pressed or quit requested
    while(not key_typed(SPACE_KEY) and not quit_requested())
    {
        intro();
    }
    //entering the data of a new game
    game_data game;
    game = new_game();
    //displaying the game untill the game is over or quit is requested
    while (not game.game_over and not quit_requested())
    {
        handle_input(game.player);
        update_game(game);
        draw_game(game);
    }

    //displaying the game over screen
    game_over(game);

    return 0;
}

//****    lost_in_space.cpp    ****

#include "splashkit.h"
#include <vector>
#include "lost_in_space.h"
#include "planet.h"
using namespace std;
#define top_left_offset -1500
#define total_height 3000
#define total_width 3000
#define mini_map_height 100
#define mini_map_width 100
#define mini_map_location_x 63
#define mini_map_location_y 120

//drawing the bitmaps and data on screen
void HUD_sources(game_data &game)
{
    draw_bitmap("space", -10, -10, option_to_screen(option_scale_bmp(1, 1)));
    draw_bitmap("background", 0, 0, option_to_screen());
    draw_rectangle(COLOR_MIDNIGHT_BLUE,-10,-10,370,280,option_to_screen());
    draw_bitmap("empty", 180, 185, option_to_screen());
    draw_bitmap("full", 180, 185, option_to_screen(option_part_bmp(0, 0, game.player.fuel_pct * bitmap_width("full"), bitmap_height("full"))));
    draw_bitmap("empty",180,205,option_to_screen());
    draw_bitmap("battery_bar", 180, 205, option_to_screen(option_part_bmp(0, 0, game.player.battery * bitmap_width("battery_bar"), bitmap_height("battery_bar"))));
    if(game.player.lives >=3)
        {draw_bitmap("heart", 230, 120, option_to_screen(option_scale_bmp(0.3, 0.3)));}
    if(game.player.lives >=2)
        {draw_bitmap("heart", 190, 120, option_to_screen(option_scale_bmp(0.3, 0.3)));}
    if(game.player.lives>=1)
        {draw_bitmap("heart", 150, 120, option_to_screen(option_scale_bmp(0.3, 0.3)));}
    if(game.player.shield >= 3)
        draw_bitmap("shield", 230, 80, option_to_screen(option_scale_bmp(0.3, 0.3)));
    if(game.player.shield >= 2)
        draw_bitmap("shield", 190, 80, option_to_screen(option_scale_bmp(0.3, 0.3)));
    if(game.player.shield >= 1)
        draw_bitmap("shield", 150, 80, option_to_screen(option_scale_bmp(0.3, 0.3)));
}

//putting the inforamation and text on screen
void HUD_text(game_data &game)
{
    int score = convert_to_integer(to_string(game.player.score));
    draw_text("SCORE: " + to_string(score), COLOR_WHITE,"font_hud",15, 60, 60, option_to_screen());
    draw_text("LOCATION: " + point_to_string(center_point(game.player.player_sprite)), COLOR_WHITE,"font_hud",15, 60, 75, option_to_screen());
    draw_text("CASH: " + to_string(game.player.cash), COLOR_WHITE,"font_hud",15, 60, 88, option_to_screen());
    draw_text("DIAMONDS: " + to_string(game.player.diamond), COLOR_WHITE,"font_hud",15, 60, 102, option_to_screen());
    draw_text("LEVEL: " + to_string(game.level),COLOR_WHITE,"level",36, 620, 30, option_to_screen());
}

//drawing the HUD on screen
void HUD(game_data &game)
{
    HUD_sources(game);
    HUD_text(game);
    draw_mini_map(game);
}

//displaying the intro of game untill space is entered
void intro()
{
    process_events();
    clear_screen(COLOR_BLACK);
    draw_text("LOST IN SPACE",COLOR_WHITE,"lost_in_space",50, 170,250);
    draw_text("PRESS SPACE TO ENTER SPACE",COLOR_MIDNIGHT_BLUE,"lost_in_space",35, 100,450);
    refresh_screen();
}

//putting the data of new game which is to be played
game_data new_game()
{
    game_data game;
    game.player = new_player();
    game.planet_count = 0;
    game.game_timer = create_timer("time");
    start_timer("time");
    game.level = 0;
    game.game_over = false;
    return game;
}

//adding all the planets on game area one by one such that each planet appear once
void add_planet(game_data &game)
{
    while (game.planet_count < 9)
    {
        game.planets.push_back(new_planet(rnd(-1500,1500),rnd(-1500,1500),game.planet_count));      //putting planet on random locations
        game.planet_count++;
    }
}

//adding new power up on random location
void add_power_up(game_data &game)
{
    game.power_ups.push_back(new_power_up(rnd(-1500, 1500), rnd(-1500, 1500)));
}

//drawing all the sprites and hud on game area
void draw_game(game_data &game)
{
    clear_screen(COLOR_BLACK);
    HUD(game);
    draw_player(game.player);
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        draw_power_up(game.power_ups[i]);
    }
    for (int j = 0; j < game.planets.size(); j++)
    {
        draw_planet(game.planets[j]);
    }
    refresh_screen(60);
}

//removing the power up from game
void remove_power_up(game_data &game, int index)
{
    int last_index = game.power_ups.size() - 1;
    game.power_ups[index] = game.power_ups[last_index];
    game.power_ups.pop_back();
}

//removing the planet from game
void remove_planet(game_data &game, int index)
{
    int last_index = game.planets.size() - 1;
    game.planets[index] = game.planets[last_index];
    game.planets.pop_back();
}

//keeping the power ups inside the game area and if a power up gets out remove it
void check_power_up(game_data &game)
{
    float power_up_x, power_up_y;
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        power_up_x = sprite_x(game.power_ups[i].power_up_sprite);
        power_up_y = sprite_y(game.power_ups[i].power_up_sprite);

        if (power_up_x > 1500 || power_up_x < -1500 || power_up_y > 1500 || power_up_y < -1500)
        {
            remove_power_up(game, i);
        }
    }
}

//updating the game over and over again
void update_game(game_data &game)
{
    update_player(game.player);

    //after a fixed period of time the level of the game increase
    if (timer_ticks("time") > 0 and timer_ticks("time") < 15000)
        game.level = 1;
    else if (timer_ticks("time") > 15000 and timer_ticks("time") < 30000)
        game.level = 2;
    else if (timer_ticks("time") > 30000 and timer_ticks("time") < 45000)
        game.level = 3;
    else if (timer_ticks("time") > 45000 and timer_ticks("time") < 60000)
        game.level = 4;
    else if (timer_ticks("time") > 60000)
        game.level = 5;

    //changing the setting at different levels, increased level = less power up
    if (game.level == 1){
        if (rnd() < 0.05)
            add_power_up(game);
    }
    else if (game.level == 2){
        if (rnd() < 0.04)
            add_power_up(game);
    }
    else if (game.level == 3){
        if (rnd() < 0.03)
            add_power_up(game);
    }
    else if (game.level == 4){
        if (rnd() < 0.02)
            add_power_up(game);
    }
    else if (game.level == 5){
        if (rnd() < 0.01)
            add_power_up(game);
    }
    
    //adding the planets
    add_planet(game);
    //updating all the power ups and planets
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        update_power_up(game.power_ups[i]);
    }
    for (int j = 0; j < game.planets.size(); j++)
    {
        update_planet(game.planets[j]);
    }

    //if all the planets are removed, new planets appear at random location
    if (game.planets.size() == 0)
    {
        game.planet_count = 0;
        add_planet(game);
    }

    //if lives are less than zero, game will be over
    if(game.player.lives < 0)
        game.game_over = true;
    
    //checking the power up and checking the collision with planet or power up
    check_power_up(game);
    check_collision(game);
}

//getting the power up location on mini map
point_2d power_mini_map_coordinate(const power_up_data &power_up)
{
    int mini_map_x, mini_map_y;

    double power_x, power_y;
    power_x = sprite_x(power_up.power_up_sprite);
    power_y = sprite_y(power_up.power_up_sprite);

    mini_map_x = (power_x - top_left_offset) / total_width * mini_map_width + mini_map_location_x;
    mini_map_y = (power_y - top_left_offset) / total_height * mini_map_height + mini_map_location_y;

    return point_at(mini_map_x, mini_map_y);
}

//getting the player location on mini map
point_2d player_mini_map_coordination(const player_data &player)
{
    int mini_map_x, mini_map_y;

    double player_x, player_y;
    player_x = sprite_x(player.player_sprite);
    player_y = sprite_y(player.player_sprite);

    mini_map_x = (player_x - top_left_offset) / total_width * mini_map_width + mini_map_location_x;
    mini_map_y = (player_y - top_left_offset) / total_height * mini_map_height + mini_map_location_y;

    return point_at(mini_map_x, mini_map_y);
}

//getting the planet location on mini map
point_2d planet_mini_map_coordination(const planet_data &planet)
{
    int mini_map_x, mini_map_y;

    double planet_x, planet_y;
    planet_x = sprite_x(planet.planet_sprite);
    planet_y = sprite_y(planet.planet_sprite);

    mini_map_x = (planet_x - top_left_offset) / total_width * mini_map_width + mini_map_location_x;
    mini_map_y = (planet_y - top_left_offset) / total_height * mini_map_height + mini_map_location_y;

    return point_at(mini_map_x, mini_map_y);
}

//drawing the player, power up and planet location on mini map
void draw_mini_map(const game_data &game)
{
    fill_rectangle(COLOR_BLACK, 63, 120, 100, 100, option_to_screen());
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        draw_pixel(rgba_color(255, 255, 255, 255), power_mini_map_coordinate(game.power_ups[i]), option_to_screen());
    }
    for (int j = 0; j < game.planets.size(); j++)
    {
        draw_pixel(rgba_color(255, 255, 0, 255), planet_mini_map_coordination(game.planets[j]), option_to_screen());
    }
    draw_pixel(rgba_color(255, 0, 0, 255), player_mini_map_coordination(game.player), option_to_screen());
}

//applying the power up on player, different power up has different benefits
void apply_power_up(game_data &game, int index)
{
    power_ups power = game.power_ups[index].power_up;
    switch (power)
    {
    case COIN:
        game.player.cash += 1;
        write_line("A Coin is taken!");
        break;
    case CASH:
        game.player.cash += 3;
        write_line("A stack of cash is taken!");
        break;
    case DIAMOND:
        game.player.diamond += 1;
        write_line("A dimond is taken!");
        break;
    case FUEL:
        game.player.fuel_pct += 0.50;
        write_line("Fuel is refilled!");
        if(game.player.fuel_pct > 1)
            game.player.fuel_pct = 1;
        break;
    case HEART:
        game.player.lives += 1;
        write_line("Heart is taken!");
        break;
    case SHIELD:
        write_line("Shield is taken!");
        game.player.shield += 1;
        break;
    case BATTERY:
        write_line("A battery is taken");
        game.player.battery = 1.00;
        break;
    default:
        break;
    }
}

// checking the collision with power up or planet and act accordingly, each power up and planet are checked one by one
void check_collision(game_data &game)
{
    bool collision_power_up;
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        collision_power_up = sprite_collision(game.player.player_sprite, game.power_ups[i].power_up_sprite);
        if (collision_power_up == true)
        {
            apply_power_up(game, i);
            remove_power_up(game, i);
            play_sound_effect("pick_up");
        }
    }
    bool collision_planet;
    for(int i = 0;i < game.planets.size(); i++)
    {
        collision_planet = sprite_collision(game.player.player_sprite, game.planets[i].planet_sprite);
        if (collision_planet == true)
        {
            remove_planet(game,i);
            game.player.shield -= 1;
            play_sound_effect("crash");
        }
    }
}

//displaying the game over screen and showing the collectables we collected in game
void game_over(game_data &game)
{
    clear_screen(COLOR_BLACK);
    draw_text("GAME OVER", COLOR_RED,"lost_in_space",62,180,210,option_to_screen());
    int score , cash , diamonds;
    score = game.player.score; 
    cash = game.player.cash;
    diamonds = game.player.diamond;
    draw_text("SCORE: " + to_string(score), COLOR_WHITE,"game_over",60,200,380,option_to_screen());
    draw_text("CASH: " + to_string(cash), COLOR_WHITE,"game_over",60,200,420,option_to_screen());
    draw_text("DIAMONDS: " + to_string(diamonds), COLOR_WHITE,"game_over",60,200,460,option_to_screen());
    refresh_screen();
    delay(3000);
}

//****     lost_in_space.h    ****

#include "splashkit.h"
#include <vector>
#include "power_up.h"
#include "player.h"
#include "planet.h"
using namespace std;
using std::vector;

//struct which contain all the information of the game
struct game_data
{
    player_data player;
    vector<power_up_data> power_ups;
    vector<planet_data> planets;
    int planet_count;
    timer game_timer;
    int level;
    bool game_over;
};
game_data new_game();
void draw_game(game_data &game);
void update_game(game_data &game);
void draw_mini_map(const game_data &game);
void check_power_up(const game_data &game);
void check_collision(game_data &game);
void apply_power_up(game_data &game, int index);
void HUD_sources(game_data &game);
void HUD_text(game_data &game);
void HUD(game_data &game);
void game_over(game_data &game);
void intro();

//****   power_up.cpp   ****

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

//**** power_up.h ****

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

//****    planet.cpp   ****

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

//****   planet.h   ****


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

//****   player.cpp   ****

#include "player.h"
#include "splashkit.h"

/**
 * The ship bitmap function converts a ship kind into a 
 * bitmap that can be used.
 * 
 * Not exposed by the header.
 * 
 * @param kind  The kind of ship you want the bitmap of
 * @return      The bitmap matching this ship kind
 */
bitmap ship_bitmap(ship_kind kind)
{
    switch (kind)
    {
    case AQUARII:
        return bitmap_named("aquarii");
    case GLIESE:
        return bitmap_named("gliese");
    default:
        return bitmap_named("pegasi");
    }
}

player_data new_player()
{
    player_data result;
    bitmap default_bitmap = ship_bitmap(AQUARII);

    // Create the sprite with 3 layers - we can turn on and off based
    // on the ship kind selected
    result.player_sprite = create_sprite(default_bitmap);
    

    sprite_add_layer(result.player_sprite, ship_bitmap(GLIESE), "GLIESE");
    sprite_add_layer(result.player_sprite, ship_bitmap(PEGASI), "PEGASI");

    // Default to layer 0 = Aquarii so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = AQUARII;

    //making the sprite to be in center of screen
    double x = (screen_width() - sprite_width(result.player_sprite))/2;
    double y = (screen_height() - sprite_height(result.player_sprite))/2;
    // Position in the centre of the initial screen
    sprite_set_x(result.player_sprite, x);
    sprite_set_y(result.player_sprite, y);
    result.score = 0;
    result.shield = 3;
    result.fuel_pct = 1.00;
    result.cash = 0;
    result.diamond = 0;
    result.lives = 3;
    result.battery = 1.00;

    return result;
}

//drawing the player 
void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

//updating the player
void update_player(player_data &player_to_update)
{
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(player_to_update.player_sprite);

    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER;
    double top_edge = camera_y() + SCREEN_BORDER;
    double bottom_edge = top_edge + screen_height() - 2 * SCREEN_BORDER;

    // Get the center of the player
    point_2d sprite_center = center_point(player_to_update.player_sprite);

    // Test if the player is outside the area and move the camera
    // the player will appear to stay still and everything else
    // will appear to move :)

    // Test top/bottom of screen
    if (sprite_center.y < top_edge)
    {
        move_camera_by(0, sprite_center.y - top_edge);
    }
    else if (sprite_center.y > bottom_edge)
    {
        move_camera_by(0, sprite_center.y - bottom_edge);
    }

    // Test left/right of screen
    if (sprite_center.x < left_edge)
    {
        move_camera_by(sprite_center.x - left_edge, 0);
    }
    else if (sprite_center.x > right_edge)
    {
        move_camera_by(sprite_center.x - right_edge, 0);
    }
    //to keep in boundary
    if (sprite_center.x < -1500){
        sprite_set_x(player_to_update.player_sprite, 1450);
    }
    else if (sprite_center.y < -1500){
        sprite_set_y(player_to_update.player_sprite, 1450);
    }
    if (sprite_center.x > 1500){
        sprite_set_x(player_to_update.player_sprite, -1500);
    }
    else if (sprite_center.y > 1500){
        sprite_set_y(player_to_update.player_sprite, -1500);
    }

    player_to_update.score += 0.03;
    player_to_update.fuel_pct -= 0.001278;
    player_to_update.battery -= 0.0007;
    //battery will refill and decrement a live
    if(player_to_update.battery < 0)
    {
        player_to_update.lives -= 1;
        player_to_update.battery = 1.00;
    }
    //fuel will refill and decrement a live
    if(player_to_update.fuel_pct < 0)
        player_to_update.fuel_pct = 0;
    //shield are not greater than 3 or less than 0
    if(player_to_update.shield > 3)
        player_to_update.shield = 3;
    if(player_to_update.shield < 0)
    {
        player_to_update.shield = 0;
        player_to_update.lives -= 1;
    }
    //if fuel is zero
    if(player_to_update.fuel_pct == 0)
    {
        player_to_update.lives -= 1;
        player_to_update.fuel_pct = 1.00;
    }
    //so lives are not greater than 3
    if(player_to_update.lives > 3)
        player_to_update.lives = 3;
}

/**
 * Switch the ship kind - will adjust which layer is hidden/shown
 */
void player_switch_to_ship(player_data &player, ship_kind target)
{
    // only do this if there is a change
    if (player.kind != target)
    {
        // show then hide layers
        sprite_show_layer(player.player_sprite, static_cast<int>(target));
        sprite_hide_layer(player.player_sprite, static_cast<int>(player.kind));

        // remember what is currently shown
        player.kind = target;
    }
}

void handle_input(player_data &player)
{
    process_events();
    // Allow the player to switch ships
    // now , functions occure at 1 , 2 and 3
    if (key_typed(NUM_1_KEY))
        player_switch_to_ship(player, AQUARII);
    if (key_typed(NUM_2_KEY))
        player_switch_to_ship(player, GLIESE);
    if (key_typed(NUM_3_KEY))
        player_switch_to_ship(player, PEGASI);

    // Handle movement - rotating left/right and moving forward/back
    float dx = sprite_dx(player.player_sprite);
    float rotation = sprite_rotation(player.player_sprite);

    // Allow rotation with left/right keys
    if (key_down(LEFT_KEY))
        sprite_set_rotation(player.player_sprite, rotation - PLAYER_ROTATE_SPEED);
    if (key_down(RIGHT_KEY))
        sprite_set_rotation(player.player_sprite, rotation + PLAYER_ROTATE_SPEED);

    // Increase speed with up/down keys - typed to give step increases
    if (key_typed(DOWN_KEY))
        sprite_set_dx(player.player_sprite, dx - PLAYER_SPEED);
    if (key_typed(UP_KEY))
        sprite_set_dx(player.player_sprite, dx + PLAYER_SPEED);
}

//****    player.h    ****

#ifndef LOST_IN_SPACE_PLAYER
#define LOST_IN_SPACE_PLAYER

#include "splashkit.h"
#include <vector>

using namespace std;

#define PLAYER_SPEED 1.5
#define PLAYER_ROTATE_SPEED 3
#define SCREEN_BORDER 100

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum ship_kind
{
    AQUARII,
    GLIESE,
    PEGASI
};

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   player_sprite   The player's sprite - used to track position and movement
 * @field   score           The current score for the player
 * @field   kind            Current kind of player ship
 */
struct player_data
{
    sprite      player_sprite;
    ship_kind   kind;
    double      fuel_pct;
    double      battery;
    double      score;
    int         shield;
    int         cash;
    int         diamond;
    int         lives;
};

/**
 * Creates a new player in the centre of the screen with the default ship.
 * 
 * @returns     The new player data
 */
player_data new_player();

/**
 * Draws the player to the screen. 
 * 
 * @param player_to_draw    The player to draw to the screen
 */
void draw_player(const player_data &player_to_draw);

/**
 * Actions a step update of the player - moving them and adjusting the camera.
 * 
 * @param player_to_update      The player being updated
 */
void update_player(player_data &player_to_update);

/**
 * Read user input and update the player based on this interaction.
 * 
 * @param player    The player to update
 */
void handle_input(player_data &player);

#endif 