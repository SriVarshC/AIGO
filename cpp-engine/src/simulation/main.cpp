#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

#include "entities/Player.h"
#include "entities/Bot.h"
#include "entities/Team.h"
#include "entities/Map.h"
#include "entities/Match.h"
#include "matchmaking/ELOSystem.h"
#include "matchmaking/MatchmakingQueue.h"
#include "bot-ai/AStarPathfinder.h"
#include "bot-ai/StateMachine.h"

using namespace aigo;

void separator(char c = '=', int width = 68) {
    std::cout << "  " << std::string(width, c) << "\n";
}

// ================================================================
// PHASE 2 -- 3v3 Simulation
// ================================================================
void runPhase2() {
    separator('-');
    std::cout << "  PHASE 2: C++ SIMULATION ENGINE -- 3v3 DEMO MATCH\n";
    separator('-');

    auto demoMap = std::make_shared<Map>("Battlefield Alpha", 100.0f, 100.0f);
    auto tAlpha  = std::make_shared<Team>("T1", "Team Alpha");
    auto tBravo  = std::make_shared<Team>("T2", "Team Bravo");

    tAlpha->addPlayer(std::make_shared<Player>("T1_P1", "Striker",  160.0f));
    tAlpha->addPlayer(std::make_shared<Player>("T1_P2", "Vanguard", 140.0f));
    tAlpha->addPlayer(std::make_shared<Bot>   ("T1_B1", "Ghost",    130.0f, 0.80f));
    tBravo->addPlayer(std::make_shared<Bot>   ("T2_B1", "Cipher",   130.0f, 0.70f));
    tBravo->addPlayer(std::make_shared<Bot>   ("T2_B2", "Phantom",  120.0f, 0.90f));
    tBravo->addPlayer(std::make_shared<Player>("T2_P1", "Maverick", 150.0f));

    Match demoMatch("DEMO_3v3", tAlpha, tBravo, demoMap, 15);
    demoMatch.start();
    demoMatch.printEventLog(10);
    demoMatch.printResults(false);
}

