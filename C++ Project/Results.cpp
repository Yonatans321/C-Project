#include "Results.h"
#include <fstream>
#include <sstream>

void Results::setScreenFiles(const std::string& files)
{
    screenFiles = files;
}

void Results::addLifeLost(size_t time, PlayerType player)
{
    Event event;
    event.time = time;
    event.type = EventType::LifeLost;
    event.player = player;
    events.push_back(event);
}

void Results::addScreenChange(size_t time, const std::string& screenName)
{
    Event event;
    event.time = time;
    event.type = EventType::ScreenChange;
    event.info = screenName;
    events.push_back(event);
}

void Results::addRiddle(size_t time, int riddleId, const std::string& riddleText,
    const std::string& answer, bool solved)
{
    Event event;
    event.time = time;
    event.type = EventType::Riddle;
    event.riddle = solved ? RiddleResult::Solved : RiddleResult::Failed;
    event.info = std::to_string(riddleId) + "|" + riddleText + "|" + answer;
    events.push_back(event);
}

void Results::addGameOver(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameOver;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}

void Results::addGameFinished(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameFinished;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}

void Results::addGameExit(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameExit;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}

void Results::save(const std::string& filename) const
{
    std::ofstream file(filename);

    file << "screens= " << screenFiles << "\n";

    for (const auto& event : events)
    {
        file << "time= " << event.time << " event= ";

        if (event.type == EventType::ScreenChange)
        {
            file << "ScreenChange screenName= " << event.info;
        }
        else if (event.type == EventType::LifeLost)
        {
            std::string playerName = (event.player == PlayerType::Player1) ? "Player1" : "Player2";
            file << "LifeLost player= " << playerName;
        }
        else if (event.type == EventType::Riddle)
        {
            std::istringstream iss(event.info);
            std::string riddleId, riddleText, answer;

            std::getline(iss, riddleId, '|');
            std::getline(iss, riddleText, '|');
            std::getline(iss, answer, '|');

            std::string result = (event.riddle == RiddleResult::Solved) ? "Solved" : "Failed";

            file << "Riddle riddleId= " << riddleId
                << " riddleText= " << riddleText
                << " answer= " << answer
                << " result= " << result;
        }
        else if (event.type == EventType::GameOver)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameOver score1= " << score1 << " score2= " << score2;
        }
        else if (event.type == EventType::GameFinished)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameFinished score1= " << score1 << " score2= " << score2;
        }
        else if (event.type == EventType::GameExit)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameExit score1= " << score1 << " score2= " << score2;
        }

        file << "\n";
    }
}

void Results::load(const std::string& filename)
{
    events.clear();
    std::ifstream file(filename);
    std::string line;

    if (std::getline(file, line))
    {
        if (line.find("screens= ") == 0)
        {
            screenFiles = line.substr(9);
        }
    }

    while (std::getline(file, line))
    {
        Event event;
        parseEventLine(line, event);
        events.push_back(event);
    }
}

