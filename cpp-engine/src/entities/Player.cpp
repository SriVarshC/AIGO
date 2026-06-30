#include "entities/Player.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace aigo {

Player::Player(const std::string& id, const std::string& name, float maxHealth)
    : playerId(id), name(name), health(maxHealth), maxHealth(maxHealth),
      kills(0), deaths(0), score(0), position(0.0f, 0.0f), alive(true) {}

std::string Player::getPlayerId()  const { return playerId; }
std::string Player::getName()      const { return name; }
float       Player::getHealth()    const { return health; }
float       Player::getMaxHealth() const { return maxHealth; }
int         Player::getKills()     const { return kills; }
int         Player::getDeaths()    const { return deaths; }
int         Player::getScore()     const { return score; }
Position    Player::getPosition()  const { return position; }
bool        Player::isAlive()      const { return alive; }

void Player::setPosition(float x, float y)       { position.x = x; position.y = y; }
void Player::setPosition(const Position& pos)     { position = pos; }

void Player::takeDamage(float damage) {
    if (!alive) return;
    health -= damage;
    if (health <= 0.0f) {
        health = 0.0f;
        alive  = false;
        addDeath();
    }
}

void Player::heal(float amount) {
    if (!alive) return;
    health = std::min(health + amount, maxHealth);
}

void Player::addKill()         { kills++;       score += 100; }
void Player::addDeath()        { deaths++; }
void Player::addScore(int pts) { score += pts; }

void Player::respawn(const Position& spawnPoint) {
    health   = maxHealth;
    alive    = true;
    position = spawnPoint;
}

void Player::reset() {
    health   = maxHealth;
    kills    = 0;
    deaths   = 0;
    score    = 0;
    alive    = true;
    position = Position(0.0f, 0.0f);
}

void Player::printStats() const {
    std::cout << "  [Player] " << std::left  << std::setw(12) << name
              << " | HP: "     << std::right << std::setw(7)  << std::fixed
              << std::setprecision(1) << health
              << " | K: "  << std::setw(3) << kills
              << " | D: "  << std::setw(3) << deaths
              << " | Score: " << score << "\n";
}

} // namespace aigo
