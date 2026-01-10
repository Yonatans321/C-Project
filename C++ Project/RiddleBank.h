#pragma once
#include <string>
#include "Riddle.h"
#include "Player.h"
#include "Screen.h"
# include "Results.h"
# include "Steps.h"
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
    static constexpr char YES = 'Y';
    static constexpr char NO = 'N';
    static constexpr char HINT = 'H';
    static constexpr int BACKSPACE = 8;

    static constexpr size_t MAX_RIDDLES = 14;
    Riddle riddles[MAX_RIDDLES];
    size_t riddleCount = 0;
    bool loadedSuccessfully = false;
	Results* gameResults = nullptr;  // pointer to Results object
    size_t* eventTimerPtr = nullptr;           // pointer timer for events
	Steps* recordedSteps = nullptr; // pointer to Steps riddles

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
	// Attach positions to unsolved riddles based on '?' locations on the screen
    void attachPositionToRoom(Screen& screen);

    //Riddle Logic Functions
    
     // Add a riddle to the bank (stores a copy)
    void addRiddle(const Riddle& r);

    // Non-interactive check: supply an answer string and receive outcome.
    RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);
	// Helper function to get user answer for a riddle
    std::string getUserAnswer(int bx, int answerLine, int answerInputLine,
        int hintorResultLine, Riddle* r);
	// Helper function to display the riddle question inside a box
    void displayRiddleQuestion(Riddle* r, int bx, int by);

    // Interactive handling of a riddle when a player steps on a riddle tile
    void handleRiddle(Player& player, Screen& screen, int level);
	// Reset all riddles to unsolved state
    void resetAllRiddles();

    void attachResults(Results* results, size_t* timerPtr);
    void attachSteps(Steps* steps);
};
