#include "Game.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Player.h"
#include "Utils.h"

using namespace std;


void Game::showMenu() {

	cls();
	gotoxy(25, 5);
	cout << "==============================" << endl;
	gotoxy(25, 6);
	cout << "      TEXT ADVENTURE GAME 2025      " << endl;
	gotoxy(25, 7);
	cout << "==============================" << endl;
	gotoxy(25, 9);
	cout << "(1) Start a new game" << endl;
	gotoxy(25, 10);
	cout << "(8) Present instructions and keys" << endl;
	gotoxy(25, 11);
	cout << "(9) EXIT" << endl;

	bool valid = false;

	while (!valid) {
		char input = _getch();

		if (input == START_KEY) {
			curr_status = NEW_GAME;
			valid = true;
		}
		else if (input == INSTRUCTIONS_KEY) {
			curr_status = INSTRUCTIONS;
			valid = true;
		}
		else if (input == EXIT_KEY) {
			curr_status = EXIT;
			valid = true;
		}

	}
}





void Game::run() {
	
	while (curr_status != EXIT) {
		if (curr_status == MENU) {
			showMenu();
		}

		else if (curr_status == INSTRUCTIONS) {
			showInstructions();
		}
			
		else  {
			initLevel();
			gameLoop();
		}
	}
}



