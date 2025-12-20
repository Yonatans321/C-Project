#include "Torch.h"
#include "Player.h"
#include "Screen.h"

bool Torch::playerHasTorch(const Player& p)
{
    return p.hasItem(TORCH_CHAR);
}

void Torch::handleTorch(const Player& p1, const Player& p2, Screen& screen)
{
    if (!screen.isDark())
    {
        screen.drawMap();
        return;
    }

    // אם החדר חשוך – נצייר עם אור
    screen.drawMapWithTorch(p1, p2);
}