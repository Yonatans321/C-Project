#pragma once
#include "Game.h"

class LoadGame : public Game {
public:
    LoadGame(bool silent = false) : isSilentMode(silent) {}
    virtual ~LoadGame() = default;

    void run() override;

protected:
    bool isSilentMode;

    // Replay game loop that ignores user input
    void replayGameLoop();
};