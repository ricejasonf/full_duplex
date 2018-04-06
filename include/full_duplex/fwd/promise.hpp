//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_PROMISE_HPP
#define FULL_DUPLEX_FWD_PROMISE_HPP

#include <boost/hana/fwd/lift.hpp>

namespace full_duplex {
    struct promise_tag { };

    struct promise_fn {
        template <typename AsyncFn>
        constexpr auto operator()(AsyncFn&&) const;
    };

    constexpr promise_fn promise{};

    constexpr boost::hana::lift_t<promise_tag> promise_lift;
}

namespace full_duplex::detail {
    template <typename Impl>
    struct promise_t;
}

#endif
