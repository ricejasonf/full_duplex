//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_DO_HPP
#define FULL_DUPLEX_FWD_DO_HPP

namespace full_duplex {
    struct do_fn {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const;
    };

    constexpr do_fn do_{};

    struct do_loop_fn {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const;
    };

    constexpr do_loop_fn do_loop{};

    // void_input - tag used as placeholder for initial input

    struct void_input_t { };
    constexpr void_intput_t void_input{};
}

#endif