// ================================================================
// PHASE 3 -- ELO Matchmaking Tournament
// ================================================================
void runPhase3() {
    separator();
    std::cout << "  PHASE 3: ELO MATCHMAKING ENGINE\n";
    separator();

    std::vector<PlayerProfile> allProfiles = {
        PlayerProfile("P1", "Titan",  1820.0f, 52, 33, 19),
        PlayerProfile("P2", "Nexus",  1650.0f, 38, 22, 16),
        PlayerProfile("P3", "Surge",  1450.0f, 25, 12, 13),
        PlayerProfile("P4", "Echo",   1410.0f, 31, 15, 16),
        PlayerProfile("P5", "Byte",   1210.0f, 18,  9,  9),
        PlayerProfile("P6", "Nova",   1180.0f, 22, 10, 12)
    };

    MatchmakingQueue queue(300.0f);
    std::cout << "\n  Enqueueing players...\n";
    for (const auto& p : allProfiles) {
        queue.enqueue(p);
        std::cout << "  + " << std::left << std::setw(8) << p.name
                  << "  ELO: " << std::right << std::setw(7)
                  << std::fixed << std::setprecision(0) << p.rating
                  << "  [" << ELOSystem::getRank(p.rating) << "]\n";
    }
    queue.printQueue();

    std::cout << "\n  Processing queue...\n\n";
    auto pairings = queue.processQueue();

    separator('-');
    std::cout << "  " << pairings.size() << " MATCHES SCHEDULED:\n";
    for (int i = 0; i < static_cast<int>(pairings.size()); ++i) {
        const auto& pr = pairings[i];
        std::cout << "  Match " << (i + 1) << ":  "
                  << std::left  << std::setw(8) << pr.player1.name
                  << " [" << std::fixed << std::setprecision(0)
                  << pr.player1.rating << "]  vs  "
                  << std::setw(8) << pr.player2.name
                  << " [" << pr.player2.rating << "]"
                  << "  |  diff: " << pr.ratingDiff << "\n";
    }
    separator('-');

    for (int i = 0; i < static_cast<int>(pairings.size()); ++i) {
        const auto& pairing = pairings[i];
        std::string mNum = std::to_string(i + 1);

        PlayerProfile* prof1 = nullptr;
        PlayerProfile* prof2 = nullptr;
        for (auto& p : allProfiles) {
            if (p.id == pairing.player1.id) prof1 = &p;
            if (p.id == pairing.player2.id) prof2 = &p;
        }

        std::cout << "\n";
        separator();
        std::cout << "  MATCH " << mNum << " / " << pairings.size() << "  --  "
                  << prof1->name << " [" << std::fixed << std::setprecision(0)
                  << prof1->rating << " " << ELOSystem::getRank(prof1->rating)
                  << "]  vs  "
                  << prof2->name << " [" << prof2->rating << " "
                  << ELOSystem::getRank(prof2->rating) << "]\n";
        separator();

        auto sp1   = std::make_shared<Player>(prof1->id, prof1->name, 150.0f);
        auto sp2   = std::make_shared<Player>(prof2->id, prof2->name, 150.0f);
        auto team1 = std::make_shared<Team>("T1", prof1->name + " Squad");
        auto team2 = std::make_shared<Team>("T2", prof2->name + " Squad");
        team1->addPlayer(sp1);
        team2->addPlayer(sp2);
        auto arena = std::make_shared<Map>("Arena " + mNum, 100.0f, 100.0f);

        Match match("MATCH_" + mNum, team1, team2, arena, 12);
        match.start();
        match.printEventLog(20);
        match.printResults(true);

        MatchResult res  = match.getResult();
        float       old1 = prof1->rating, old2 = prof2->rating;
        bool        p1won  = (res.winnerId == "T1");
        bool        isDraw = (res.winnerId == "DRAW");

        if (isDraw) {
            ELOSystem::updateRatings(prof1->rating, prof2->rating,
                                      prof1->gamesPlayed, prof2->gamesPlayed, 0.5f);
        } else if (p1won) {
            ELOSystem::updateRatings(prof1->rating, prof2->rating,
                                      prof1->gamesPlayed, prof2->gamesPlayed, 1.0f);
            ++prof1->wins; ++prof2->losses;
        } else {
            ELOSystem::updateRatings(prof2->rating, prof1->rating,
                                      prof2->gamesPlayed, prof1->gamesPlayed, 1.0f);
            ++prof2->wins; ++prof1->losses;
        }
        ++prof1->gamesPlayed; ++prof2->gamesPlayed;

        float d1 = prof1->rating - old1, d2 = prof2->rating - old2;
        std::cout << "\n  ELO UPDATE:\n  " << std::string(52, '-') << "\n";
        auto printRow = [](const std::string& n, float o, float r, float d,
                            const std::string& rank) {
            std::cout << "  " << std::left  << std::setw(10) << n
                      << std::right << std::setw(7) << std::fixed
                      << std::setprecision(0) << o
                      << "  ->  " << std::setw(7) << r
                      << "   (" << (d >= 0 ? "+" : "")
                      << std::setprecision(1) << d << ")   " << rank << "\n";
        };
        printRow(prof1->name, old1, prof1->rating, d1, ELOSystem::getRank(prof1->rating));
        printRow(prof2->name, old2, prof2->rating, d2, ELOSystem::getRank(prof2->rating));
    }

    std::sort(allProfiles.begin(), allProfiles.end(),
              [](const PlayerProfile& a, const PlayerProfile& b) {
                  return a.rating > b.rating; });

    std::cout << "\n\n"; separator();
    std::cout << "  FINAL ELO LEADERBOARD\n"; separator();
    std::cout << "  " << std::left  << std::setw(6) << "Rank" << std::setw(12) << "Name"
              << std::right << std::setw(8) << "ELO" << "  "
              << std::left  << std::setw(13) << "Tier"
              << std::right << std::setw(7) << "Games"
              << std::setw(6) << "Wins" << std::setw(7) << "Loss"
              << std::setw(9) << "WinRate\n";
    std::cout << "  " << std::string(68, '-') << "\n";
    for (int i = 0; i < static_cast<int>(allProfiles.size()); ++i) {
        const auto& p = allProfiles[i];
        std::string pos = "#" + std::to_string(i + 1);
        std::cout << "  " << std::left  << std::setw(6) << pos << std::setw(12) << p.name
                  << std::right << std::setw(8) << std::fixed << std::setprecision(0)
                  << p.rating << "  " << std::left << std::setw(13)
                  << ELOSystem::getRank(p.rating)
                  << std::right << std::setw(7) << p.gamesPlayed
                  << std::setw(6) << p.wins << std::setw(7) << p.losses
                  << std::setw(8) << std::setprecision(1) << p.getWinRate() << "%\n";
    }
}

