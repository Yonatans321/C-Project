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
    static constexpr int ESC = 27;
    static constexpr size_t MAX_RIDDLES = 14;
    Riddle riddles[MAX_RIDDLES];
    size_t riddleCount = 0;
    bool loadedSuccessfully = false;

public:
    // Constructor
    RiddleBank();

    // Check if riddles were loaded successfully
    bool isLoaded() const { return loadedSuccessfully; }

    // Riddle Retrieval Functions
    // Retrieve a riddle by its ID (returns nullptr if not found)
    Riddle* getRiddleById(int riddleID);

    // Retrieve a riddle by its position (x, y) on the screen
    Riddle* getRiddleAt(int x, int y);

    void attachPositionToRoom(Screen& screen);

    //Riddle Logic Functions
     // Add a riddle to the bank (stores a copy)
    void addRiddle(const Riddle& r);

    // Non-interactive check: supply an answer string and receive outcome.
    RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);

    // Interactive handling of a riddle when a player steps on a riddle tile
    void handleRiddle(Player& player, Screen& screen, int level);

    void resetAllRiddles();
};
