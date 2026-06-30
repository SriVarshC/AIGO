#pragma once
#include "entities/Player.h"
#include "entities/Weapon.h"
#include <memory>

namespace aigo {

enum class BotState {
    IDLE,
    PATROL,
    CHASE,
    ATTACK,
    RETREAT,
    DEAD
};

class Bot : public Player {
private:
    BotState                   state;
    std::unique_ptr<Weapon>    weapon;
    float                      aggressionLevel;  // 0.0 – 1.0
    float                      detectionRange;
    float                      attackRange;
    std::string                targetId;
    Position                   targetPos;
    float                      stateTimer;

    std::string stateToString(BotState s) const;

public:
    Bot(const std::string& id, const std::string& name,
        float maxHealth = 100.0f, float aggression = 0.7f);

    // Getters
    BotState    getState()          const;
    std::string getStateName()      const;
    Weapon*     getWeapon()         const;
    float       getAggressionLevel() const;

    // AI actions
    void  setState(BotState newState);
    void  setTarget(const std::string& id, const Position& pos);
    float attackTarget();                // returns damage dealt
    void  think(float deltaTime);        // AI decision making

    // Overrides
    void takeDamage(float damage)           override;
    void respawn(const Position& spawnPt)   override;
    void printStats()                 const override;
};

} // namespace aigo