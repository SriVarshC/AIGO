#pragma once
#include <vector>
#include <string>
#include <limits>

namespace aigo {

struct GridPos {
    int x, y;
    GridPos(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const GridPos& o) const { return x == o.x && y == o.y; }
    bool operator!=(const GridPos& o) const { return !(*this == o); }
};

class AStarPathfinder {
private:
    int                            gridW;
    int                            gridH;
    std::vector<std::vector<bool>> walkable;   // walkable[y][x]

    float                heuristic(const GridPos& a, const GridPos& b) const;
    bool                 inBounds(int x, int y)                         const;
    std::vector<GridPos> getNeighbors(const GridPos& pos)               const;

public:
    AStarPathfinder(int width, int height);

    void setObstacle(int x, int y);
    void clearObstacle(int x, int y);
    void addObstacleRect(int x, int y, int w, int h);
    void clearAll();
    bool isWalkable(int x, int y) const;
    int  getWidth()                const;
    int  getHeight()               const;

    std::vector<GridPos> findPath(const GridPos& start, const GridPos& goal);

    void printGrid(const std::vector<GridPos>& path,
                   const GridPos& start, const GridPos& goal) const;
    void printPathInfo(const std::vector<GridPos>& path,
                       const GridPos& start, const GridPos& goal) const;
};

} // namespace aigo