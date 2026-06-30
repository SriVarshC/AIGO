#include "entities/Bot.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

namespace aigo {

Bot::Bot(const std::string& id, const std::string& name,
         float maxHealth, float aggression)
    : Player(id, name, maxHealth),
      state(BotState::PATROL),
      weapon(std::make_unique<AssaultRifle>()),
      aggressionLevel(aggression),
      detectionRange(50.0f),
      attackRange(25.0f),
      targetPos(0.0f, 0.0f),
      stateTimer(0.0f) {}

BotState    Bot::getState()           const { return state; }
std::string Bot::getStateName()       const { return stateToString(state); }
Weapon*     Bot::getWeapon()          const { return weapon.get(); }
float       Bot::getAggressionLevel() const { return aggressionLevel; }

std::string Bot::stateToString(BotState s) const {
    switch (s) {
        case BotState::IDLE:    return "IDLE";
        case BotState::PATROL:  return "PATROL";
        case BotState::CHASE:   return "CHASE";
        case BotState::ATTACK:  return "ATTACK";
        case BotState::RETREAT: return "RETREAT";
        case BotState::DEAD:    return "DEAD";
        default:                return "UNKNOWN";
    }
}

void Bot::setState(BotState newState) {
    state      = newState;
    stateTimer = 0.0f;
}

void Bot::setTarget(const std::string& id, const Position& pos) {
    targetId  = id;
    targetPos = pos;
}

float Bot::attackTarget() {
    if (!alive || state != BotState::ATTACK) return 0.0f;
    if (!weapon) return 0.0f;
    if (weapon->canFire()) return weapon->fire();
    weapon->reload();
    return 0.0f;
}

void Bot::think(float deltaTime) {
    if (!alive) { state = BotState::DEAD; return; }
    stateTimer += deltaTime;
    weapon->updateCooldown(deltaTime);

    // Low health → retreat
    if (health < maxHealth * 0.25f && state != BotState::RETREAT) {
        setState(BotState::RETREAT);
        return;
    }
    // Has target + aggressive → attack
    if (!targetId.empty() && aggressionLevel > 0.5f) {
        if (state != BotState::ATTACK) setState(BotState::ATTACK);
        return;
    }
    // Idle ↔ Patrol cycling
    if (state == BotState::PATROL && stateTimer > 3.0f) {
        if ((std::rand() % 100) < 20) setState(BotState::IDLE);
    } else if (state == BotState::IDLE && stateTimer > 2.0f) {
        setState(BotState::PATROL);
    }
}

void Bot::takeDamage(float damage) {
    Player::takeDamage(damage);
    if (!alive) {
        state = BotState::DEAD;
    } else if (health < maxHealth * 0.3f) {
        setState(BotState::RETREAT);
    } else if (state != BotState::ATTACK) {
        setState(BotState::CHASE);
    }
}

void Bot::respawn(const Position& spawnPt) {
    Player::respawn(spawnPt);
    state    = BotState::PATROL;
    targetId = "";
    if (weapon) weapon->reload();
}

void Bot::printStats() const {
    std::cout << "  [Bot]    " << std::left  << std::setw(12) << name
              << " | HP: "     << std::right << std::setw(7)  << std::fixed
              << std::setprecision(1) << health
              << " | K: "  << std::setw(3) << kills
              << " | D: "  << std::setw(3) << deaths
              << " | Score: " << std::setw(6) << score
              << " | State: " << stateToString(state) << "\n";
}

} // namespace aigo