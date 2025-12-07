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

// class to contorl the the game

class Game {
private:
	// Levels
	static constexpr int MAX_LEVELS = 4;    
	std::array<Screen, MAX_LEVELS> gameScreens;
	
    
    
    // ============================
//       CONSTRUCTOR
// ============================

    Game()
        : player1(Point(2, 2, Direction::directions[Direction::STAY], '&'),
            { 'W','D','X','A','S','E' }),
        player2(Point(77, 2, Direction::directions[Direction::STAY], '$'),
            { 'I','L','M','J','K','O' })
    {
        hideCursor();
        currStatus = GameModes::MENU;

        gameScreens[0].loadMap(0);
        player1.setScreen(gameScreens[0]);
        player2.setScreen(gameScreens[0]);
    }

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

public:
    Game(); 
    void run(); 
    bool checkLevel();
};
