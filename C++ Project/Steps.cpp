#include "Steps.h"
#include <fstream>

// ---------- seed ----------

void Steps::setSeed(long s)
{
    seed = s;
}

long Steps::getSeed() const
{
    return seed;
}

// ---------- רישום צעדים ----------

void Steps::add(size_t time, char key)
{
    steps.push_back({ time, key });
}

// ---------- load mode ----------

bool Steps::hasStepAt(size_t time) const
{
    return !steps.empty() && steps.front().first == time;
}

char Steps::pop()
{
    char key = steps.front().second;
    steps.pop_front();
    return key;
}

bool Steps::empty() const
{
    return steps.empty();
}

// ---------- קבצים ----------

void Steps::save(const std::string& filename) const
{
    std::ofstream out(filename);

    // seed
    out << seed << '\n';

    // מספר צעדים
    out << steps.size();

    // צעדים
    for (const auto& s : steps)
    {
        out << '\n' << s.first << ' ' << s.second;
    }
}

void Steps::load(const std::string& filename)
{
    steps.clear();

    std::ifstream in(filename);
    size_t count;

    // seed
    in >> seed;

    // מספר צעדים
    in >> count;

    for (size_t i = 0; i < count; ++i)
    {
        size_t time;
        char key;
        in >> time >> key;
        steps.push_back({ time, key });
    }
}
