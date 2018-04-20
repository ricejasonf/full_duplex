//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_EVENT_HPP
#define FULL_DUPLEX_FWD_EVENT_HPP

namespace full_duplex {
    struct event_tag { };

    template <typename T>
    struct event_t {
        using hana_tag = event_tag;

        template <typename P>
        constexpr auto operator=(P&& p) const;
    };
}

#endif
