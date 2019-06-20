//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_ENDPOINT_HPP
#define FULL_DUPLEX_FWD_ENDPOINT_HPP

#include <full_duplex/fwd/event.hpp>

namespace full_duplex {
    struct endpoint_tag { };

    template <typename ...>
    struct endpoint_t;

    struct endpoint_fn {
        template <typename ...Events>
        constexpr auto operator()(Events&& ...) const;
    };

    constexpr endpoint_fn endpoint{};

    struct endpoint_compose_fn {
        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...) const;
    };

    constexpr endpoint_compose_fn endpoint_compose{};
}

namespace full_duplex::event {
    struct init_t { };
    struct read_message_t { };
    struct write_message_t { };
    struct error_t { };
    struct terminate_t { };

    constexpr event_t<init_t>           init;
    constexpr event_t<read_message_t>   read_message;
    constexpr event_t<write_message_t>  write_message;
    constexpr event_t<error_t>          error;
    constexpr event_t<terminate_t>      terminate;
}

#endif
