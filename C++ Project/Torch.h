#pragma once
class Player;
class Screen;

class Torch
{
public:
    static constexpr char TORCH_CHAR = '!';

    static bool playerHasTorch(const Player& p);
    static void handleTorch(const Player& p1, const Player& p2, Screen& screen);
};