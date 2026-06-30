#include "matchmaking/ELOSystem.h"
#include <cmath>

namespace aigo {

float ELOSystem::calculateExpectedScore(float ratingA, float ratingB) {
    return 1.0f / (1.0f + std::pow(10.0f, (ratingB - ratingA) / 400.0f));
}

float ELOSystem::calculateNewRating(float currentRating, float expectedScore,
                                     float actualScore, float kFactor) {
    float newRating = currentRating + kFactor * (actualScore - expectedScore);
    if (newRating < MIN_RATING) newRating = MIN_RATING;
    if (newRating > MAX_RATING) newRating = MAX_RATING;
    return newRating;
}

float ELOSystem::getKFactor(int gamesPlayed, float rating) {
    if (gamesPlayed < 30)  return K_BEGINNER;
    if (rating > 2400.0f)  return K_EXPERT;
    if (gamesPlayed > 100) return K_EXPERT;
    return K_STANDARD;
}

void ELOSystem::updateRatings(float& winnerRating, float& loserRating,
                               int winnerGames, int loserGames, float result) {
    float expW = calculateExpectedScore(winnerRating, loserRating);
    float expL = calculateExpectedScore(loserRating,  winnerRating);

    float kW = getKFactor(winnerGames, winnerRating);
    float kL = getKFactor(loserGames,  loserRating);

    float newW = calculateNewRating(winnerRating, expW, result,         kW);
    float newL = calculateNewRating(loserRating,  expL, 1.0f - result,  kL);

    winnerRating = newW;
    loserRating  = newL;
}

std::string ELOSystem::getRank(float rating) {
    if (rating >= 2400) return "GRANDMASTER";
    if (rating >= 2000) return "MASTER";
    if (rating >= 1800) return "DIAMOND";
    if (rating >= 1600) return "PLATINUM";
    if (rating >= 1400) return "GOLD";
    if (rating >= 1200) return "SILVER";
    if (rating >= 1000) return "BRONZE";
    return "IRON";
}

} // namespace aigo