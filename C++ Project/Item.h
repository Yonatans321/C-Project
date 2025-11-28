#pragma once
class Item
{ public:
	enum Type {
		NONE,
		KEY,
		BOMB,
		TORCH,
		RIDDLE
	};
	Type type;
	int id;
	Item() : type(NONE), id(-1) {}
	Item(Type t, int identifier) : type(t), id(identifier) {}
};
Type getItemTypeFromChar(char ch);

bool isRiddleChar(char ch)
{
	return ch == '?';
}

bool isDoorChar(char ch)
{
	return ch >= '1' && ch <= '9';
}
bool DorID(char c)
{
	return c - '0';
}
bool isSwitch(char ch)
{
	return ch == '\' || ch == '/' ;
}