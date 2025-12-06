//#include "Obstacle.h"
//#include "Player.h"
//#include "Screen.h"
//#include "Point.h"
//#include <vector>
//
//void Obstacle::tryPushObstacles(Player& p1, Player& p2, Screen& screen)
//{
//    // בדוק קודם האם p1 הוא זה שדוחף
//    Point firstObstacle(0, 0);
//    if (playerIsAgainstObstacle(p1, screen, firstObstacle) &&
//        secondPlayerSupportsPush(p1, p2))
//    {
//        auto chain = collectChain(firstObstacle, p1, screen);
//        pushChainForward(chain, p1, screen);
//        return;
//    }
//
//    // בדוק האם p2 הוא זה שדוחף
//    if (playerIsAgainstObstacle(p2, screen, firstObstacle) &&
//        secondPlayerSupportsPush(p2, p1))
//    {
//        auto chain = collectChain(firstObstacle, p2, screen);
//        pushChainForward(chain, p2, screen);
//    }
//}
//
//bool Obstacle::playerIsAgainstObstacle(Player& p, Screen& screen, Point& obstaclePos)
//{
//    Point pos = p.getPosition();
//    Point forward = pos;
//    forward.move(p.getDirection());
//
//    if (screen.getCharAt(forward.getX(), forward.getY()) == '*')
//    {
//        obstaclePos = forward;
//        return true;
//    }
//
//    return false;
//}
//
//bool Obstacle::secondPlayerSupportsPush(Player& front, Player& back)
//{
//    Point backPos = front.getPosition();
//    backPos.move(front.getOppositeDirection());
//
//    return backPos == back.getPosition();
//}
//
//std::vector<Point> Obstacle::collectChain(Point start, Player& p, Screen& screen)
//{
//    std::vector<Point> chain;
//    Point cur = start;
//
//    while (screen.getCharAt(cur.getX(), cur.getY()) == '*')
//    {
//        chain.push_back(cur);
//        cur.move(p.getDirection());
//    }
//
//    // אם התא אחרי השרשרת לא ריק — אי אפשר לדחוף
//    if (screen.getCharAt(cur.getX(), cur.getY()) != ' ')
//        chain.clear(); // מחזיר ריק = סימן לכישלון
//
//    return chain;
//}
//
//void Obstacle::pushChainForward(const std::vector<Point>& chain, Player& p, Screen& screen)
//{
//    if (chain.empty()) return;
//
//    // הנקודה החדשה למכשול האחרון
//    Point next = chain.back();
//    next.move(p.getDirection());
//
//    // מזיזים מהסוף להתחלה (לא מוחקים מכשולים בדרך)
//    for (int i = chain.size() - 1; i >= 0; i--)
//    {
//        Point from = chain[i];
//        Point to = (i == chain.size() - 1) ? next : chain[i + 1];
//
//        screen.setCharAt(to.getX(), to.getY(), '*');
//        screen.setCharAt(from.getX(), from.getY(), ' ');
//    }
//}