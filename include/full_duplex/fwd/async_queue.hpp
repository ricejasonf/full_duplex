//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_ASYNC_QUEUE_HPP
#define FULL_DUPLEX_FWD_ASYNC_QUEUE_HPP

#include <full_duplex/fwd/event.hpp>
#include <queue>

namespace full_duplex {
#if 0 // TODO finish using events as means of customization
    template <typename Queue, typename Events = void>
    struct async_queue {
        template <typename Q, typename E1, typename ...Es>
        constexpr async_queue(Q&&, E1&&, Es&& ...);

        template <typename T, typename Container>
        constexpr async_queue(std::queue<T, Container>);

        constexpr decltype(auto) front(); // -> Promise
        constexpr decltype(auto) pop();   // -> Promise
        constexpr decltype(auto) push();  // -> Promise
        constexpr decltype(auto) size();  // -> Promise

        Queue queue;
    private:
        Events events;
    };
#endif

    template <typename Queue>
    struct async_queue {
        template <typename Q>
        constexpr async_queue(Q&& q);

        async_queue(async_queue const&) = delete;

        constexpr decltype(auto) front(); // -> Promise
        constexpr decltype(auto) pop();   // -> Promise
        constexpr decltype(auto) push();  // -> Promise
        constexpr decltype(auto) size();  // -> Promise

        Queue queue;
    };
}

namespace full_duplex::event {
    struct send_queue_front_t { };
    struct send_queue_pop_t { };
    struct send_queue_push_t { };
    struct send_queue_size_t { };

    constexpr event_t<send_queue_front_t>   send_queue_front;
    constexpr event_t<send_queue_pop_t>     send_queue_pop;
    constexpr event_t<send_queue_push_t>    send_queue_push;
    constexpr event_t<send_queue_size_t>    send_queue_size;
}

#endif
