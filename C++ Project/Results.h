#pragma once

#include <list>
#include <string>
#include <cstddef>

// איזה דברים חשובים קרו במשחק
enum class EventType {
    ScreenChange,
    LifeLost,
    Riddle,
    GameEnd
};

// תוצאה של חידה
enum class RiddleResult {
    Solved,
    Failed
};

// אירוע אחד בזמן מסוים
struct GameEvent {
    size_t time;                 // מונה צעדים של המשחק
    EventType type;              // סוג האירוע
    RiddleResult riddleResult;   // רלוונטי רק ל־RIDDLE
    std::string info;            // שם חדר / ניקוד וכו'
};

class Results {
private:
    std::list<GameEvent> events;

public:
    // --- ADDON בלבד: רישום אירועים ---
    void lifeLost(size_t time);
    void screenChanged(size_t time, const std::string& screenName);
    void riddleAnswered(size_t time, bool solved);
    void gameEnded(size_t time, int score);

    // --- קבצים ---
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // --- שימוש ב-load / silent ---
    bool empty() const;
    GameEvent popNext();
};
