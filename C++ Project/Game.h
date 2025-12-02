#pragma once
#include <array> // for std::array (taken from AI)
#include "Player.h"
#include "Screen.h"
#include "RiddleBank.h"
#include "RoomScreenManager.h"
#include "UIScreens.h"
#include "Utils.h"


static constexpr char START_KEY = '1';
static constexpr char INSTRUCTIONS_KEY = '8';
static constexpr char EXIT_KEY = '9';

// class to contorl the the game

class Game {
private:
	// Levels
	static constexpr int MAX_LEVELS = 4;    
	std::array<Screen, MAX_LEVELS> gameScreens;
	int currentLevel = 0;

    // Game modes - options of the game
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;
	RiddleBank riddleBank;
	RoomScreenManager ui;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel();       //run the game
	void gameLoop();// main game loop       
	void handleTile(Player& player);
	void showWinScreen();
	

	void loadScreens(int level);
	void changeLevel(int newLevel);

	void printStatus();
public:
    Game(); 
    void run(); 
	void GamePaused();
	void handleRiddle(Player& player);
	void drawBox(int x, int y, int width, int height);
	char getYesNo();
	void clearBox(int x, int y, int width, int height);
	void clearInputBuffer();
	void handleTile(Player& player);
	void drawAnimatedBox(int x, int y, int w, int h);
	void closeAnimatedBox(int x, int y, int w, int h);
};
