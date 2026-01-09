#pragma once
#include "Game.h"

class LoadGame : public Game { // derived from Game
public:
	LoadGame(bool silent = false) : isSilentMode(silent) {} // constructor
	virtual ~LoadGame() = default; // virtual destructor

	void run() override; // override run method

protected:
	bool isSilentMode; // flag for silent mode

    // Replay game loop that ignores user input
    void replayGameLoop();
};