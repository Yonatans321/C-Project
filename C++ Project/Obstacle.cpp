#include "Obstacle.h"
#include "Player.h"
#include "Screen.h"
#include "Point.h"
#include "Direction.h"


// Handle obstacle interaction for two players (helped by AI)
// pushing rules:
// 1 obstacle  -> 1 player
// 2 obstacles -> 2 aligned players

void Obstacle::handleObstacle(Player& p1, Player& p2, Screen& screen)
{
    Point obs;
    bool pushed = false;

    if (isFacingObstacle(p1, screen, obs))
    {
        auto chain = collectChain(obs, p1.getDirection(), screen);
        size_t count = chain.size();

        if (count == 1)
        {
            pushChain(chain, p1.getDirection(), screen);
            pushed = true;
        }
        else if (count == 2 && playersAlignedAndPushing(p1, p2))
        {
            pushChain(chain, p1.getDirection(), screen);
            pushed = true;
        }
    }

    if (!pushed && isFacingObstacle(p2, screen, obs))
    {
        auto chain = collectChain(obs, p2.getDirection(), screen);
        size_t count = chain.size();

        if (count == 1)
        {
            pushChain(chain, p2.getDirection(), screen);
        }
        else if (count == 2 && playersAlignedAndPushing(p2, p1))
        {
            pushChain(chain, p2.getDirection(), screen);
        }
    }
}

// check if the player is facing an obstacle (helped by AI)
bool Obstacle::isFacingObstacle(const Player& p, Screen& screen, Point& obstaclePos)
{
	// Calculate the position in front of the player    
    Point pos = p.getPosition();
    Direction dir = p.getDirection();

    Point forward(pos.getX() + dir.getX(), pos.getY() + dir.getY()); //pos in front of player

    if (screen.getCharAt(forward.getX(), forward.getY()) == '*') // check if obstacle
    {
        obstaclePos = forward; // store obstacle position
        return true;
    }
    return false;
}
// check if both player are aligned and pushing (helped by AI)
bool Obstacle::playersAlignedAndPushing(const Player& front,const Player& back)
{
    // get ditrection of both players
    Direction df = front.getDirection();
    Direction db = back.getDirection();

    // not pushing in same direction
	if (df.getX() != db.getX() || df.getY() != db.getY()) 
        return false;
    if (db.getX() == 0 && db.getY() == 0)
        return false;

    // check direction from back to front 
    Point fp = front.getPosition();
    Direction opp = front.getOppositeDirection();

    // expected pos of back player
    Point expectedBack(fp.getX() + opp.getX(),
        fp.getY() + opp.getY());

    return (expectedBack.getX() == back.getX() &&
        expectedBack.getY() == back.getY());
}
// collect the chain of obstacles (helped by AI)
std::vector<Point> Obstacle::collectChain( Point start, const Direction& dir,const Screen& screen)
{
    std::vector<Point> chain; // store obstacle chain
    Point cur = start;
	// collect obstacles in the direction
    while (screen.getCharAt(cur.getX(), cur.getY()) == '*')
    {
        chain.push_back(cur);
        cur = Point(cur.getX() + dir.getX(), cur.getY() + dir.getY());
    }
	// check if next position is empty
    if (screen.getCharAt(cur.getX(), cur.getY()) != ' ')
        return {};

    return chain;
}

// push the chain of obstacles
void Obstacle::pushChain(const std::vector<Point>& chain, const Direction& dir, Screen& screen)
{
    if (chain.empty()) return;

    Point newSpot(chain.back().getX() + dir.getX(),
        chain.back().getY() + dir.getY());

    // move obstacle from end to start
    for (int i =(int)chain.size() - 1; i >= 0; i--)
    {
        Point from = chain[i];
        Point to = (i == chain.size() - 1) ? newSpot : chain[i + 1];

        screen.setCharAt(to.getX(), to.getY(), '*');
        screen.setCharAt(from.getX(), from.getY(), ' ');
    }
}
