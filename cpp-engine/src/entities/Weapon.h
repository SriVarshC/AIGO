#pragma once
#include <string>

namespace aigo {

enum class WeaponType {
    ASSAULT_RIFLE,
    SHOTGUN,
    SNIPER,
    PISTOL,
    MELEE
};

class Weapon {
protected:
    std::string name;
    WeaponType  type;
    float       damage;
    float       accuracy;     // 0.0 – 1.0
    int         ammo;
    int         maxAmmo;
    float       cooldown;     // seconds between shots
    float       cooldownTimer;

public:
    Weapon(const std::string& name, WeaponType type,
           float damage, float accuracy, int maxAmmo, float cooldown);
    virtual ~Weapon() = default;

    // Getters
    std::string getName()     const;
    WeaponType  getType()     const;
    float       getDamage()   const;
    float       getAccuracy() const;
    int         getAmmo()     const;
    int         getMaxAmmo()  const;
    bool        canFire()     const;

    // Actions
    virtual float fire();           // returns damage dealt (0 on miss/empty)
    virtual void  reload();
    void          updateCooldown(float deltaTime);

    // Utility
    static std::string typeToString(WeaponType t);
    void printInfo() const;
};

// ── Derived weapon types ────────────────────────────────────────
class AssaultRifle : public Weapon {
public:
    AssaultRifle();
};

class Shotgun : public Weapon {
public:
    Shotgun();
    float fire() override; // fires multiple pellets
};

class SniperRifle : public Weapon {
public:
    SniperRifle();
    float fire() override; // high damage + headshot chance
};

class Pistol : public Weapon {
public:
    Pistol();
};

} // namespace aigo