#pragma once
#include "entities/Player.h"
#include <string>
#include <vector>

namespace aigo {

struct SpawnZone {
    float x, y, radius;
    SpawnZone(float x, float y, float radius = 6.0f)
        : x(x), y(y), radius(radius) {}
};

class Map {
private:
    std::string            mapName;
    float                  width;
    float                  height;
    std::vector<SpawnZone> spawnZonesT1;
    std::vector<SpawnZone> spawnZonesT2;

public:
    Map(const std::string& name, float width, float height);

    std::string getMapName() const;
    float       getWidth()   const;
    float       getHeight()  const;

    void     addSpawnZone(int teamIndex, const SpawnZone& zone);
    Position getRandomSpawnPoint(int teamIndex) const;
    bool     isValidPosition(float x, float y)  const;

    void printInfo() const;
};

} // namespace aigo