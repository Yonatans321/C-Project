#include "Player.h"
#include <cctype>
#include "Direction.h"

void Player::move() {
	Point nextPosition = position;	
	nextPosition.move();
	if (!screen.isWall(nextPosition))
	{
		position = nextPosition;
	}
}
void Player::keyPressed(char ch) {
	size_t index = 0;
	for (char key : keys) {
		if (std::tolower(key) == std::tolower(ch)) {
			position.changeDirection(Direction::directions[index]);
			break;
		}
		++index;
	}
}
void Player::erase() const {
	position.erase();
}

