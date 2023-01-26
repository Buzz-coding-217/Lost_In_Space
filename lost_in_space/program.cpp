/*
Name :- Shravel Sharma
Roll no :- 2110994791
*/
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