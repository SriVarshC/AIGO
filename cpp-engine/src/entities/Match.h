#pragma once
#include "entities/Team.h"
#include "entities/Map.h"
#include <string>
#include <vector>
#include <memory>

namespace aigo {

struct MatchResult {
    std::string winnerName;
    std::string winnerId;
    int         team1Score;
    int         team2Score;
    int         totalRounds;
    int         totalEvents;
};

class Match {
private:
    std::string              matchId;
    std::shared_ptr<Team>    team1;
    std::shared_ptr<Team>    team2;
    std::shared_ptr<Map>     gameMap;
    std::vector<std::string> eventLog;
    int                      maxRounds;
    int                      currentRound;
    bool                     matchOver;
    MatchResult              result;

    void logEvent(const std::string& event);
    void simulateRound();
    void resolveCombat(Player* attacker, Player* defender, Team* atkTeam);
    void determineWinner();

public:
    Match(const std::string& id,
          std::shared_ptr<Team> t1,
          std::shared_ptr<Team> t2,
          std::shared_ptr<Map>  map,
          int maxRounds = 15);

    void             start();
    MatchResult      getResult()   const;
    const std::vector<std::string>& getEventLog() const;
    bool             isOver()      const;

    void printResults(bool verbose = true) const;
    void printEventLog(int lastN = 20)     const;
};

} // namespace aigo