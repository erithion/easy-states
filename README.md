# easy-states

A tiny header-only library allowing to define a state machine using solely a function signature.

## Example

```cpp
#include "easy_states/easy_states.hpp"

enum States
{
    state_1,
    state_2
};

enum Events
{
    event_1
};

template <>
constexpr States std::numeric_limits<States>::max() { return state_2; }

template <>
constexpr Events std::numeric_limits<Events>::max() { return event_1; }

template <States S>
using State = easy_states::Const<States, S>;

template <Events E>
using Event = easy_states::Const<Events, E>;

State<state_2> enter(State<state_1> src, Event<event_1> evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "state_2");
    return {};
}

int main()
{
    auto state = easy_states::next(States::state_1, Events::event_1);
    fmt::println("{}", state); // prints state_2
    return 0;
}
```