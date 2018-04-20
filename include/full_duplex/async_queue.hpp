//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ASYNC_QUEUE_HPP
#define FULL_DUPLEX_ASYNC_QUEUE_HPP

#include <full_duplex/event.hpp>
#include <full_duplex/fwd/async_queue.hpp>
#include <full_duplex/map.hpp>
#include <full_duplex/promise.hpp>

#include <boost/hana/at_key.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;
#if 0 // TODO enable custom events instead of class specialization
    template <typename Queue, typename Events>
    template <typename Q, typename E1, typename ...Es>
    constexpr async_queue<Queue, Events>::async_queue(Q&& q, E1&& e1, Es&& ...es)
        : queue(std::forward<Q>(q))
        , events(hana::make_map(std::forward<E1>(e1), std::forward<Es>(es)...))
    { }

    template <typename Q, typename E1, typename ...Es>
    async_queue(Q&&, E1&&, E1...)
        -> async_queue<std::decay_t<Q>, decltype(hana::make_map(std::declval<Es>()...))>;

    template <typename Queue, typename Events>
    constexpr decltype(auto) async_queue<Queue, Events>::front() {
        return events[event::send_queue_front];
    }

    template <typename Queue, typename Events>
    constexpr decltype(auto) async_queue<Queue, Events>::pop() {
        return events[event::send_queue_pop];
    }

    template <typename Queue, typename Events>
    constexpr decltype(auto) async_queue<Queue, Events>::push() {
        return events[event::send_queue_push];
    }

    template <typename Queue, typename Events>
    constexpr decltype(auto) async_queue<Queue, Events>::size() {
        return events[event::send_queue_size];
    }
#endif

    template <typename Q>
    async_queue(Q&&) -> async_queue<std::decay_t<Q>>;

    template <typename Queue>
    template <typename Q>
    constexpr async_queue<Queue>::async_queue(Q&& q)
        : queue(std::forward<Q>(q))
    { }

    template <typename Queue>
    constexpr decltype(auto) async_queue<Queue>::front() {
        return map([this](auto&&) noexcept { return queue.front(); });
    }

    template <typename Queue>
    constexpr decltype(auto) async_queue<Queue>::pop() {
        return tap([this](auto&&) noexcept { queue.pop(); });
    }

    template <typename Queue>
    constexpr decltype(auto) async_queue<Queue>::push() {
        return map([this](auto&& input) noexcept {
            queue.push(std::forward<decltype(input)>(input));
            return void_input;
        });
    }

    template <typename Queue>
    constexpr decltype(auto) async_queue<Queue>::size() {
        return map([this](auto&&) noexcept { return queue.size(); });
    }
}

#endif
