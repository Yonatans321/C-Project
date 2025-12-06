#pragma once
#include <vector>

class Player;
class Screen;
class Point;
class Direction;

class Obstacle {
public:
    // הפונקציה היחידה שהמשחק קורא מתוך handleTile
    static void handleObstacle(Player& pFront, Player& pBack, Screen& screen);

private:
    // האם השחקן עומד מול מכשול — מאתר את מיקום הכוכבית הראשונה
    static bool isFacingObstacle(Player& p, Screen& screen, Point& obstaclePos);

    // האם שני השחקנים ממוקמים נכון ודוחפים באותו כיוון
    static bool playersAlignedAndPushing(Player& front, Player& back);

    // אסוף רצף של כוכביות לכיוון הדחיפה
    static std::vector<Point> collectChain(Point start, const Direction& dir, Screen& screen);

    // דחיפת השרשרת קדימה
    static void pushChain(const std::vector<Point>& chain, const Direction& dir, Screen& screen);
};
