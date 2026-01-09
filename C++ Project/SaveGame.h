#pragma once
#include "Game.h"

extern bool SAVE_MODE;

class SaveGame : public Game {
public:
    SaveGame() = default;
    virtual ~SaveGame() = default;

    void run() override;

protected:
    // Custom game loop that records events
    void saveGameLoop();

    // Override methods to record events
    bool checkGameOver() override;
    bool checkLevel() override;
};