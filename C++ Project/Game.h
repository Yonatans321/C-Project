#pragma once
#include <array> // for std::array (taken from AI)
#include "Player.h"
#include "Screen.h"
#include "RiddleBank.h"
#include "UIScreens.h"
#include "Utils.h"

class Game {
private:
	// Constants
    static constexpr int ESC = 27;
    static constexpr int GAME_DELAY = 80;
	// Player starting positions
    static constexpr int P1_START_X = 2;
    static constexpr int P1_START_Y = 2;
    static constexpr int P2_START_X = 77;
    static constexpr int P2_START_Y = 2;
	// Menu keys
    static constexpr char START_KEY = '1';
    static constexpr char INSTRUCTIONS_KEY = '8';
    static constexpr char EXIT_KEY = '9';

    // Levels
    static constexpr int MAX_LEVELS = 4;
	std::array <Screen, MAX_LEVELS> gameScreens; // array to hold game screens (helped by AI)
	
    int currentLevel = 0;
	char activeDoor = ' '; // to track last door used
   

    // Game modes - options of the game
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;
	RiddleBank riddleBank;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel(int specificDoor = -1);  //run the game
	void gameLoop();// main game loop       
	bool handleTile(Player& player);// handle tile interaction for a player
	void showWinScreen();
	void placePlayersAtEntrance(int apecificDoor = -1);// place players next to the door they entered from (manager)
	void resetGame();// reset game to initial state
	void placeNextToDoor(const Point& targetDoorPos);// place player next to a specific door (helper)
    void handlePause(Screen& currentScreen, bool& gameRunning);
	bool checkLevel();// check if level is completed
public:
    Game(); 
    void run(); 
    static bool pauseRequestedFromRiddle;
};