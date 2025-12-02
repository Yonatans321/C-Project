#pragma once

enum class GameModes { MENU, INSTRUCTIONS, NEW_GAME, EXIT, RETURN_TO_THE_GAME};
class UIScreens
{
public:
	static constexpr int SCREEN_WIDTH = 80;
	static constexpr int HEIGHT = 25;

	static GameModes showMenu();
	static void showInstructions();
	static GameModes showPauseScreen();
	static void showWinScreen();
private:
	static void printScreen(const char* screen[HEIGHT]);
	static const char* MENU_SCREEN[HEIGHT];
	static const char*  INSTRUCTIONS_SCREEN[HEIGHT];
	static const char*  PAUSE_SCREEN[HEIGHT];
	static const char* WIN_SCREEN[HEIGHT];
};