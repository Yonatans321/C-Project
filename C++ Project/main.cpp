// main function
#include "Game.h"
#include "SaveGame.h"
#include "LoadGame.h"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {

    // No arguments - normal game with menu
    if (argc == 1) {
        SAVE_MODE = false;
        Game game;
        game.run();
        return 0;
    }

    // Parse command-line arguments
    if (strcmp(argv[1], "-save") == 0) {
        // ===== SAVE MODE =====
        SAVE_MODE = true;
        SaveGame game;
        game.run();
    }
    else if (strcmp(argv[1], "-load") == 0) {
        // ===== LOAD MODE =====
        SAVE_MODE = false;
        bool silent = false;

        // Check for -silent flag
        if (argc > 2 && strcmp(argv[2], "-silent") == 0) {
            silent = true;
        }

        LoadGame game(silent);
        game.run();
    }
    else {
        std::cout << "ERROR: Unknown argument '" << argv[1] << "'\n";
    }

    return 0;
}