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