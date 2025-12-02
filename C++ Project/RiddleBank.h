#pragma once
#include <string>
#include "Riddle.h"
#include "Player.h"
#include "Rooms.h"
#include <iostream>
#include <conio.h>
#include "RoomScreenManager.h"
#include <windows.h>
#include "Screen.h"
#include "Utils.h"


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
    RiddleBank();
	// Retrieve a riddle by its ID (returns nullptr if not found)

	Riddle* getRiddleById(int riddleID);
    // Add a riddle to the bank (stores a copy)
    void addRiddle(const Riddle& r);

    // Ask the riddle interactively (prints question and reads a full line).
    // Returns outcome; caller should update score / life based on the result.
    RiddleOutcome askRiddleInteractive(int riddleID);

    // Non-interactive check: supply an answer string and receive outcome.
    RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);

	void printAllRiddles() const;
	void clearInputBuffer();
    void handleRiddle(Player& player, Screen& screen, RoomScreenManager& ui, int level);
};
