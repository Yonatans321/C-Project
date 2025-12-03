#pragma once

enum class GameModes { MENU, INSTRUCTIONS, NEW_GAME, EXIT, RETURN_TO_THE_GAME};
class UIScreens
{
public:
	static constexpr int SCREEN_WIDTH = 80;
	static constexpr int HEIGHT = 25;

	static void showMenu();
	static void showInstructions();
	static void showPauseScreen();
	static void showWinScreen();
private:
	static void printScreen(const char** screen );
};