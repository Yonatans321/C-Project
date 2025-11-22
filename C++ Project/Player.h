#pragma once
class Player
{
		int x, y;
		int dirx, diry;
		char ch;
		 
		static constexpr int MAX_X = 79;
		static constexpr int MAX_Y = 24;
	public:
		Player(int x1, int y1, int dirx1, int diry1, char c) {
			x = x1;
			y = y1;
			dirx = dirx1;
			diry = diry1;
			ch = c;
		}
		void draw() {
			draw(ch);
		}
		void draw(char c);
		void move();

		int getX() const {
			return x;
		}
		int getY() const {
			return y;
		}
};


