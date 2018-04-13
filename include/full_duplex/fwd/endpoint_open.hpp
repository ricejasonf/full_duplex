//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_ENDPOINT_OPEN_HPP
#define FULL_DUPLEX_FWD_ENDPOINT_OPEN_HPP

namespace full_duplex {
    template <typename ...>
    struct endpoint_instance_t;
  
    struct endpoint_open_fn {
        template <typename Input, typename SendQueue, typename Endpoint>
        constexpr auto operator()(Input&&, SendQueue&&, Endpoint&&) const;
    };

    constexpr endpoint_open_fn endpoint_open{};
}

#endif
