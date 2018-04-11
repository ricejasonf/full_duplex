//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_ENDPOINT_HPP
#define FULL_DUPLEX_FWD_ENDPOINT_HPP

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
    struct event_tag { };

    template <typename T>
    struct event_t {
        using hana::tag = event_tag;
        constexpr auto operator=(P&& p) const;
    };

    struct init_t { };
    struct read_message_t { };
    struct write_message_t { };

    constexpr event<init_t> init;
    constexpr event<read_message_t> read_message;
    constexpr event<write_message_t> write_message;
}

#endif
