#pragma once
#include <string>
#include <vector>

namespace aigo {

struct PlayerProfile {
    std::string id;
    std::string name;
    float       rating;
    int         gamesPlayed;
    int         wins;
    int         losses;

    PlayerProfile(const std::string& id,   const std::string& name,
                  float rating = 1200.0f,  int gamesPlayed = 0,
                  int   wins   = 0,        int losses = 0)
        : id(id), name(name), rating(rating), gamesPlayed(gamesPlayed),
          wins(wins), losses(losses) {}

    float getWinRate() const {
        if (gamesPlayed == 0) return 0.0f;
        return static_cast<float>(wins) / static_cast<float>(gamesPlayed) * 100.0f;
    }
};

struct MatchPairing {
    PlayerProfile player1;
    PlayerProfile player2;
    float         ratingDiff;
    bool          valid;

    MatchPairing()
        : player1("", ""), player2("", ""),
          ratingDiff(0.0f), valid(false) {}

    MatchPairing(const PlayerProfile& p1, const PlayerProfile& p2)
        : player1(p1), player2(p2), valid(true) {
        ratingDiff = (p1.rating > p2.rating)
                   ? (p1.rating - p2.rating)
                   : (p2.rating - p1.rating);
    }
};

class MatchmakingQueue {
private:
    std::vector<PlayerProfile> waitingPlayers;
    float                      maxRatingDiff;
    int                        totalMatchesMade;

public:
    explicit MatchmakingQueue(float maxRatingDiff = 300.0f);

    // Queue management
    void enqueue(const PlayerProfile& player);
    bool removePlayer(const std::string& playerId);
    int  getQueueSize()        const;
    bool isEmpty()             const;
    int  getTotalMatchesMade() const;
    void setMaxRatingDiff(float diff);

    // Matchmaking
    MatchPairing              findBestMatch();   // single closest-ELO pair
    std::vector<MatchPairing> processQueue();    // drain queue into all pairs

    void printQueue() const;
};

} // namespace aigo