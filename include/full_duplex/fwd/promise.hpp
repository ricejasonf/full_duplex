//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_PROMISE_HPP
#define FULL_DUPLEX_FWD_PROMISE_HPP

namespace full_duplex {
    struct promise_tag { };

    struct promise_fn {
        template <typename AsyncFn>
        constexpr auto operator()(AsyncFn&&) const;
    };

    constexpr promise_fn promise{};

    //
    // error
    //

    struct error_tag { };

    template <typename T, typename = void>
    struct error;

    struct make_error_fn {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr make_error_fn make_error;

    //
    // terminate
    //

    struct terminate { };
}

#endif
