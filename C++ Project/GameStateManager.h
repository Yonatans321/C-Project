#pragma once
#include <string>
#include <vector>

struct StateSnapshot {
    // Player 1 data
    int p1_x = 0, p1_y = 0, p1_lives = 0, p1_score = 0, p1_item_id = -1;
    char p1_item = 0;

    // Player 2 data
    int p2_x = 0, p2_y = 0, p2_lives = 0, p2_score = 0, p2_item_id = -1;
    char p2_item = 0;

    // Game data
    int level = 0, timer = 0;
    bool timer_active = false, switches_on = false;
    bool door_open[10] = { false };

    // Map data - שמור את מצב החדר כולו
    char mapData[22][80] = {};  // 22 שורות, 80 עמודות

    // Meta
    std::string screens = "";
};

class GameStateManager {
public:
    // ===== פונקציות עיקריות =====
    static StateSnapshot* showLoadMenu();    // ✨ מוביל תפריט ומחזיר snapshot או nullptr
    static bool showSaveMenu(const StateSnapshot& snap);  // ✨ מוביל תפריט שמירה

    // ===== פונקציות עזר =====
    static bool save(const StateSnapshot& state, const std::string& saveName);
    static bool load(StateSnapshot& state, const std::string& saveName);
    static std::vector<std::string> listSaves();
    static bool exists(const std::string& saveName);
    static bool deleteSave(const std::string& saveName);

private:
    static const std::string FOLDER;
    static std::string getMetaFilename(const std::string& saveName);
};