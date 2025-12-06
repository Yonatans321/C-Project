//#pragma once
//#include <vector>
//class Player;
//class Screen;
//class Point;
//
//class Obstacle {
//public:
//
//    // בודק ומבצע דחיפה — הפונקציה היחידה שהמשחק קורא
//    static void tryPushObstacles(Player& p1, Player& p2, Screen& screen);
//
//private:
//
//    // האם השחקן עומד מול מכשול ויכול לתחיל דחיפה
//    static bool playerIsAgainstObstacle(Player& p, Screen& screen, Point& obstaclePos);
//
//    // מחזיר true אם השחקן השני נמצא מאחור (בקו הדחיפה)
//    static bool secondPlayerSupportsPush(Player& front, Player& back);
//
//    // אוסף את כל רצף ה־* קדימה לכיוון הדחיפה
//    static std::vector<Point> collectChain(Point start, Player& p, Screen& screen);
//
//    // מזיז את השרשרת קדימה ברגע שיש מקום פנוי
//    static void pushChainForward(const std::vector<Point>& chain, Player& p, Screen& screen);
//};