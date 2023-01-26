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