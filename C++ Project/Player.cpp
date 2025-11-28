#include "Player.h"
#include <cctype>
#include "Direction.h"

// moving and drawing functions
void Player::draw() { // draw player at current position
	position.draw();
}
void Player::erase() const { // erase player from current position
	position.erase();
}
int Player::getX() const { // get player's X position
	return position.getX();
}
int Player::getY() const { // get player's Y position
	return position.getY();
}
char Player::getChar() const { // get player's character representation
	return position.getChar();
}
// move player in the current direction if no wall is there - taken from lab 4
void Player::move() {
	Point nextPosition = position;	
	nextPosition.move();
	if (!screen.isWall(nextPosition))
	{
		position = nextPosition;
	}
}
// change direction based on key pressed - Taken from lab 4
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

bool Player::hasItem() const { // check if player has any item
	return heldItem != 0;
}

bool Player::hasItem(char item) const { // check if player has specific item
	return heldItem == item;
}

bool Player::hasKey() const { // check if player has a key
	return heldItem == 'K';
}
char Player::getHeldItem() const { // get the held item
	return heldItem;
}
int Player::getItemId() const { // get the held item id
	return itemId;
}
void Player::DropItem() { // drop the held item
	heldItem = 0;
	itemId = -1;
}
void Player::GrabItem(char item, int id) { // grab an item if not already holding one
	if (hasItem())
		return;
	heldItem = item;
	itemId = id;
}