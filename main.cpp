/**
 * @file main.cpp
 * @author Artem Bondartsov (erithion@gmail.com)
 * @brief Example of use of easy_states
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 */
#include "easy_states.hpp"
#include <type_traits>
#include <fmt/format.h>
#include <magic_enum.hpp>

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

template <>
constexpr States std::numeric_limits<States>::max() { return States::Unlocked; }

template <>
constexpr Events std::numeric_limits<Events>::max() { return Events::Coin; }

template <typename Enum>
struct fmt::formatter<Enum, std::enable_if_t<std::is_enum_v<Enum>, char>> : fmt::formatter<std::string_view>
{
    auto format(Enum e, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(e));
    }
};

template <States S>
using State = Const<States, S>;

template <Events E>
using Event = Const<Events, E>;

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

    state = next(state, Events::Push);
    state = next(state, Events::Coin);
    state = next(state, Events::Push);
    state = next(state, Events::Push);

    state = next(state, Events::Coin);
    state = next(state, Events::Coin);
    state = next(state, Events::Push);
    state = next(state, Events::Push);
    return 0;
}