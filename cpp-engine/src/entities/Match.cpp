#include "entities/Match.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>

namespace aigo {

Match::Match(const std::string& id,
             std::shared_ptr<Team> t1,
             std::shared_ptr<Team> t2,
             std::shared_ptr<Map>  map,
             int maxRounds)
    : matchId(id), team1(t1), team2(t2), gameMap(map),
      maxRounds(maxRounds), currentRound(0), matchOver(false) {}

void Match::logEvent(const std::string& event) {
    std::ostringstream oss;
    oss << "[R" << std::setw(2) << std::setfill('0') << currentRound
        << "] " << event;
    eventLog.push_back(oss.str());
}

void Match::resolveCombat(Player* attacker, Player* defender, Team* atkTeam) {
    if (!attacker->isAlive() || !defender->isAlive()) return;

    // Random damage 15–40 per hit
    float damage  = 15.0f + static_cast<float>(std::rand() % 26);
    bool  wasAlive = defender->isAlive();
    defender->takeDamage(damage);

    std::ostringstream ev;
    ev << std::fixed << std::setprecision(1);
    ev << attacker->getName() << " -> " << defender->getName()
       << "  [dmg: " << damage
       << "  hp left: " << defender->getHealth() << "]";

    if (wasAlive && !defender->isAlive()) {
        attacker->addKill();
        atkTeam->addScore(150);
        ev << "  *** ELIMINATED ***";
    }
    logEvent(ev.str());
}

void Match::simulateRound() {
    ++currentRound;
    logEvent("===== Round Start =====");

    auto& t1p = team1->getPlayers();
    auto& t2p = team2->getPlayers();

    // Team 1 attacks Team 2
    for (auto& p1 : t1p) {
        if (!p1->isAlive()) continue;
        std::vector<Player*> targets;
        for (auto& p2 : t2p)
            if (p2->isAlive()) targets.push_back(p2.get());
        if (targets.empty()) break;
        resolveCombat(p1.get(),
                      targets[std::rand() % targets.size()],
                      team1.get());
    }

    // Team 2 attacks Team 1
    for (auto& p2 : t2p) {
        if (!p2->isAlive()) continue;
        std::vector<Player*> targets;
        for (auto& p1 : t1p)
            if (p1->isAlive()) targets.push_back(p1.get());
        if (targets.empty()) break;
        resolveCombat(p2.get(),
                      targets[std::rand() % targets.size()],
                      team2.get());
    }

    // Check end conditions
    if (team1->isEliminated() || team2->isEliminated() || currentRound >= maxRounds)
        matchOver = true;
}

void Match::determineWinner() {
    result.team1Score  = team1->getTeamScore();
    result.team2Score  = team2->getTeamScore();
    result.totalRounds = currentRound;
    result.totalEvents = static_cast<int>(eventLog.size());

    bool t1elim = team1->isEliminated();
    bool t2elim = team2->isEliminated();

    if (!t1elim && t2elim) {
        result.winnerId   = team1->getTeamId();
        result.winnerName = team1->getTeamName();
    } else if (t1elim && !t2elim) {
        result.winnerId   = team2->getTeamId();
        result.winnerName = team2->getTeamName();
    } else if (result.team1Score > result.team2Score) {
        result.winnerId   = team1->getTeamId();
        result.winnerName = team1->getTeamName();
    } else if (result.team2Score > result.team1Score) {
        result.winnerId   = team2->getTeamId();
        result.winnerName = team2->getTeamName();
    } else {
        result.winnerId   = "DRAW";
        result.winnerName = "DRAW";
    }
}

void Match::start() {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  MATCH ID : " << matchId << "\n";
    std::cout << "  TEAMS    : " << team1->getTeamName()
              << "  vs  "        << team2->getTeamName() << "\n";
    gameMap->printInfo();
    std::cout << std::string(70, '=') << "\n";

    // Spawn all players
    for (auto& p : team1->getPlayers())
        p->setPosition(gameMap->getRandomSpawnPoint(0));
    for (auto& p : team2->getPlayers())
        p->setPosition(gameMap->getRandomSpawnPoint(1));

    while (!matchOver)
        simulateRound();

    determineWinner();
}

MatchResult Match::getResult()   const { return result; }
const std::vector<std::string>& Match::getEventLog() const { return eventLog; }
bool Match::isOver()             const { return matchOver; }

void Match::printResults(bool verbose) const {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  MATCH OVER -- " << matchId << "\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << "  WINNER  : " << result.winnerName << "\n";
    std::cout << "  SCORE   : " << team1->getTeamName() << " "
              << result.team1Score << "  --  "
              << result.team2Score << " " << team2->getTeamName() << "\n";
    std::cout << "  ROUNDS  : " << result.totalRounds << "\n";
    std::cout << "  EVENTS  : " << result.totalEvents << "\n";
    if (verbose) {
        team1->printTeamStats();
        team2->printTeamStats();
    }
}

void Match::printEventLog(int lastN) const {
    std::cout << "\n-- Combat Log (last " << lastN << " events) --\n";
    int start = std::max(0, static_cast<int>(eventLog.size()) - lastN);
    for (int i = start; i < static_cast<int>(eventLog.size()); ++i)
        std::cout << "  " << eventLog[i] << "\n";
}

} // namespace aigo