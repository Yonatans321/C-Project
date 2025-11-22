#pragma once
#include "Player.h"
#include "Utils.h" 



// class that start the game
class Game {
private:
  static constexpr char START_KEY = 1;
  static constexpr char INSTRUCTIONS_KEY = 8;
  static constexpr char EXIT_KEY = 9;
    // Game modes - ioptions of the game
    enum GameModes {
        MENU,
        NEW_GAME,
        INSTRUCTIONS,
        EXIT
    };
    GameModes curr_status;
    Player player1;         
    Player player2;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel();       //run the game
    void gameLoop();       
   
public:
    Game(); 
    void run(); 
   
};
