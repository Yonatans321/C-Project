#pragma once
class Player; // Forward declaration
class Screen; // Forward declaration

class Torch
{
public:
    static constexpr char TORCH_CHAR = '!';
    static bool playerHasTorch(const Player& p);
    static void handleTorch(const Player& p, Screen& screen);
};
