#pragma once
#include <string>
#include "Riddle.h"
#include "Player.h"
#include "Screen.h"
# include "Results.h"
# include "Steps.h"

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
    Results* gameResults = nullptr;
    size_t* eventTimerPtr = nullptr;
    Steps* recordedSteps = nullptr;
    bool isLoadMode = false;
    bool isSilentLoadMode = false;
    char getRiddleInputChar();
    void processLoadModeRiddle(Player& player, Screen& screen, Riddle* r, int x, int y);
public:
    RiddleBank();
    bool isLoaded() const { return loadedSuccessfully; }
    Riddle* getRiddleById(int riddleID);
    Riddle* getRiddleAt(int x, int y);
    void attachPositionToRoom(Screen& screen);
    void addRiddle(const Riddle& r);
    RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);
    std::string getUserAnswer(int bx, int answerLine, int answerInputLine,
        int hintorResultLine, Riddle* r);
    void displayRiddleQuestion(Riddle* r, int bx, int by);
    void handleRiddle(Player& player, Screen& screen, int level);
    void resetAllRiddles();
    void attachResults(Results* results, size_t* timerPtr);
    void attachSteps(Steps* steps, bool loadmode = false);
    void setSilentMode(bool silent) { isSilentLoadMode = silent; }
};