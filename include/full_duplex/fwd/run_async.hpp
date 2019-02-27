//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_RUN_ASYNC_HPP
#define FULL_DUPLEX_FWD_RUN_ASYNC_HPP

#include <utility>

namespace full_duplex {
    struct run_async_fn {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const;
    };

    constexpr run_async_fn run_async{};

    struct run_async_loop_fn {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const;
    };

    constexpr run_async_loop_fn run_async_loop{};

    // TODO Remove ugly interface when Concepts arrive
    struct run_async_with_state_fn {
        template <typename State, typename ...Xs>
        constexpr auto operator()(State, Xs&& ...) const;
    };

    constexpr run_async_with_state_fn run_async_with_state{};

    struct run_async_loop_with_state_fn {
        template <typename State, typename ...Xs>
        constexpr auto operator()(State, Xs&& ...) const;
    };

    constexpr run_async_loop_with_state_fn run_async_loop_with_state{};

    // void_input - tag used as placeholder for initial input

    struct void_input_t { };
    constexpr void_input_t void_input{};
}

#endif
