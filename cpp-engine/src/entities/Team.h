#pragma once
#include "entities/Player.h"
#include <vector>
#include <memory>
#include <string>

namespace aigo {

class Team {
private:
    std::string                            teamId;
    std::string                            teamName;
    std::vector<std::shared_ptr<Player>>   players;
    int                                    teamScore;

public:
    Team(const std::string& id, const std::string& name);

    // Getters
    std::string getTeamId()   const;
    std::string getTeamName() const;
    int         getTeamScore() const;
    int         getPlayerCount() const;
    int         getAliveCount()  const;

    std::vector<std::shared_ptr<Player>>&       getPlayers();
    const std::vector<std::shared_ptr<Player>>& getPlayers() const;

    // Actions
    void addPlayer(std::shared_ptr<Player> player);
    void addScore(int points);

    // Queries
    bool isEliminated()   const; // all players dead
    int  getTotalKills()  const;
    int  getTotalDeaths() const;

    // Display
    void printTeamStats() const;
};

} // namespace aigo