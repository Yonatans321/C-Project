#pragma once
#include <string>
#include "Riddle.h"



// Generatred AI functions for RiddleBank class
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
	void Hints() const;
};
