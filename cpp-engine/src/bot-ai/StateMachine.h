#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace aigo {

enum class AIState { IDLE, PATROL, CHASE, ATTACK, RETREAT, DEAD };

struct Transition {
    AIState               from;
    AIState               to;
    std::function<bool()> condition;
    std::string           label;

    Transition(AIState f, AIState t,
               std::function<bool()> cond,
               const std::string& lbl = "")
        : from(f), to(t), condition(std::move(cond)), label(lbl) {}
};

class StateMachine {
private:
    struct StateCallbacks {
        std::function<void()>      onEnter;
        std::function<void(float)> onUpdate;
        std::function<void()>      onExit;
    };

    AIState                           currentState;
    AIState                           previousState;
    std::map<AIState, StateCallbacks>  callbacks;
    std::vector<Transition>            transitions;
    float                             timeInState;
    int                               totalTransitions;

public:
    explicit StateMachine(AIState initialState);

    void registerState(AIState state,
                       std::function<void()>       onEnter,
                       std::function<void(float)>  onUpdate,
                       std::function<void()>       onExit);

    void addTransition(AIState from, AIState to,
                       std::function<bool()> condition,
                       const std::string& label = "");

    void update(float deltaTime);
    void forceState(AIState newState);

    AIState     getState()            const;
    AIState     getPreviousState()    const;
    std::string getStateName()        const;
    float       getTimeInState()      const;
    int         getTotalTransitions() const;

    static std::string toString(AIState s);
    void printStatus()                const;
};

} // namespace aigo