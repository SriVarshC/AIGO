cmake_minimum_required(VERSION 3.20)
project(aigo_sim VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/entities/Player.cpp
    src/entities/Weapon.cpp
    src/entities/Bot.cpp
    src/entities/Team.cpp
    src/entities/Map.cpp
    src/entities/Match.cpp
    src/matchmaking/ELOSystem.cpp
    src/matchmaking/MatchmakingQueue.cpp
    src/bot-ai/StateMachine.cpp
    src/bot-ai/AStarPathfinder.cpp
    src/simulation/main.cpp
)

add_executable(aigo_sim ${SOURCES})
target_include_directories(aigo_sim PRIVATE src)