#include "Item.h"

Type getItemTypeFromChar(char ch) {
	switch (ch) {
	case 'K':
		return Item::KEY;
	case '@':
		return Item::BOMB;
	case '!':
		return Item::TORCH;
	default:
		return Item::NONE;
	}
}
