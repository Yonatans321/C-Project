#pragma once
class Switch
{
private:
	enum State
	{
		OFF,
		ON
	};
	State state;

public:	
	Switch(State initialState = OFF) : state(initialState) {}
	void toggle() {
		if (state == OFF) {
			state = ON;
		}
		else {
			state = OFF;
		}
	}
	State getState() const {
		return state;
	}
	char getChar() const {
		if (state == OFF) {
			return '/';
		}
		else {
			return '\\';
		}
	}
	static State stateFromChar(char c) {
		if(c == '/') {
			return OFF;
		}
		else {
			return ON;
		}
			
		}
};

