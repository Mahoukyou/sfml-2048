#include "game.h"

int main()
{
	Game& game = Game::instance();
	game.main_loop();
}