// ================================================================
// PHASE 4a -- A* Pathfinding Demo
// ================================================================
void runPhase4a() {
    separator();
    std::cout << "  PHASE 4a: A* PATHFINDER\n";
    separator();

    // Scenario 1: Open field
    {
        std::cout << "\n  Scenario 1: Open field (no obstacles)\n";
        separator('-');
        AStarPathfinder pf(15, 7);
        GridPos s(1, 2), g(13, 5);
        auto path = pf.findPath(s, g);
        pf.printGrid(path, s, g);
        pf.printPathInfo(path, s, g);
    }

    // Scenario 2: Vertical wall with gap -- bot must detour
    {
        std::cout << "\n  Scenario 2: Wall with gap -- bot must find detour\n";
        separator('-');
        AStarPathfinder pf(15, 7);
        pf.addObstacleRect(6, 0, 1, 4);  // x=6, y=0..3 blocked
        pf.addObstacleRect(6, 6, 1, 1);  // x=6, y=6 blocked  (gap at y=4,5)
        GridPos s(1, 2), g(13, 2);
        auto path = pf.findPath(s, g);
        pf.printGrid(path, s, g);
        pf.printPathInfo(path, s, g);
    }

    // Scenario 3: Complete wall -- no path possible
    {
        std::cout << "\n  Scenario 3: Complete wall -- no path possible\n";
        separator('-');
        AStarPathfinder pf(10, 5);
        pf.addObstacleRect(4, 0, 1, 5);  // x=4 fully blocked
        GridPos s(1, 2), g(8, 2);
        auto path = pf.findPath(s, g);
        pf.printGrid(path, s, g);
        pf.printPathInfo(path, s, g);
    }
}

