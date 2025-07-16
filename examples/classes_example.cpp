/**
 * @file main.cpp
 * @author Artem Bondartsov (erithion@gmail.com)
 * @brief Example of use of easy_states
 * @version 0.1
 * @date 2025-07-10
 * 
 * @copyright Copyright (c) 2025
 */
#include "easy_states/easy_states.hpp"
#include <string>
#include <type_traits>
#include <fmt/format.h>
#include <magic_enum.hpp>

enum struct Events
{
    Push = 0,
    Coin
};

enum struct States
{
    Locked = 0,
    Unlocked
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

struct Turnstile : easy_states::Automata<Turnstile>
{
    using Base = easy_states::Automata<Turnstile>;

    Turnstile() : state_{States::Locked} {}
    void next(Events event, std::string str, int val)
    {
        // casting to const string& as declared in enter() below
        state_ = Base::next(state_, event, static_cast<const std::string&>(str), val);
    }

private:
    States state_;

private:
    // Helps Automata in accessing private "enter"/"exit"
    using easy_states::Automata<Turnstile>::enter;
    using easy_states::Automata<Turnstile>::exit;
    friend struct easy_states::Automata<Turnstile>;

    // For the sake of shorter names and clarity
    template <States S>
    using State = easy_states::Const<States, S>;

    template <Events E>
    using Event = easy_states::Const<Events, E>;

    // Simultaneously, a transition table and actions on state entering
    // *** Target state *********** Source state ************** Event ************* Arbitrary types...
    State<States::Locked>   enter(State<States::Locked>  , Event<Events::Push>, const std::string&, int);
    State<States::Unlocked> enter(State<States::Locked>  , Event<Events::Coin>, const std::string&, int);
    State<States::Locked>   enter(State<States::Unlocked>, Event<Events::Push>, const std::string&, int);
};

Turnstile::State<States::Locked> Turnstile::enter(Turnstile::State<States::Locked> src, Turnstile::Event<Events::Push> evt, const std::string& s, int v)
{
    using Ret = decltype(enter(src, evt, s, v));
    fmt::println("{} --[{}]--> {}, s='{}', v={}", src.value, evt.value,  Ret{}.value, s, v);
    return {};
}
Turnstile::State<States::Unlocked> Turnstile::enter(Turnstile::State<States::Locked> src, Turnstile::Event<Events::Coin> evt, const std::string& s, int v)
{
    using Ret = decltype(enter(src, evt, s, v));
    fmt::println("{} --[{}]--> {}, s='{}', v={}", src.value, evt.value,  Ret{}.value, s, v);
    return {};
}
Turnstile::State<States::Locked> Turnstile::enter(Turnstile::State<States::Unlocked> src, Turnstile::Event<Events::Push> evt, const std::string& s, int v)
{
    using Ret = decltype(enter(src, evt, s, v));
    fmt::println("{} --[{}]--> {}, s='{}', v={}", src.value, evt.value,  Ret{}.value, s, v);
    return {};
}

int main()
{
    Turnstile tur;

    tur.next(Events::Push, "s1", 1);
    tur.next(Events::Coin, "s2", 2);
    tur.next(Events::Push, "s3", 3);
    tur.next(Events::Push, "s4", 4);

    tur.next(Events::Coin, "s5", 5);
    tur.next(Events::Coin, "s6", 6);
    tur.next(Events::Push, "s7", 7);
    tur.next(Events::Push, "s8", 8);
    return 0;
}