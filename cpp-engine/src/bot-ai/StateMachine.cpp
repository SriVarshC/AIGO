#include "bot-ai/StateMachine.h"
#include <iostream>
#include <iomanip>

namespace aigo {

StateMachine::StateMachine(AIState initialState)
    : currentState(initialState), previousState(initialState),
      timeInState(0.0f), totalTransitions(0) {}

void StateMachine::registerState(AIState state,
                                  std::function<void()>       onEnter,
                                  std::function<void(float)>  onUpdate,
                                  std::function<void()>       onExit) {
    StateCallbacks cb;
    cb.onEnter  = std::move(onEnter);
    cb.onUpdate = std::move(onUpdate);
    cb.onExit   = std::move(onExit);
    callbacks[state] = std::move(cb);
}

void StateMachine::addTransition(AIState from, AIState to,
                                  std::function<bool()> condition,
                                  const std::string& label) {
    transitions.emplace_back(from, to, std::move(condition), label);
}

void StateMachine::update(float deltaTime) {
    timeInState += deltaTime;

    for (const auto& t : transitions) {
        if (t.from == currentState && t.condition()) {
            forceState(t.to);
            return;
        }
    }

    auto it = callbacks.find(currentState);
    if (it != callbacks.end() && it->second.onUpdate)
        it->second.onUpdate(deltaTime);
}

void StateMachine::forceState(AIState newState) {
    if (newState == currentState) return;

    auto itCur = callbacks.find(currentState);
    if (itCur != callbacks.end() && itCur->second.onExit)
        itCur->second.onExit();

    previousState   = currentState;
    currentState    = newState;
    timeInState     = 0.0f;
    ++totalTransitions;

    auto itNew = callbacks.find(currentState);
    if (itNew != callbacks.end() && itNew->second.onEnter)
        itNew->second.onEnter();
}

AIState     StateMachine::getState()            const { return currentState; }
AIState     StateMachine::getPreviousState()    const { return previousState; }
float       StateMachine::getTimeInState()      const { return timeInState; }
int         StateMachine::getTotalTransitions() const { return totalTransitions; }
std::string StateMachine::getStateName()        const { return toString(currentState); }

std::string StateMachine::toString(AIState s) {
    switch (s) {
        case AIState::IDLE:    return "IDLE";
        case AIState::PATROL:  return "PATROL";
        case AIState::CHASE:   return "CHASE";
        case AIState::ATTACK:  return "ATTACK";
        case AIState::RETREAT: return "RETREAT";
        case AIState::DEAD:    return "DEAD";
        default:               return "UNKNOWN";
    }
}

void StateMachine::printStatus() const {
    std::cout << "  FSM | State: " << toString(currentState)
              << "  |  Time: " << std::fixed << std::setprecision(1)
              << timeInState << "s"
              << "  |  Transitions: " << totalTransitions << "\n";
}

} // namespace aigo