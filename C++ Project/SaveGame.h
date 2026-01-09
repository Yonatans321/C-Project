#pragma once
#include "Game.h"

class SaveGame : public Game {
public:
    SaveGame() = default;
    virtual ~SaveGame() = default;

    void run() override;
};