// ================================================================
// PHASE 4b -- FSM Bot AI Demo
// ================================================================
void runPhase4b() {
    separator();
    std::cout << "  PHASE 4b: FINITE STATE MACHINE -- BOT AI\n";
    separator();

    struct BotCtx {
        std::string name;
        float health, maxHealth;
        bool  targetVisible, targetInRange;
        float hpPct() const { return (health / maxHealth) * 100.0f; }
    };
    BotCtx ctx{"Phantom", 100.0f, 100.0f, false, false};

    StateMachine fsm(AIState::PATROL);

    auto noop   = [](){};
    auto noopDt = [](float){};

    fsm.registerState(AIState::PATROL,  noop, noopDt, noop);
    fsm.registerState(AIState::CHASE,   noop, noopDt, noop);
    fsm.registerState(AIState::ATTACK,  noop, noopDt, noop);
    fsm.registerState(AIState::RETREAT, noop,
        [&](float) {
            ctx.health += 20.0f;
            if (ctx.health > ctx.maxHealth) ctx.health = ctx.maxHealth;
        }, noop);
    fsm.registerState(AIState::IDLE,    noop, noopDt, noop);
    fsm.registerState(AIState::DEAD,    noop, noopDt, noop);

    fsm.addTransition(AIState::PATROL,  AIState::CHASE,
        [&]() { return ctx.targetVisible && !ctx.targetInRange; }, "target_spotted");
    fsm.addTransition(AIState::CHASE,   AIState::ATTACK,
        [&]() { return ctx.targetInRange; }, "in_range");
    fsm.addTransition(AIState::ATTACK,  AIState::RETREAT,
        [&]() { return ctx.hpPct() < 30.0f; }, "critical_hp");
    fsm.addTransition(AIState::ATTACK,  AIState::CHASE,
        [&]() { return !ctx.targetInRange && ctx.targetVisible; }, "target_moved");
    fsm.addTransition(AIState::RETREAT, AIState::IDLE,
        [&]() { return !ctx.targetVisible && ctx.hpPct() >= 50.0f; }, "safe_zone");
    fsm.addTransition(AIState::IDLE,    AIState::PATROL,
        [&]() { return fsm.getTimeInState() >= 2.0f; }, "idle_timeout");

    std::vector<AIState> mortalStates = {
        AIState::PATROL, AIState::CHASE, AIState::ATTACK, AIState::RETREAT
    };
    for (AIState s : mortalStates)
        fsm.addTransition(s, AIState::DEAD,
            [&]() { return ctx.health <= 0.0f; }, "killed");

    std::cout << "\n  Bot: " << ctx.name << "  |  Max HP: " << ctx.maxHealth << "\n";
    separator('-');
    std::cout << "  " << std::left
              << std::setw(8)  << "Time"
              << std::setw(9)  << "State"
              << std::right << std::setw(7) << "HP"
              << std::setw(5)  << "HP%"
              << "  " << std::left
              << std::setw(8)  << "Target"
              << std::setw(8)  << "InRange"
              << "Event\n";
    separator('-');

    struct SimStep {
        float dt; bool vis, inRange; float dmg; std::string event;
    };
    std::vector<SimStep> script = {
        {1.0f, false, false,   0.0f, "Patrolling sector A"},
        {1.0f, false, false,   0.0f, "Patrolling sector B"},
        {1.0f, true,  false,   0.0f, "Enemy detected at long range"},
        {1.0f, true,  false,   0.0f, "Closing distance..."},
        {1.0f, true,  true,    0.0f, "Attack range reached"},
        {1.0f, true,  true,  -15.0f, "Exchange of fire"},
        {1.0f, true,  true,  -30.0f, "Receiving heavy fire!"},
        {1.0f, true,  true,  -35.0f, "Critical damage taken!"},
        {1.0f, false, false,   0.0f, "Retreating, lost target"},
        {1.0f, false, false,   0.0f, "Retreating, regaining HP"},
        {1.0f, false, false,   0.0f, "HP restored, safe zone"},
        {1.0f, false, false,   0.0f, "Holding position"},
        {1.0f, false, false,   0.0f, "Idle timeout"},
        {1.0f, false, false,   0.0f, "Patrolling sector C"},
        {1.0f, false, false,   0.0f, "End of simulation"},
    };

    float totalTime = 0.0f;
    for (const auto& step : script) {
        ctx.targetVisible = step.vis;
        ctx.targetInRange = step.inRange;
        ctx.health = std::max(0.0f, ctx.health + step.dmg);

        AIState before = fsm.getState();
        fsm.update(step.dt);
        AIState after = fsm.getState();
        totalTime += step.dt;

        std::string note = (after != before)
            ? ("** " + StateMachine::toString(before)
               + " -> " + StateMachine::toString(after) + " **")
            : step.event;

        std::cout << "  t=" << std::fixed << std::setprecision(1)
                  << std::setw(4) << totalTime
                  << " | " << std::left  << std::setw(7) << fsm.getStateName()
                  << " | " << std::right << std::setw(5) << ctx.health
                  << " |" << std::setw(4) << static_cast<int>(ctx.hpPct()) << "%"
                  << " | " << std::left  << std::setw(6)
                  << (ctx.targetVisible ? "Yes" : "No")
                  << " | " << std::setw(6)
                  << (ctx.targetInRange ? "Yes" : "No")
                  << " | " << note << "\n";

        if (fsm.getState() == AIState::DEAD) break;
    }
    separator('-');
    std::cout << "  Total transitions: " << fsm.getTotalTransitions()
              << "  |  Simulation time: " << totalTime << "s\n";
}

// ================================================================
// MAIN
// ================================================================
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    separator();
    std::cout << "  AIGO - Adaptive Intelligence Game Operations Platform\n";
    std::cout << "  Phase 2: C++ Engine | Phase 3: ELO | Phase 4: Bot AI\n";
    separator();

    runPhase2();
    runPhase3();

    std::cout << "\n\n";
    separator();
    std::cout << "  PHASE 4: BOT AI -- A* PATHFINDER + FINITE STATE MACHINE\n";
    separator();

    runPhase4a();
    std::cout << "\n";
    runPhase4b();

    std::cout << "\n\n";
    separator();
    std::cout << "  [AIGO] Phase 2: C++ Simulation Engine     -- OPERATIONAL\n";
    std::cout << "  [AIGO] Phase 3: ELO Matchmaking Engine    -- OPERATIONAL\n";
    std::cout << "  [AIGO] Phase 4: Bot AI (FSM + A*)         -- OPERATIONAL\n";
    separator();
    std::cout << "\n";

    return 0;
}