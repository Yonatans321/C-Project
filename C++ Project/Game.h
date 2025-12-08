#pragma once
#include <array> // for std::array (taken from AI)
#include "Player.h"
#include "Screen.h"
#include "RiddleBank.h"
#include "UIScreens.h"
#include "Utils.h"


static constexpr char START_KEY = '1';
static constexpr char INSTRUCTIONS_KEY = '8';
static constexpr char EXIT_KEY = '9';

class Game {
private:
	// Levels
	static constexpr int MAX_LEVELS = 4;    
	std::array <Screen, MAX_LEVELS> gameScreens;
	
    int currentLevel = 0;
	char activeDoor = ' '; // to track last door used
    bool newGameStarted = false;

    // Game modes - options of the game
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;
	RiddleBank riddleBank;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel();  //run the game
	void gameLoop();// main game loop       
	bool handleTile(Player& player);
	void showWinScreen();
    void placePlayersAtEntrance();
    void resetGame();

    void handlePause(Screen& currentScreen, bool& gameRunning);

public:
    Game(); 
    void run(); 
    bool checkLevel();
    static bool pauseRequestedFromRiddle;
};