#include "Game.h"

int main(int argc, char** argv)
{
    Game* game = new Game();

    if(game->Init() == false)
    {
        return 1;
    }

    game->Run();

    delete game;

    return 0;
}
