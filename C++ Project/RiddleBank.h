#pragma once
#include <string>
#include "Riddle.h"
#include "Player.h"
#include "Rooms.h"
#include "Screen.h"


// Stores and manages multiple riddles
// Possible outcomes when asking or checking a riddle

enum class RiddleOutcome {
    Correct,
    Incorrect,
    AlreadySolved,
    NotFound
};

class RiddleBank
{
private:
	static constexpr size_t MAX_RIDDLES = 10;
	Riddle riddles[MAX_RIDDLES];
	int riddleCount = 0;

public:
	// Constructor////
    RiddleBank();

	// Riddle Retrieval Functions
    
	// Retrieve a riddle by its ID (returns nullptr if not found)
	Riddle* getRiddleById(int riddleID);

	// Retrieve a riddle by its position (x, y) on the screen
	Riddle* getRiddleAt(int x, int y);

    void attachPositionToRoom(const char room[][Screen::WIDTH], int roomWidth, int roomHeight);

	//Riddle Logic Functions////
    
     // Add a riddle to the bank (stores a copy)
    void addRiddle(const Riddle& r);

    // Non-interactive check: supply an answer string and receive outcome.
    RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);

    // Ask the riddle interactively (prints question and reads a full line).
    // Returns outcome; caller should update score / life based on the result.
    RiddleOutcome askRiddleInteractive(int riddleID);

   

	void printAllRiddles() const;
    void handleRiddle(Player& player, Screen& screen, int level);
};
