#include "Obstacle.h"
#include "Player.h"
#include "Screen.h"
#include "Point.h"
#include "Direction.h"
#include <vector>

// ======================================================
//            פונקציה ראשית שהמשחק קורא
// ======================================================
void Obstacle::handleObstacle(Player& p1, Player& p2, Screen& screen)
{
    Point obs(0, 0);

    // בדיקה אם p1 הוא הדוחף המוביל
    if (isFacingObstacle(p1, screen, obs) && playersAlignedAndPushing(p1, p2))
    {
        auto chain = collectChain(obs, p1.getDirection(), screen);
        if (!chain.empty())
            pushChain(chain, p1.getDirection(), screen);
        return;
    }

    // בדיקה אם p2 הוא הדוחף המוביל
    if (isFacingObstacle(p2, screen, obs) && playersAlignedAndPushing(p2, p1))
    {
        auto chain = collectChain(obs, p2.getDirection(), screen);
        if (!chain.empty())
            pushChain(chain, p2.getDirection(), screen);
    }
}

// ======================================================
//        בדיקה אם שחקן עומד מול כוכבית
// ======================================================
bool Obstacle::isFacingObstacle(Player& p, Screen& screen, Point& obstaclePos)
{
    Point pos = p.getPosition();
    Direction dir = p.getDirection();

    Point forward(pos.getX() + dir.getX(), pos.getY() + dir.getY());

    if (screen.getCharAt(forward.getX(), forward.getY()) == '*')
    {
        obstaclePos = forward;
        return true;
    }
    return false;
}

// ======================================================
//     בדיקה אם שני השחקנים מיושרים ויכולים לדחוף יחד
// ======================================================
bool Obstacle::playersAlignedAndPushing(Player& front, Player& back)
{
    Direction df = front.getDirection();
    Direction db = back.getDirection();

    // חייבים לדחוף באותו כיוון
    if (df.getX() != db.getX() || df.getY() != db.getY())
        return false;

    // השחקן האחורי חייב לזוז (לא STAY)
    if (db.getX() == 0 && db.getY() == 0)
        return false;

    // בדיקה שהשחקן האחורי צמוד ממש מאחור
    Point fp = front.getPosition();
    Direction opp = front.getOppositeDirection();

    Point expectedBack(fp.getX() + opp.getX(),
        fp.getY() + opp.getY());

    return (expectedBack.getX() == back.getX() &&
        expectedBack.getY() == back.getY());
}

// ======================================================
//        איסוף השרשרת של המכשולים (***)
// ======================================================
std::vector<Point> Obstacle::collectChain(Point start, const Direction& dir, Screen& screen)
{
    std::vector<Point> chain;
    Point cur = start;

    while (screen.getCharAt(cur.getX(), cur.getY()) == '*')
    {
        chain.push_back(cur);
        cur = Point(cur.getX() + dir.getX(), cur.getY() + dir.getY());
    }

    // אם התא שאחרי לא ריק — אי אפשר לזוז
    if (screen.getCharAt(cur.getX(), cur.getY()) != ' ')
        return {};

    return chain;
}

// ======================================================
//                 דחיפת כל השרשרת קדימה
// ======================================================
void Obstacle::pushChain(const std::vector<Point>& chain, const Direction& dir, Screen& screen)
{
    if (chain.empty()) return;

    Point newSpot(chain.back().getX() + dir.getX(),
        chain.back().getY() + dir.getY());

    // דוחפים מהסוף להתחלה
    for (int i = chain.size() - 1; i >= 0; i--)
    {
        Point from = chain[i];
        Point to = (i == chain.size() - 1) ? newSpot : chain[i + 1];

        screen.setCharAt(to.getX(), to.getY(), '*');
        screen.setCharAt(from.getX(), from.getY(), ' ');
    }
}
