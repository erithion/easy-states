/**
 * @file easy_states.hpp
 * @author Artem Bondartsov (erithion@gmail.com)
 * @brief Deterministic finite automata that deduces state transitions solely from function signatures
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <limits>
#include <variant>
#include <type_traits>
#include <fmt/format.h>

template <typename T, T V>
using Const = std::integral_constant<T, V>;

template <typename Enum>
constexpr auto enum_to_variant(Enum value) // returns variant = integral_constant<Enum, state>{}
{
    // enum must specialize std::numeric_limits::max with its maximum possible value
    constexpr auto size = uint64_t(std::numeric_limits<Enum>::max()) + 1;
    return [value]<size_t... I>(std::index_sequence<I...>)
    {
        std::variant<Const<Enum, Enum(I)>...> ret; // variant with enum ids as types
        // assigning the correct value to variant
        (([value, &ret]
        {
            return Enum(I) == value ? (ret = Const<Enum, Enum(I)>{}, true) : false;
        }()) || ...);
        return ret;
    } (std::make_index_sequence<size>{});
}

template <typename State, State S, typename...Args>
void exit(Const<State, S>, Args&&...) {}

template <typename State, typename Event, State S, Event E, typename...Args>
Const<State, S> enter(Const<State, S>, Const<Event, E>, Args&&...)
{
    fmt::println("Default handler: Transition from '{}' by '{}' does not exist" , S, E);
    return {};
}

/**
 * @brief Performs the transition from 'state' by 'event' returning the next state,
 * deduced from the overloads
 *    Const<State, S> enter(Const<State, S>, Const<Event, E>),
 * where State and Event are enums.
 * 
 * If args are supplied, then the overloads must have them, too.
 */
template <typename State, typename Event, typename...Args>
State next(State state, Event event, Args&&...args)
{
    // as if in cartesian product of two sets, the visit specializes the lambda
    // for all combinations of (state, event) pairs but calls only the one
    // corresponding to the current values stored in variants.
    // it would take nested switches if done manually.
    return std::visit([&args...](auto&& state, auto&& event)
    {
        exit(state, args...);
        return enter(state, event, args...).value;
    }, enum_to_variant(state), enum_to_variant(event));
}
