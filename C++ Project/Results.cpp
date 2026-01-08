#include "Results.h"
#include <fstream>

// ---------- רישום אירועים (ADDON בלבד) ----------

void Results::lifeLost(size_t time)
{
    events.push_back({ time, EventType::LifeLost, RiddleResult::Failed, "" });
}

void Results::screenChanged(size_t time, const std::string& screenName)
{
    events.push_back({ time, EventType::ScreenChange, RiddleResult::Failed, screenName });
}

void Results::riddleAnswered(size_t time, bool solved)
{
    events.push_back({ time,
                       EventType::Riddle,
                       solved ? RiddleResult::Solved : RiddleResult::Failed,
                       "" });
}

void Results::gameEnded(size_t time, int score)
{
    events.push_back({ time,
                       EventType::GameEnd,
                       RiddleResult::Failed,
                       std::to_string(score) });
}

// ---------- שמירה לקובץ ----------

void Results::saveToFile(const std::string& filename) const
{
    std::ofstream out(filename);
    out << events.size();

    for (const auto& ev : events)
    {
        out << '\n'
            << ev.time << ' '
            << static_cast<int>(ev.type) << ' '
            << static_cast<int>(ev.riddleResult) << ' '
            << ev.info;
    }
}

// ---------- טעינה מקובץ ----------

void Results::loadFromFile(const std::string& filename)
{
    events.clear();

    std::ifstream in(filename);
    size_t count;
    in >> count;

    for (size_t i = 0; i < count; ++i)
    {
        GameEvent ev;
        int typeInt, riddleInt;

        in >> ev.time >> typeInt >> riddleInt;
        ev.type = static_cast<EventType>(typeInt);
        ev.riddleResult = static_cast<RiddleResult>(riddleInt);

        std::getline(in, ev.info);
        if (!ev.info.empty() && ev.info[0] == ' ')
            ev.info.erase(0, 1);

        events.push_back(ev);
    }
}

// ---------- load / silent ----------

bool Results::empty() const
{
    return events.empty();
}

GameEvent Results::popNext()
{
    GameEvent event = events.front();
    events.pop_front();
    return event;
}
