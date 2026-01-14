
#include "GameStateManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Utils.h"

const std::string GameStateManager::FOLDER = "saves";

std::string GameStateManager::getMetaFilename(const std::string& saveName) {
    return FOLDER + "/" + saveName + ".save";
}

bool GameStateManager::save(const StateSnapshot& state, const std::string& saveName) {
    try {
        if (!std::filesystem::exists(FOLDER))
            std::filesystem::create_directory(FOLDER);
    }
    catch (...) {
        return false;
    }

    std::string filename = getMetaFilename(saveName);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    // כתוב נתוני שחקנים
    file.write((char*)&state.p1_x, sizeof(int));
    file.write((char*)&state.p1_y, sizeof(int));
    file.write((char*)&state.p1_lives, sizeof(int));
    file.write((char*)&state.p1_score, sizeof(int));
    file.write(&state.p1_item, 1);
    file.write((char*)&state.p1_item_id, sizeof(int));

    file.write((char*)&state.p2_x, sizeof(int));
    file.write((char*)&state.p2_y, sizeof(int));
    file.write((char*)&state.p2_lives, sizeof(int));
    file.write((char*)&state.p2_score, sizeof(int));
    file.write(&state.p2_item, 1);
    file.write((char*)&state.p2_item_id, sizeof(int));

    // כתוב נתוני משחק
    file.write((char*)&state.level, sizeof(int));
    file.write((char*)&state.timer, sizeof(int));
    file.write((char*)&state.timer_active, 1);
    file.write((char*)&state.switches_on, 1);

    // כתוב מצב דלתות
    for (int i = 0; i < 10; i++)
        file.write((char*)&state.door_open[i], 1);

    // כתוב מצב המסך כולו
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 80; x++) {
            file.write((char*)&state.mapData[y][x], 1);
        }
    }

    // כתוב רשימת קבצים
    size_t len = state.screens.length();
    file.write((char*)&len, sizeof(size_t));
    file.write(state.screens.c_str(), len);

    file.close();
    return true;
}

bool GameStateManager::load(StateSnapshot& state, const std::string& saveName) {
    std::string filename = getMetaFilename(saveName);

    if (!exists(saveName)) {
        return false;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    // קרא נתוני שחקנים
    file.read((char*)&state.p1_x, sizeof(int));
    file.read((char*)&state.p1_y, sizeof(int));
    file.read((char*)&state.p1_lives, sizeof(int));
    file.read((char*)&state.p1_score, sizeof(int));
    file.read(&state.p1_item, 1);
    file.read((char*)&state.p1_item_id, sizeof(int));

    file.read((char*)&state.p2_x, sizeof(int));
    file.read((char*)&state.p2_y, sizeof(int));
    file.read((char*)&state.p2_lives, sizeof(int));
    file.read((char*)&state.p2_score, sizeof(int));
    file.read(&state.p2_item, 1);
    file.read((char*)&state.p2_item_id, sizeof(int));

    // קרא נתוני משחק
    file.read((char*)&state.level, sizeof(int));
    file.read((char*)&state.timer, sizeof(int));
    file.read((char*)&state.timer_active, 1);
    file.read((char*)&state.switches_on, 1);

    // קרא מצב דלתות
    for (int i = 0; i < 10; i++)
        file.read((char*)&state.door_open[i], 1);

    // קרא מצב המסך כולו
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 80; x++) {
            file.read((char*)&state.mapData[y][x], 1);
        }
    }

    // קרא רשימת קבצים
    size_t len;
    file.read((char*)&len, sizeof(size_t));
    state.screens.resize(len);
    file.read(&state.screens[0], len);

    file.close();
    return true;
}

std::vector<std::string> GameStateManager::listSaves() {
    std::vector<std::string> result;

    if (!std::filesystem::exists(FOLDER))
        return result;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(FOLDER)) {
            if (entry.path().extension() == ".save") {
                std::string name = entry.path().stem().string();
                result.push_back(name);
            }
        }
    }
    catch (...) {}

    return result;
}

bool GameStateManager::exists(const std::string& saveName) {
    return std::filesystem::exists(getMetaFilename(saveName));
}

bool GameStateManager::deleteSave(const std::string& saveName) {
    std::string filename = getMetaFilename(saveName);

    if (!exists(saveName)) {
        return false;
    }

    try {
        std::filesystem::remove(filename);
        return true;
    }
    catch (...) {
        return false;
    }
}

