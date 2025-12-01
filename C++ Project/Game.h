#pragma once
#include <iostream>
#include "Player.h"
#include "Utils.h"
#include "Screen.h"
#include <array>// for std::array (taken from AI)


static constexpr char START_KEY = '1';
static constexpr char INSTRUCTIONS_KEY = '8';
static constexpr char EXIT_KEY = '9';
enum class GameModes { MENU, INSTRUCTIONS, NEW_GAME, EXIT };
// class to contorl the the game
class Game {
private:
	// Levels
	static constexpr int MAX_LEVELS = 4;    
	std::array<Screen, MAX_LEVELS> gameScreens;
	int currentLevel = 0;

    // Game modes - ioptions of the game
	//Screen  currentScreen;
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel();       //run the game
	void gameLoop();// main game loop       
	void handleDoor(Player& p);// handle door interaction

	void loadScreens(int level);
	void showWinScreen();
	void changeLevel(int newLevel);

	void printStatus();
public:
    Game(); 
    void run(); 
	void GamePaused();
   
};
