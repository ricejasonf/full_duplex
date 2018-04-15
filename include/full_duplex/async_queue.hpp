//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ASYNC_QUEUE_HPP
#define FULL_DUPLEX_ASYNC_QUEUE_HPP

#include <full_duplex/fwd/async_queue.hpp>
#include <full_duplex/map.hpp>
#include <full_duplex/promise.hpp>

#include <boost/hana/map.hpp>
#include <utility>

namespace full_duplex::detail {
    constexpr auto async_queue_default_events = hana::make_map(
        event::send_queue_pop   = map([](auto& queue) noexcept { return queue.pop(); }),
        event::send_queue_front = map([](auto& queue) noexcept { return queue.front(); }),
        event::send_queue_push  = map([](auto&& pair) noexcept {
            hana::first(pair).get().push(hana::second(std::forward<decltype(pair)>(pair)));
            return void_input;
        })
    );

    using async_queue_default_events_t = std::decay_t<decltype(async_queue_default_events)>;
}

namespace full_duplex {
    template <typename Q, typename E1, typename ...Es>
    constexpr async_queue::async_queue(Q&& q, E1&& e1, Es&& ...es)
        : queue(std::forward<Q>(q))
        , events(hana::make_map(std::forward<E1>(e1), std::forward<Es>(es)...))
    { }

    template <typename Q, typename ...E>
    async_queue(Q&&, E&&) -> async_queue<std::decay_t<Q>, decltype(hana::make_map(std::declval<E>()...))>;

    template <typename T, typename Container>
    async_queue(std::queue<T, Container>) -> async_queue<std::queue<T, Container>, async_queue_default_events_t>;


    //TODO implement front, push, pop member functions (and the endpoint events too (in other file))
}

#endif