// ===== תפריט שמירה =====
bool GameStateManager::showSaveMenu(const StateSnapshot& snap) {
    while (true) {
        cls();
        gotoxy(15, 10);
        std::cout << "Enter save name: ";
        std::string saveName;
        std::getline(std::cin, saveName);

        saveName.erase(0, saveName.find_first_not_of(" "));
        saveName.erase(saveName.find_last_not_of(" ") + 1);

        if (saveName.empty()) {
            return false;
        }

        // ✅ בדוק שלא מתחיל ב-0 וגם לא בדיוק 0
        if (saveName[0] == '0' || saveName == "0") {
            cls();
            gotoxy(15, 10);
            std::cout << "Save names cannot start with 0!";
            Sleep(1500);
            continue;
        }

        // בדוק אם קיימת
        if (exists(saveName)) {
            cls();
            gotoxy(15, 10);
            std::cout << "Save '" << saveName << "' already exists!";
            gotoxy(15, 11);
            std::cout << "Overwrite? (Y/N): ";
            char ans = _getch();
            if (ans != 'Y' && ans != 'y') {
                continue;
            }
        }

        // שמור
        if (save(snap, saveName)) {
            cls();
            gotoxy(20, 10);
            std::cout << "Game saved as '" << saveName << "'!";
            Sleep(1500);
            return true;
        }
        return false;
    }
}

// ===== תפריט טעינה =====
StateSnapshot* GameStateManager::showLoadMenu() {
    auto saves = listSaves();

    while (true) {
        cls();
        gotoxy(15, 3);
        std::cout << "=== LOAD GAME ===";

        if (saves.empty()) {
            gotoxy(15, 5);
            std::cout << "No saved games found!";
            gotoxy(15, 7);
            std::cout << "Press any key...";
            waitForKey();
            return nullptr;
        }

        gotoxy(15, 5);
        std::cout << "Available saves:";
        for (size_t i = 0; i < saves.size(); i++) {
            gotoxy(15, 7 + i);
            std::cout << (i + 1) << ". " << saves[i];
        }

        gotoxy(15, 9 + saves.size());
        std::cout << "Press [0] to DELETE FILE";
        gotoxy(15, 10 + saves.size());
        std::cout << "Press [ESC] to return to menu";
        gotoxy(15, 11 + saves.size());
        std::cout << "Enter save name to load: ";

        // ✅ קרא קלט עם ESC ישיר
        std::string input;
        while (true) {
            if (_kbhit()) {
                char ch = _getch();

                // ✅ ESC יוצא מיד
                if (ch == 27) {  // 27 = ESC
                    return nullptr;
                }

                // ENTER - סיום קלט
                if (ch == '\r') {
                    break;
                }

                // תו רגיל - הוסף לקלט
                if (ch >= 32 && ch <= 126) {  // תו הדפוס
                    input += ch;
                    std::cout << ch;
                }

                // BACKSPACE
                if (ch == 8) {
                    if (!input.empty()) {
                        input.pop_back();
                        std::cout << "\b \b";
                    }
                }
            }
        }

        std::cout << "\n";

        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);

        if (input.empty()) {
            continue;
        }

        // ===== DELETE מצב =====
        if (input == "0") {
            cls();
            gotoxy(15, 5);
            std::cout << "=== DELETE FILE ===";
            gotoxy(15, 7);
            std::cout << "Available saves:";
            for (size_t i = 0; i < saves.size(); i++) {
                gotoxy(15, 9 + i);
                std::cout << (i + 1) << ". " << saves[i];
            }

            gotoxy(15, 11 + saves.size());
            std::cout << "Enter save name to delete: ";
            std::string nameToDelete;
            std::getline(std::cin, nameToDelete);

            nameToDelete.erase(0, nameToDelete.find_first_not_of(" "));
            nameToDelete.erase(nameToDelete.find_last_not_of(" ") + 1);

            if (nameToDelete.empty()) {
                continue;
            }

            if (!exists(nameToDelete)) {
                cls();
                gotoxy(15, 10);
                std::cout << "Save '" << nameToDelete << "' not found!";
                Sleep(1500);
                continue;
            }

            cls();
            gotoxy(15, 10);
            std::cout << "Are you sure you want to DELETE '" << nameToDelete << "'?";
            gotoxy(15, 11);
            std::cout << "(Y/N): ";
            char ans = _getch();

            if (ans == 'Y' || ans == 'y') {
                if (deleteSave(nameToDelete)) {
                    cls();
                    gotoxy(15, 10);
                    std::cout << "Save '" << nameToDelete << "' deleted!";
                    Sleep(1500);

                    // עדכן את הרשימה
                    saves = listSaves();
                }
            }
            continue;
        }

        // ===== LOAD מצב =====
        StateSnapshot* snap = new StateSnapshot();
        if (!load(*snap, input)) {
            cls();
            gotoxy(15, 10);
            std::cout << "Save '" << input << "' not found!";
            Sleep(1500);
            delete snap;
            continue;
        }

        return snap;  // ✅ מחזיר pointer לנתונים
    }
}