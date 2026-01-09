#pragma once
#include <list>
#include <string>
#include <cstddef>

enum class EventType {
	ScreenChange,  // Changed Screen
	LifeLost, // Player lost a life
	Riddle, // Riddle attempted
	GameEnd// Game Over->player lost all lives
};

enum class RiddleResult { // Result of a riddle attempt
    Solved,
    Failed
};

struct Event { // Represents a game event
    size_t time = 0;
    EventType type = EventType::LifeLost;
    RiddleResult riddle = RiddleResult::Failed;
    std::string info;
};

class Results { // Manages game results and events
private:
    std::list<Event> events;
    std::string screenFiles;

    void parseEventLine(const std::string& line, Event& event);

public:
    void setScreenFiles(const std::string& files);

    void addLifeLost(size_t time);
    void addScreenChange(size_t time, const std::string& screenName);
    void addRiddle(size_t time, int riddleId, const std::string& riddleText,
        const std::string& answer, bool solved);
    void addGameEnd(size_t time, int score1, int score2);

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool empty() const;
    Event pop();
};