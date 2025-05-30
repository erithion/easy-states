/**
 * @file main.cpp
 * @author Artem Bondartsov (erithion@gmail.com)
 * @brief Example of use of easy_states
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 */
#include "easy_states/easy_states.hpp"
#include <type_traits>
#include <fmt/format.h>
#include <magic_enum.hpp>

// States and events
enum struct States
{
    Locked = 0,
    Unlocked
};

enum struct Events
{
    Push = 0,
    Coin
};

// Enums used in easy_states::next must specialize std::numeric_limits indicating their last value
template <>
constexpr States std::numeric_limits<States>::max() { return States::Unlocked; }

template <>
constexpr Events std::numeric_limits<Events>::max() { return Events::Coin; }

// Formatter for any enum
template <typename Enum>
struct fmt::formatter<Enum, std::enable_if_t<std::is_enum_v<Enum>, char>> : fmt::formatter<std::string_view>
{
    auto format(Enum e, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(e));
    }
};

// For the sake of shorter names and clarity
template <States S>
using State = easy_states::Const<States, S>;

template <Events E>
using Event = easy_states::Const<Events, E>;

// Simultaneously, a transition table and actions on state entering
// *** Target state **************** Source state ******************* Event *************
State<States::Locked>   enter(State<States::Locked>     src, Event<Events::Push>    evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "Locked");
    return {};
}
State<States::Unlocked> enter(State<States::Locked>     src, Event<Events::Coin>    evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "Unlocked");
    return {};
}
State<States::Locked>   enter(State<States::Unlocked>   src, Event<Events::Push>    evt)
{
    fmt::println("{} --[{}]--> {}", src.value, evt.value, "Locked");
    return {};
}

int main()
{
    auto state = States::Locked;

    state = easy_states::next(state, Events::Push);
    state = easy_states::next(state, Events::Coin);
    state = easy_states::next(state, Events::Push);
    state = easy_states::next(state, Events::Push);

    state = easy_states::next(state, Events::Coin);
    state = easy_states::next(state, Events::Coin);
    state = easy_states::next(state, Events::Push);
    state = easy_states::next(state, Events::Push);
    return 0;
}