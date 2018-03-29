//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_FUTURE_HPP
#define FULL_DUPLEX_FWD_FUTURE_HPP

namespace full_duplex {
    struct future_tag { }

    template <typename ...>
    struct future_t<...>;

    // unresolved

    struct unresolved_t { };
    constexpr unresolved_t unresolved{}; 
}

#endif
