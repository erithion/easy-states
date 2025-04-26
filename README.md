# easy-states

A tiny header-only library allowing to define a state machine using solely a function signature.

## Example

Given arbitrary enums `State` and `Event`, transitions are inferred from the set of overloaded functions `enter`, which, thus, act as a transition table and define a state machine:

```cpp
#include "easy_states/easy_states.hpp"

enum States
{
    state_1,
    state_2,
    state_3
};

enum Events
{
    event_1,
    ecent_2
};

// ...

// The signature encodes a transition state_1 --[event_1]--> state_2
State<state_2> enter(State<state_1> src, Event<event_1> evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "state_2");
    return {};
}
// The signature encodes a transition state_1 --[event_2]--> state_3
State<state_3> enter(State<state_1> src, Event<event_2> evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "state_3");
    return {};
}

int main()
{
    fmt::println("{}", easy_states::next(state_1, event_1)); // prints state_2
    fmt::println("{}", easy_states::next(state_1, event_2)); // prints state_3
    return 0;
}
```
