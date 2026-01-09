#include "Results.h"
#include <fstream>
#include <sstream>

void Results::setScreenFiles(const std::string& files)
{
    screenFiles = files;
}

void Results::addLifeLost(size_t time)
{
    Event event;
    event.time = time;
    event.type = EventType::LifeLost;
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

void Results::addGameEnd(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameEnd;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}

// ---------- שמירה לקובץ ----------
void Results::save(const std::string& filename) const
{
    std::ofstream file(filename);

    // שמור שמות קבצי ה-screens
    file << "screens=" << screenFiles << "\n";

    // שמור כל אירוע
    for (const auto& event : events)
    {
        file << "time=" << event.time << " event=";

        if (event.type == EventType::ScreenChange)
        {
            file << "ScreenChange screenName=" << event.info;
        }
        else if (event.type == EventType::LifeLost)
        {
            file << "LifeLost";
        }
        else if (event.type == EventType::Riddle)
        {
            // parse: riddleId|riddleText|answer
            std::istringstream iss(event.info);
            std::string riddleId, riddleText, answer;

            std::getline(iss, riddleId, '|');
            std::getline(iss, riddleText, '|');
            std::getline(iss, answer, '|');

            std::string result = (event.riddle == RiddleResult::Solved) ? "Solved" : "Failed";

            file << "Riddle riddleId=" << riddleId
                << " riddleText=" << riddleText
                << " answer=" << answer
                << " result=" << result;
        }
        else if (event.type == EventType::GameEnd)
        {
            // parse: score1|score2
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameEnd score1=" << score1 << " score2=" << score2;
        }

        file << "\n";
    }
}

// ---------- טעינה מקובץ ----------
void Results::load(const std::string& filename)
{
    events.clear();
    std::ifstream file(filename);
    std::string line;

    // קרא שורה ראשונה - שמות ה-screens
    if (std::getline(file, line))
    {
        if (line.find("screens=") == 0)
        {
            screenFiles = line.substr(8);
        }
    }

    // קרא כל אירוע
    while (std::getline(file, line))
    {
        Event event;
        parseEventLine(line, event);
        events.push_back(event);
    }
}

void Results::parseEventLine(const std::string& line, Event& event)
{
    // parse: time=X event=Y ...

    // חלץ זמן
    size_t pos = line.find("time=");
    if (pos != std::string::npos)
    {
        event.time = std::stoll(line.substr(pos + 5));
    }

    // חלץ סוג אירוע
    pos = line.find("event=");
    if (pos == std::string::npos) return;

    std::string eventPart = line.substr(pos + 6);

    if (eventPart.find("ScreenChange") == 0)
    {
        event.type = EventType::ScreenChange;
        pos = line.find("screenName=");
        if (pos != std::string::npos)
        {
            event.info = line.substr(pos + 11);
        }
    }
    else if (eventPart.find("LifeLost") == 0)
    {
        event.type = EventType::LifeLost;
    }
    else if (eventPart.find("Riddle") == 0)
    {
        event.type = EventType::Riddle;

        // חלץ riddleId
        pos = line.find("riddleId=");
        size_t endPos = line.find(" ", pos + 9);
        std::string riddleId = line.substr(pos + 9, endPos - pos - 9);

        // חלץ riddleText
        pos = line.find("riddleText=");
        endPos = line.find(" answer=", pos);
        std::string riddleText = line.substr(pos + 11, endPos - pos - 11);

        // חלץ answer
        pos = line.find("answer=");
        endPos = line.find(" result=", pos);
        std::string answer = line.substr(pos + 7, endPos - pos - 7);

        // חלץ result
        pos = line.find("result=");
        std::string result = line.substr(pos + 7);
        event.riddle = (result == "Solved") ? RiddleResult::Solved : RiddleResult::Failed;

        event.info = riddleId + "|" + riddleText + "|" + answer;
    }
    else if (eventPart.find("GameEnd") == 0)
    {
        event.type = EventType::GameEnd;

        // חלץ score1 ו-score2
        pos = line.find("score1=");
        size_t endPos = line.find(" score2=", pos);
        std::string score1 = line.substr(pos + 7, endPos - pos - 7);

        pos = line.find("score2=");
        std::string score2 = line.substr(pos + 7);

        event.info = score1 + "|" + score2;
    }
}

// ---------- load / silent ----------
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