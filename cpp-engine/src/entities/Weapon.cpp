#include "entities/Weapon.h"
#include <iostream>
#include <cstdlib>

namespace aigo {

Weapon::Weapon(const std::string& name, WeaponType type,
               float damage, float accuracy, int maxAmmo, float cooldown)
    : name(name), type(type), damage(damage), accuracy(accuracy),
      ammo(maxAmmo), maxAmmo(maxAmmo), cooldown(cooldown), cooldownTimer(0.0f) {}

std::string Weapon::getName()     const { return name; }
WeaponType  Weapon::getType()     const { return type; }
float       Weapon::getDamage()   const { return damage; }
float       Weapon::getAccuracy() const { return accuracy; }
int         Weapon::getAmmo()     const { return ammo; }
int         Weapon::getMaxAmmo()  const { return maxAmmo; }
bool        Weapon::canFire()     const { return ammo > 0 && cooldownTimer <= 0.0f; }

float Weapon::fire() {
    if (!canFire()) return 0.0f;
    --ammo;
    cooldownTimer = cooldown;
    float roll = static_cast<float>(std::rand() % 100) / 100.0f;
    return (roll <= accuracy) ? damage : 0.0f;
}

void Weapon::reload() {
    ammo = maxAmmo;
}

void Weapon::updateCooldown(float deltaTime) {
    if (cooldownTimer > 0.0f) {
        cooldownTimer -= deltaTime;
        if (cooldownTimer < 0.0f) cooldownTimer = 0.0f;
    }
}

std::string Weapon::typeToString(WeaponType t) {
    switch (t) {
        case WeaponType::ASSAULT_RIFLE: return "Assault Rifle";
        case WeaponType::SHOTGUN:       return "Shotgun";
        case WeaponType::SNIPER:        return "Sniper Rifle";
        case WeaponType::PISTOL:        return "Pistol";
        case WeaponType::MELEE:         return "Melee";
        default:                        return "Unknown";
    }
}

void Weapon::printInfo() const {
    std::cout << "  Weapon: " << name
              << " | Type: "  << typeToString(type)
              << " | Dmg: "   << damage
              << " | Acc: "   << static_cast<int>(accuracy * 100) << "%"
              << " | Ammo: "  << ammo << "/" << maxAmmo << "\n";
}

// ── Derived constructors ─────────────────────────────────────────
AssaultRifle::AssaultRifle()
    : Weapon("M4A1", WeaponType::ASSAULT_RIFLE, 28.0f, 0.75f, 30, 0.1f) {}

Shotgun::Shotgun()
    : Weapon("SPAS-12", WeaponType::SHOTGUN, 14.0f, 0.55f, 8, 0.8f) {}

float Shotgun::fire() {
    if (!canFire()) return 0.0f;
    --ammo;
    cooldownTimer = cooldown;
    float total = 0.0f;
    for (int i = 0; i < 5; ++i) {          // 5 pellets per shot
        float roll = static_cast<float>(std::rand() % 100) / 100.0f;
        if (roll <= accuracy) total += damage;
    }
    return total;
}

SniperRifle::SniperRifle()
    : Weapon("AWM", WeaponType::SNIPER, 90.0f, 0.85f, 5, 1.5f) {}

float SniperRifle::fire() {
    if (!canFire()) return 0.0f;
    --ammo;
    cooldownTimer = cooldown;
    float roll = static_cast<float>(std::rand() % 100) / 100.0f;
    if (roll <= accuracy) {
        float headshot = static_cast<float>(std::rand() % 100) / 100.0f;
        return (headshot <= 0.25f) ? damage * 2.0f : damage;  // 25% headshot
    }
    return 0.0f;
}

Pistol::Pistol()
    : Weapon("Glock-18", WeaponType::PISTOL, 22.0f, 0.65f, 15, 0.15f) {}

} // namespace aigo