void Results::parseEventLine(const std::string& line, Event& event)
{
    size_t pos = line.find("time= ");
    if (pos != std::string::npos)
    {
        event.time = std::stoll(line.substr(pos + 6));
    }

    pos = line.find("event= ");
    if (pos == std::string::npos) return;

    std::string eventPart = line.substr(pos + 7);

    if (eventPart.find("ScreenChange") == 0)
    {
        event.type = EventType::ScreenChange;
        pos = line.find("screenName= ");
        if (pos != std::string::npos)
        {
            event.info = line.substr(pos + 12);
        }
    }
    else if (eventPart.find("LifeLost") == 0)
    {
        event.type = EventType::LifeLost;
        pos = line.find("player= ");
        if (pos != std::string::npos)
        {
            std::string playerName = line.substr(pos + 8);
            event.player = (playerName == "Player1") ? PlayerType::Player1 : PlayerType::Player2;
        }
    }
    else if (eventPart.find("Riddle") == 0)
    {
        event.type = EventType::Riddle;

        pos = line.find("riddleId= ");
        size_t endPos = line.find(" ", pos + 10);
        std::string riddleId = line.substr(pos + 10, endPos - pos - 10);

        pos = line.find("riddleText= ");
        endPos = line.find(" answer= ", pos);
        std::string riddleText = line.substr(pos + 12, endPos - pos - 12);

        pos = line.find("answer= ");
        endPos = line.find(" result= ", pos);
        std::string answer = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("result= ");
        std::string result = line.substr(pos + 8);
        event.riddle = (result == "Solved") ? RiddleResult::Solved : RiddleResult::Failed;

        event.info = riddleId + "|" + riddleText + "|" + answer;
    }
    else if (eventPart.find("GameOver") == 0)
    {
        event.type = EventType::GameOver;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
    else if (eventPart.find("GameFinished") == 0)
    {
        event.type = EventType::GameFinished;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
    else if (eventPart.find("GameExit") == 0)
    {
        event.type = EventType::GameExit;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
}

bool Results::empty() const
{
    return events.empty();
}

Event Results::pop()
{
    Event event = events.front();
    events.pop_front();
    return event;
}

bool Results::compareWith(const Results& other, std::string& failureReason) const
{
    // Check 1: Screen files must match
    if (screenFiles != other.screenFiles)
    {
        failureReason = "Screen files don't match!\n";
        failureReason += "Expected: " + other.screenFiles + "\n";
        failureReason += "Got: " + screenFiles;
        return false;
    }

    // Check 2: Event count must match
    if (events.size() != other.events.size())
    {
        failureReason = "Event count mismatch!\n";
        failureReason += "Expected: " + std::to_string(other.events.size()) + " events\n";
        failureReason += "Got: " + std::to_string(events.size()) + " events";
        return false;
    }

    // Check 3: Compare each event
    auto it1 = events.begin();
    auto it2 = other.events.begin();
    int eventIndex = 0;

    while (it1 != events.end() && it2 != other.events.end())
    {
        const Event& actualEvent = *it1;
        const Event& expectedEvent = *it2;

        // Check event time
        if (actualEvent.time != expectedEvent.time)
        {
            failureReason = "Event #" + std::to_string(eventIndex) + ": Time mismatch!\n";
            failureReason += "Expected time: " + std::to_string(expectedEvent.time) + "\n";
            failureReason += "Got time: " + std::to_string(actualEvent.time);
            return false;
        }

        // Check event type
        if (actualEvent.type != expectedEvent.type)
        {
            failureReason = "Event #" + std::to_string(eventIndex) + ": Event type mismatch!\n";
            failureReason += "Expected: " + eventTypeToString(expectedEvent.type) + "\n";
            failureReason += "Got: " + eventTypeToString(actualEvent.type);
            return false;
        }

        // Check event data (for relevant events)
        if (actualEvent.info != expectedEvent.info)
        {
            failureReason = "Event #" + std::to_string(eventIndex) + ": Event data mismatch!\n";
            failureReason += "Expected: " + expectedEvent.info + "\n";
            failureReason += "Got: " + actualEvent.info;
            return false;
        }

        // For LifeLost and Riddle events, check player/result
        if (actualEvent.type == EventType::LifeLost)
        {
            if (actualEvent.player != expectedEvent.player)
            {
                failureReason = "Event #" + std::to_string(eventIndex) + ": Player mismatch!\n";
                std::string expectedPlayer = (expectedEvent.player == PlayerType::Player1) ? "Player1" : "Player2";
                std::string gotPlayer = (actualEvent.player == PlayerType::Player1) ? "Player1" : "Player2";
                failureReason += "Expected: " + expectedPlayer + "\n";
                failureReason += "Got: " + gotPlayer;
                return false;
            }
        }

        if (actualEvent.type == EventType::Riddle)
        {
            if (actualEvent.riddle != expectedEvent.riddle)
            {
                failureReason = "Event #" + std::to_string(eventIndex) + ": Riddle result mismatch!\n";
                std::string expectedResult = (expectedEvent.riddle == RiddleResult::Solved) ? "Solved" : "Failed";
                std::string gotResult = (actualEvent.riddle == RiddleResult::Solved) ? "Solved" : "Failed";
                failureReason += "Expected: " + expectedResult + "\n";
                failureReason += "Got: " + gotResult;
                return false;
            }
        }

        ++it1;
        ++it2;
        ++eventIndex;
    }

    // If we got here, everything matches!
    return true;
}

// Helper function to convert EventType to string
std::string Results::eventTypeToString(EventType type) const
{
    switch (type)
    {
    case EventType::ScreenChange:   return "ScreenChange";
    case EventType::LifeLost:       return "LifeLost";
    case EventType::Riddle:         return "Riddle";
    case EventType::GameOver:       return "GameOver";
    case EventType::GameFinished:   return "GameFinished";
    case EventType::GameExit:       return "GameExit";
    default:                        return "Unknown";
    }
}