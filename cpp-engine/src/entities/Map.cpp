#include "entities/Map.h"
#include <iostream>
#include <cstdlib>

namespace aigo {

Map::Map(const std::string& name, float width, float height)
    : mapName(name), width(width), height(height) {
    // Default spawn zones — team 1 on left, team 2 on right
    spawnZonesT1.emplace_back(10.0f,  50.0f, 8.0f);
    spawnZonesT1.emplace_back(15.0f,  25.0f, 6.0f);
    spawnZonesT2.emplace_back(90.0f,  50.0f, 8.0f);
    spawnZonesT2.emplace_back(85.0f,  75.0f, 6.0f);
}

std::string Map::getMapName() const { return mapName; }
float       Map::getWidth()   const { return width; }
float       Map::getHeight()  const { return height; }

void Map::addSpawnZone(int teamIndex, const SpawnZone& zone) {
    if (teamIndex == 0) spawnZonesT1.push_back(zone);
    else                spawnZonesT2.push_back(zone);
}

Position Map::getRandomSpawnPoint(int teamIndex) const {
    const auto& zones = (teamIndex == 0) ? spawnZonesT1 : spawnZonesT2;
    if (zones.empty()) return Position(50.0f, 50.0f);

    const SpawnZone& z = zones[std::rand() % zones.size()];
    float ox = (static_cast<float>(std::rand() % 200) / 100.0f - 1.0f) * z.radius;
    float oy = (static_cast<float>(std::rand() % 200) / 100.0f - 1.0f) * z.radius;
    return Position(z.x + ox, z.y + oy);
}

bool Map::isValidPosition(float x, float y) const {
    return x >= 0.0f && x <= width && y >= 0.0f && y <= height;
}

void Map::printInfo() const {
    std::cout << "  Map: " << mapName
              << "  |  Size: " << width << "x" << height
              << "  |  Spawn Zones: T1=" << spawnZonesT1.size()
              << " T2=" << spawnZonesT2.size() << "\n";
}

} // namespace aigo