#pragma once
#include <iostream>
#include "Player.h"
#include "Utils.h"
#include "Screen.h"

static constexpr char START_KEY = '1';
static constexpr char INSTRUCTIONS_KEY = '8';
static constexpr char EXIT_KEY = '9';
enum class GameModes { MENU, INSTRUCTIONS, NEW_GAME, EXIT };
// class to contorl the the game
class Game {
private:
  
    // Game modes - ioptions of the game
	Screen  currentScreen;
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel();       //run the game
	void gameLoop();// main game loop       
	void handleDoor(Player& p);// handle door interaction
public:
    Game(); 
    void run(); 
	void GamePaused();
   
};
