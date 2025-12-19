#include "Player.h"
#include <cctype>
#include "Direction.h"
#include "Screen.h"
#include "Door.h"
#include "Key.h"

// moving and drawing functions
 // draw player at current position
void Player::draw() {
	if (active) 
	position.draw();
}
// erase player from current position
void Player::erase() const
{
	int x = position.getX();
	int y = position.getY();

	char tile = screen->getCharAt(x, y);

	//if on a switch - redraw the switch
	if (tile == '/' || tile == '\\')
	{
		//
		gotoxy(x, y);
		if (tile == '/')
		{
			setColor(COLOR_LIGHT_GREEN); // COLOR GREEN
		}
		else 
		{
			setColor(COLOR_LIGHT_RED);  // COLOR RED
		}
		std::cout << tile; //paint the switch
		resetColor();
		return;
	}

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
  prevPos=position;
	if (!active) return;
	Point nextPosition = position;	
	nextPosition.move();
	char nextCell = screen->getCharAt(nextPosition);
	if (screen->isWall(nextPosition))
	{
		return;
	}
	if (Door::isDoorChar(nextCell)) {
		return;
	}
	position = nextPosition;
}

void Player::rememberPosition() { // remember current position
	prevPos = position;
}

void Player::stepBack() { // move player back to previous position
	position = prevPos;
	position.changeDirection(Direction::directions[Direction::STAY]);
}

// change direction based on key pressed - Taken from lab 4
void Player::keyPressed(char ch) 
{
	char lower = std::tolower(ch);
	if (lower==std::tolower(keys[5])&& heldItem !=0)
	{
		dropHeldItem();
		return;
	}
	size_t index = 0;
	for (char key : keys) {
		if (std::tolower(key) == std::tolower(ch))
		{
			position.changeDirection(Direction::directions[index]);
			break;
		}
		++index;
	}
}
void Player::setScreen(Screen& newScreen) { // set player's current screen
	this->screen = &newScreen;
}


bool Player::hasItem() const { // check if player has any item
	return heldItem != 0;
}

bool Player::hasItem(char item) const { // check if player has specific item
	return heldItem == item;
}

char Player::getHeldItem() const { // get the held item
	return heldItem;
}
int Player::getItemId() const { // get the held item id
	return itemId;
}
void Player::DropItem() // drop the held item 
{ 
	heldItem = 0;
	itemId = -1;
}
void Player::GrabItem(char item, int id) { // grab an item if not already holding one
	if (hasItem())
		return;
	heldItem = item;
	itemId = id;
}
bool Player::useKeyForDoor( char doorChar) const  // use key for a door
{ // check if player hasn't a key
	if (heldItem == 'K')
	{
		return true;
	}
		return false;
	
}
		
void Player::keyUsed() { // remove the used key
	
	DropItem();
}
// points and life functions
void Player::addPoints(int pts)
{
	points += pts;
}
void Player::losePoints(int pts)
{
	points -= pts;
}
int Player::getScore() const
{
	return points;
}
void Player::addLives()
{
	lives++;
}
void Player::loseLife()
{
	if (lives>0)
	{
		lives--;
	}
}
int Player::getLives() const
{
	return lives;
}
bool Player::isDead() const
{
	return lives <= 0;

}
void Player::setPosition(const Point& pos) {
	position = pos;
}
//item function to drop item
void Player::dropHeldItem()
{
	if (heldItem == 0 || screen == nullptr)
		return;

	int dropX, dropY;
	Direction direction = position.getDirection();

	// if the player is not moving
	if (direction.getX()==0 && direction.getY()==0)
	{
		// default to right
		dropX = position.getX() + 1;
		dropY = position.getY();
		

		// if right blocked - try left
		if (screen->getCharAt(dropX, dropY) != ' ')
		{
			dropX = position.getX() - 1;
			dropY = position.getY();
		}

		//if still blocked -  try down
		if (screen->getCharAt(dropX, dropY) != ' ')
		{
			dropX = position.getX();
			dropY = position.getY() + 1;
		}

		// if still blocked -  try up
		if (screen->getCharAt(dropX, dropY) != ' ')
		{
			dropX = position.getX();
			dropY = position.getY() - 1;
		}
	}
	else
	{
		// if moving - drop in the previous position
		dropX = prevPos.getX();
		dropY = prevPos.getY();
	}
	// check if drop position is empty
	if (screen->getCharAt(dropX, dropY) == ' ') {
		if (getHeldItem() == '@')
		{
			bombDroppedRequest = true;
			lastDropPos = Point(dropX, dropY);
		}
		else
		{
			// put the item on the screen
			screen->setCharAt(dropX, dropY, heldItem);
		}
		
		DropItem();// drop from the player
	}	
}



