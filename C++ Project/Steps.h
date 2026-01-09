#pragma once

#include <list>
#include <string>
#include <utility>
#include <cstddef>

// מחלקה ששומרת את הקלט של המשתמש לאורך המשחק
// כל צעד = באיזה זמן (tick) נלחץ איזה מקש
class Steps {
private:
    long seed = 0;  // seed לאקראיות (אם יש, אם לא – נשאר 0)
    std::list<std::pair<size_t, char>> steps; // (time, key)

public:
    // seed
    void setSeed(long s);
    long getSeed() const;

    // הוספת קלט (save mode)
    void add(size_t time, char key);

    // load mode – בדיקה ושליפה
    bool hasStepAt(size_t time) const;
    char pop();

    // קבצים
    void save(const std::string& filename) const;
    void load(const std::string& filename);

    bool empty() const;
};
