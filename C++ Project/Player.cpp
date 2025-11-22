#include "Player.h"
#include <iostream>
#include "Utils.h"

void Player::draw(char c) {
	gotoxy(x, y);
	std::cout << c;
}

void Player::move() {
	if (x + dirx < 0 || x + dirx > MAX_X) {
		dirx = -dirx;
	}
	if (y + diry < 0 || y + diry > MAX_Y) {
		diry = -diry;
	}
	x += dirx;
	y += diry;
}


