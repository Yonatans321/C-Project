#pragma once
#include <list>
#include <string>
#include <cstddef>

enum class EventType {
    ScreenChange,   // Changed Screen
    LifeLost,       // Player lost a life
    Riddle,         // Riddle attempted
    GameOver,       // Game ended - player died
    GameFinished    // Game ended - player won
};

enum class RiddleResult {
    Solved,
    Failed
};

enum class PlayerType {
    Player1,  // Player & 
    Player2   // Player $
};

struct Event {
    size_t time = 0;
    EventType type = EventType::LifeLost;
    RiddleResult riddle = RiddleResult::Failed;
    PlayerType player = PlayerType::Player1;
    std::string info;
};

class Results {
private:
    std::list<Event> events;
    std::string screenFiles;
    void parseEventLine(const std::string& line, Event& event);

public:
    void setScreenFiles(const std::string& files);

    void addLifeLost(size_t time, PlayerType player);
    void addScreenChange(size_t time, const std::string& screenName);
    void addRiddle(size_t time, int riddleId, const std::string& riddleText,
        const std::string& answer, bool solved);
    void addGameOver(size_t time, int score1, int score2);
    void addGameFinished(size_t time, int score1, int score2);

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool empty() const;
    Event pop();
};