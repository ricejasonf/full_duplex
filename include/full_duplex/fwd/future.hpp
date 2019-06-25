//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_FUTURE_HPP
#define FULL_DUPLEX_FWD_FUTURE_HPP

#include <boost/hana/functional/id.hpp>

namespace full_duplex {
    struct future_tag { };

    template <typename T, typename Transform = boost::hana::id_t>
    struct future_t;

    template <typename T>
    constexpr auto future = []() -> future_t<T> { return {}; };

    template <typename T, typename ...>
    struct shared_future_t;

    // unresolved

    struct unresolved_t { };
    constexpr unresolved_t unresolved{}; 
}

#endif
