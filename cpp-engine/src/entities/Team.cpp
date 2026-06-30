#include "entities/Team.h"
#include <iostream>
#include <string>

namespace aigo {

Team::Team(const std::string& id, const std::string& name)
    : teamId(id), teamName(name), teamScore(0) {}

std::string Team::getTeamId()    const { return teamId; }
std::string Team::getTeamName()  const { return teamName; }
int         Team::getTeamScore() const { return teamScore; }
int         Team::getPlayerCount() const { return static_cast<int>(players.size()); }

int Team::getAliveCount() const {
    int count = 0;
    for (const auto& p : players)
        if (p->isAlive()) ++count;
    return count;
}

std::vector<std::shared_ptr<Player>>& Team::getPlayers()             { return players; }
const std::vector<std::shared_ptr<Player>>& Team::getPlayers() const { return players; }

void Team::addPlayer(std::shared_ptr<Player> player) { players.push_back(player); }
void Team::addScore(int points)                       { teamScore += points; }

bool Team::isEliminated() const { return getAliveCount() == 0; }

int Team::getTotalKills() const {
    int total = 0;
    for (const auto& p : players) total += p->getKills();
    return total;
}

int Team::getTotalDeaths() const {
    int total = 0;
    for (const auto& p : players) total += p->getDeaths();
    return total;
}

void Team::printTeamStats() const {
    std::cout << "\n  Team: " << teamName
              << "  |  Score: " << teamScore
              << "  |  Alive: " << getAliveCount()
              << "/" << players.size()
              << "  |  Total Kills: " << getTotalKills() << "\n";
    std::cout << "  " << std::string(66, '-') << "\n";
    for (const auto& p : players)
        p->printStats();
}

} // namespace aigo