#pragma once
class Screen;

enum class GameModes { MENU, INSTRUCTIONS, NEW_GAME, EXIT, RETURN_TO_THE_GAME}; // possible game modes
class UIScreens
{
public:
	// screen limits
	static constexpr int SCREEN_WIDTH = 80;
	static constexpr int HEIGHT = 25;
	//screens functions
	static void showMenu();
	static void showInstructions();
	static void showPauseScreen();
	static void showWinScreen();
	static void showExitMessage();

	static void showInventoryFullMessage(const Screen& currentScreen);
private:
	static void printScreen(const char** screen );
};