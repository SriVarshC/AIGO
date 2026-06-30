#include "bot-ai/AStarPathfinder.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

namespace aigo {

AStarPathfinder::AStarPathfinder(int w, int h)
    : gridW(w), gridH(h),
      walkable(h, std::vector<bool>(w, true)) {}

int  AStarPathfinder::getWidth()  const { return gridW; }
int  AStarPathfinder::getHeight() const { return gridH; }

bool AStarPathfinder::inBounds(int x, int y) const {
    return x >= 0 && x < gridW && y >= 0 && y < gridH;
}
bool AStarPathfinder::isWalkable(int x, int y) const {
    return inBounds(x, y) && walkable[y][x];
}
void AStarPathfinder::setObstacle(int x, int y) {
    if (inBounds(x, y)) walkable[y][x] = false;
}
void AStarPathfinder::clearObstacle(int x, int y) {
    if (inBounds(x, y)) walkable[y][x] = true;
}
void AStarPathfinder::addObstacleRect(int x, int y, int w, int h) {
    for (int dy = 0; dy < h; ++dy)
        for (int dx = 0; dx < w; ++dx)
            setObstacle(x + dx, y + dy);
}
void AStarPathfinder::clearAll() {
    for (auto& row : walkable)
        std::fill(row.begin(), row.end(), true);
}

float AStarPathfinder::heuristic(const GridPos& a, const GridPos& b) const {
    return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

std::vector<GridPos> AStarPathfinder::getNeighbors(const GridPos& pos) const {
    std::vector<GridPos> nbrs;
    const int dx[] = { 0,  0, -1,  1};
    const int dy[] = {-1,  1,  0,  0};
    for (int i = 0; i < 4; ++i) {
        int nx = pos.x + dx[i], ny = pos.y + dy[i];
        if (isWalkable(nx, ny)) nbrs.emplace_back(nx, ny);
    }
    return nbrs;
}

std::vector<GridPos> AStarPathfinder::findPath(const GridPos& start,
                                                const GridPos& goal) {
    if (!isWalkable(start.x, start.y) || !isWalkable(goal.x, goal.y)) return {};
    if (start == goal) return {start};

    std::vector<std::vector<float>>   g(gridH,
        std::vector<float>(gridW, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<GridPos>> parent(gridH,
        std::vector<GridPos>(gridW, GridPos(-1, -1)));
    std::vector<std::vector<bool>>    closed(gridH,
        std::vector<bool>(gridW, false));

    struct ONode { float f; GridPos pos; };
    std::vector<ONode> open;

    g[start.y][start.x] = 0.0f;
    open.push_back({heuristic(start, goal), start});

    while (!open.empty()) {
        auto minIt = std::min_element(open.begin(), open.end(),
            [](const ONode& a, const ONode& b) { return a.f < b.f; });
        ONode cur = *minIt;
        open.erase(minIt);

        if (closed[cur.pos.y][cur.pos.x]) continue;
        closed[cur.pos.y][cur.pos.x] = true;

        if (cur.pos == goal) {
            std::vector<GridPos> path;
            GridPos p = goal;
            while (p != start) {
                path.push_back(p);
                GridPos par = parent[p.y][p.x];
                if (par.x == -1 && par.y == -1) break;
                p = par;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        float curG = g[cur.pos.y][cur.pos.x];
        for (const GridPos& nb : getNeighbors(cur.pos)) {
            if (closed[nb.y][nb.x]) continue;
            float newG = curG + 1.0f;
            if (newG < g[nb.y][nb.x]) {
                g[nb.y][nb.x]      = newG;
                parent[nb.y][nb.x] = cur.pos;
                open.push_back({newG + heuristic(nb, goal), nb});
            }
        }
    }
    return {};
}

void AStarPathfinder::printGrid(const std::vector<GridPos>& path,
                                 const GridPos& start,
                                 const GridPos& goal) const {
    std::vector<std::vector<char>> grid(gridH, std::vector<char>(gridW, '.'));
    for (int y = 0; y < gridH; ++y)
        for (int x = 0; x < gridW; ++x)
            if (!walkable[y][x]) grid[y][x] = '#';
    for (const auto& p : path)
        if (grid[p.y][p.x] == '.') grid[p.y][p.x] = '*';
    if (inBounds(start.x, start.y)) grid[start.y][start.x] = 'S';
    if (inBounds(goal.x,  goal.y))  grid[goal.y][goal.x]   = 'G';

    std::cout << "    ";
    for (int x = 0; x < gridW; ++x)
        std::cout << std::setw(2) << x;
    std::cout << "\n";
    for (int y = 0; y < gridH; ++y) {
        std::cout << std::setw(3) << y << " ";
        for (int x = 0; x < gridW; ++x)
            std::cout << " " << grid[y][x];
        std::cout << "\n";
    }
}

void AStarPathfinder::printPathInfo(const std::vector<GridPos>& path,
                                     const GridPos& start,
                                     const GridPos& goal) const {
    if (path.empty()) {
        std::cout << "  >> NO PATH FOUND from ("
                  << start.x << "," << start.y << ") to ("
                  << goal.x  << "," << goal.y  << ")\n";
        return;
    }
    int steps = static_cast<int>(path.size()) - 1;
    std::cout << "  >> Path found: " << steps << " steps | "
              << path.size() << " nodes\n  Route: ";
    if (static_cast<int>(path.size()) <= 9) {
        for (int i = 0; i < static_cast<int>(path.size()); ++i) {
            if (i > 0) std::cout << " -> ";
            std::cout << "(" << path[i].x << "," << path[i].y << ")";
        }
    } else {
        for (int i = 0; i < 4; ++i)
            std::cout << "(" << path[i].x << "," << path[i].y << ") -> ";
        std::cout << "... -> (" << path.back().x << "," << path.back().y << ")";
    }
    std::cout << "\n";
}

} // namespace aigo