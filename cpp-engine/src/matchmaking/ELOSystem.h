#pragma once
#include <string>

namespace aigo {

class ELOSystem {
public:
    static constexpr float INITIAL_RATING = 1200.0f;
    static constexpr float MAX_RATING     = 3000.0f;
    static constexpr float MIN_RATING     = 100.0f;
    static constexpr float K_BEGINNER     = 40.0f;  // < 30 games
    static constexpr float K_STANDARD     = 20.0f;  // 30-100 games
    static constexpr float K_EXPERT       = 10.0f;  // > 100 games OR rating > 2400

    // Core ELO math
    static float calculateExpectedScore(float ratingA, float ratingB);
    static float calculateNewRating(float currentRating, float expectedScore,
                                     float actualScore, float kFactor);
    static float getKFactor(int gamesPlayed, float rating);

    // Update both ratings after a match
    // result: 1.0 = winner wins, 0.5 = draw, 0.0 = winner loses
    static void updateRatings(float& winnerRating, float& loserRating,
                               int winnerGames,    int loserGames,
                               float result = 1.0f);

    static std::string getRank(float rating);
};

} // namespace aigo