#include "Bomb.h"
#include "Screen.h"
#include "Player.h"
#include "Results.h"
#include "Utils.h"
#include <cmath> // For std::abs
#include <iostream>

// Update signature to match Bomb.h (3 arguments)
void Bomb::explode(Screen& screen, Player& p1, Player& p2,bool isInCurrentRoom)
{
    // 1. Remove the bomb from logical map and physical screen
    screen.setCharAt(position, ' ');
    if (isInCurrentRoom) {
        position.draw(' ');
    }
    

	int centerX = position.getX(); // Center of explosion
	int centerY = position.getY(); // Center of explosion

    // 2. Destroy 'w' walls and '*' obstacles in range 1 (3x3 area)
    for (int y = -3; y <= 3; y++)
    {
        for (int x = -3; x <= 3; x++)
        {
            if (x == 0 && y == 0) continue; // Skip the center

			int targetX = centerX + x; // Target cell coordinates
			int targetY = centerY + y; 

            char targetChar = screen.getCharAt(targetX, targetY);
			if (targetChar == 'w' || targetChar == '*') // Check for walls and obstacles
            {
				screen.setCharAt(targetX, targetY, ' '); // Remove from logical map
                if (isInCurrentRoom) {
					Point(targetX, targetY).draw(' '); // Remove from physical screen
                }
                
            }
        }
    }
    if (isInCurrentRoom) 
    {
        // 3. Check for player damage in range 3 (Chebyshev distance)
        // If player is within 3 cells (including diagonals), they lose a life
        if (std::abs(p1.getX() - centerX) <= 3 && std::abs(p1.getY() - centerY) <= 3) {
            p1.loseLife();
			if (gameResults != nullptr)  // if are writing results
            {
                gameResults->addLifeLost(eventTimer, PlayerType::Player1);
            }
        }
        if (std::abs(p2.getX() - centerX) <= 3 && std::abs(p2.getY() - centerY) <= 3) {
            p2.loseLife();
            if (gameResults != nullptr)  // if are writing results
            {
                gameResults->addLifeLost(eventTimer, PlayerType::Player2);
            }
        }
     }
}


bool Bomb::tick(Screen& screen, Player& p1, Player& p2, int currentRoomID)
{
    
    // Decrease internal timer
    timer--;

	bool isInCurrentRoom = (this->roomID == currentRoomID);
 //   if(isInCurrentRoom)
 //   {
 //       // Draw the bomb only if it's in the current room
 //       gotoxy(position.getX(), position.getY());
 //       setColor(COLOR_RED);  // Red color for bomb
 //       std::cout << '@';
 //       resetColor();
	//}


   
    if (timer <= 0)
    {
        explode(screen, p1, p2, isInCurrentRoom); // Trigger explosion with player references
        return true;     // Bomb has finished its life cycle
    }
    return false;        // Bomb is still active


}

void Bomb::attachResults(Results* results, size_t timer)
{
    gameResults = results;
    eventTimer = timer;
}