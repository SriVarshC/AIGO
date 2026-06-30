#pragma once
#include <string>

namespace aigo {

struct Position {
    float x;
    float y;
    Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

class Player {
protected:
    std::string playerId;
    std::string name;
    float health;
    float maxHealth;
    int kills;
    int deaths;
    int score;
    Position position;
    bool alive;

public:
    Player(const std::string& id, const std::string& name,
           float maxHealth = 100.0f);
    virtual ~Player() = default;

    // Getters
    std::string getPlayerId()  const;
    std::string getName()      const;
    float       getHealth()    const;
    float       getMaxHealth() const;
    int         getKills()     const;
    int         getDeaths()    const;
    int         getScore()     const;
    Position    getPosition()  const;
    bool        isAlive()      const;

    // Setters
    void setPosition(float x, float y);
    void setPosition(const Position& pos);

    // Actions
    virtual void takeDamage(float damage);
    void heal(float amount);
    void addKill();
    void addDeath();
    void addScore(int points);
    virtual void respawn(const Position& spawnPoint);
    virtual void reset();

    // Display
    virtual void printStats() const;
};

} // namespace aigo