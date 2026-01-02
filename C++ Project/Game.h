#pragma once
#include <array> // for std::array (taken from AI)
#include "Player.h"
#include "Screen.h"
#include "RiddleBank.h"
#include "UIScreens.h"
#include "Utils.h"
#include "Bomb.h"
#include "RoomMeta.h"
#include <vector>   // vector for storing screen file names
#include <string>   // for string handling
#include <filesystem> // for directory iteration

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
    static constexpr char Colors_ON_OFF = '2';

	void drawCurrentScreen(); // draw the current screen
	void redrawGame();         // redraw game elements
	void updateBomb();        // update bomb state
	void drawActiveBomb();   // draw the active bomb
	void updatePlayers();      // draw both players
	bool checkGameOver();      // check if game over
	void updateDisplay();	 // update players and status bar


	std::vector<std::string> screenFileNames; // list of screen file names
	Screen currentScreen;             // current game screen      
	RoomMeta currentRoomMeta;       // current room metadata
	std::vector<Screen> allLevels;    // all game levels
	int currentLevelIdx = 0;         // current level index         

	//function to get all screen file names from the directory
    void getAllScreenFileNames(std::vector<std::string>& vec_to_fill);

    //int currentLevel = 0;
	char activeDoor = ' '; // to track last door used
   
    // Game modes - options of the game
    GameModes currStatus = GameModes::MENU;
    Player player1;         
    Player player2;
	RiddleBank riddleBank;

    
    void showMenu();        
    void showInstructions(); 
    void initLevel(const std::string& filename, int specificDoor = -1);  //run the game
	void gameLoop();// main game loop       
	bool handleTile(Player& player);// handle tile interaction for a player
	void showWinScreen();
	void placePlayersAtEntrance(int apecificDoor = -1);// place players next to the door they entered from (manager)
	void resetGame();// reset game to initial state
	void placeNextToDoor(const Point& targetDoorPos);// place player next to a specific door (helper)
    void handlePause(Screen& currentScreen, bool& gameRunning);
	bool checkLevel();// check if level is completed

    Bomb* activeBomb = nullptr;
    
public:
    Game(); 
    void run(); 
    static bool pauseRequestedFromRiddle;
};