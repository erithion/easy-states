/**
 * @file easy_states.hpp
 * @author Artem Bondartsov (erithion@gmail.com)
 * @brief Deterministic finite automata allowing to keep state machine transitions in function signatures.
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <limits>
#include <variant>
#include <cstdint>
#include <stddef.h>
#include <type_traits>
#ifdef WITH_FMT
#include <fmt/format.h>
#endif

namespace easy_states
{
    template <typename T, T V>
    using Const = std::integral_constant<T, V>;

    namespace detail
    {
        /**
         * @brief Given an Enum { id_1, ..., id_i, ..., id_n }, the call enum_to_variant(id_i)
         * returns std::variant<Const<Enum, id_1>, ..., Const<Enum, id_i>, ..., Const<Enum, id_n>>
         * instance set to Const<id_i>{}.
         * 
         * @prerequisite: Specialized std::numeric_limits<Enum>::max().
         */
        template <typename Enum>
        constexpr auto enum_to_variant(Enum value)
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
    }

    /**
     * @brief Called unless an overload is defined for Const<State, S>, Args&&...
     */
    template <typename State, State S, typename...Args>
    void exit(Const<State, S>, Args&&...) {}

    /**
     * @brief Called unless an overload is defined for Const<State, S>, Const<Event, E>, Args&&...
     */
    template <typename State, typename Event, State S, Event E, typename...Args>
    Const<State, S> enter(Const<State, S>, Const<Event, E>, Args&&...)
    {
#ifdef WITH_FMT
        fmt::println("Default handler: Transition from '{}' by '{}' does not exist" , S, E);
#endif
        return {};
    }

    /**
     * @brief Given a 'state' of State and 'event' of Event, performs a transition from
     * 'state' by 'event' returning the next state, deduced from the overload of
     *    Const<State, ...> enter(Const<State, state>, Const<Event, event>),
     * or just 'state' if no such overload exists.
     * 
     * If args are supplied, then all the overloads must have them, too.
     */
    template <typename State, typename Event, typename...Args>
    State next(State state, Event event, Args&&...args)
    {
        // As if a cartesian product of two sets, this compiles into all combinations
        // of (state, event) pairs. It would take nested switches if done manually.
        return std::visit([&args...](auto&& state, auto&& event)
        {
            exit(state, args...);
            return enter(state, event, args...).value;
        }, detail::enum_to_variant(state), detail::enum_to_variant(event));
    }
}