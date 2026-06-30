#include "matchmaking/MatchmakingQueue.h"
#include "matchmaking/ELOSystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

namespace aigo {

MatchmakingQueue::MatchmakingQueue(float maxRatingDiff)
    : maxRatingDiff(maxRatingDiff), totalMatchesMade(0) {}

void MatchmakingQueue::enqueue(const PlayerProfile& player) {
    waitingPlayers.push_back(player);
}

bool MatchmakingQueue::removePlayer(const std::string& playerId) {
    auto it = std::find_if(waitingPlayers.begin(), waitingPlayers.end(),
                [&](const PlayerProfile& p) { return p.id == playerId; });
    if (it != waitingPlayers.end()) {
        waitingPlayers.erase(it);
        return true;
    }
    return false;
}

int  MatchmakingQueue::getQueueSize()        const { return static_cast<int>(waitingPlayers.size()); }
bool MatchmakingQueue::isEmpty()             const { return waitingPlayers.empty(); }
int  MatchmakingQueue::getTotalMatchesMade() const { return totalMatchesMade; }
void MatchmakingQueue::setMaxRatingDiff(float diff) { maxRatingDiff = diff; }

MatchPairing MatchmakingQueue::findBestMatch() {
    if (waitingPlayers.size() < 2) return MatchPairing();

    // Sort ascending by rating
    std::sort(waitingPlayers.begin(), waitingPlayers.end(),
              [](const PlayerProfile& a, const PlayerProfile& b) {
                  return a.rating < b.rating;
              });

    int   bestI = -1, bestJ = -1;
    float bestDiff = maxRatingDiff + 1.0f;

    for (int i = 0; i < static_cast<int>(waitingPlayers.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(waitingPlayers.size()); ++j) {
            float diff = std::abs(waitingPlayers[i].rating - waitingPlayers[j].rating);
            if (diff < bestDiff) {
                bestDiff = diff;
                bestI    = i;
                bestJ    = j;
            }
        }
    }

    if (bestI < 0 || bestDiff > maxRatingDiff) return MatchPairing();

    MatchPairing pairing(waitingPlayers[bestI], waitingPlayers[bestJ]);
    // Erase higher index first to avoid position shift
    waitingPlayers.erase(waitingPlayers.begin() + bestJ);
    waitingPlayers.erase(waitingPlayers.begin() + bestI);
    ++totalMatchesMade;
    return pairing;
}

std::vector<MatchPairing> MatchmakingQueue::processQueue() {
    std::vector<MatchPairing> results;
    while (waitingPlayers.size() >= 2) {
        MatchPairing p = findBestMatch();
        if (!p.valid) break;
        results.push_back(p);
    }
    return results;
}

void MatchmakingQueue::printQueue() const {
    std::vector<PlayerProfile> sorted = waitingPlayers;
    std::sort(sorted.begin(), sorted.end(),
              [](const PlayerProfile& a, const PlayerProfile& b) {
                  return a.rating > b.rating;
              });

    std::cout << "\n  Matchmaking Queue (" << sorted.size() << " players waiting)\n";
    std::cout << "  " << std::string(65, '-') << "\n";
    std::cout << "  " << std::left  << std::setw(12) << "Name"
              << std::right << std::setw(9)  << "Rating"
              << "  "
              << std::left  << std::setw(13) << "Rank"
              << std::right << std::setw(7)  << "Games"
              << std::setw(6)  << "Wins"
              << std::setw(9)  << "WinRate\n";
    std::cout << "  " << std::string(65, '-') << "\n";

    for (const auto& p : sorted) {
        std::cout << "  " << std::left  << std::setw(12) << p.name
                  << std::right << std::setw(9)  << std::fixed
                  << std::setprecision(0) << p.rating
                  << "  "
                  << std::left  << std::setw(13) << ELOSystem::getRank(p.rating)
                  << std::right << std::setw(7)  << p.gamesPlayed
                  << std::setw(6)  << p.wins
                  << std::setw(8)  << std::setprecision(1)
                  << p.getWinRate() << "%\n";
    }
}

} // namespace aigo