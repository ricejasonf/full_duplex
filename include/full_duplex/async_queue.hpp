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
#include <cassert>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;

    template <typename Q>
    async_queue(Q&&) -> async_queue<std::decay_t<Q>>;

    template <typename Queue>
    template <typename Q>
    constexpr async_queue<Queue>::async_queue(Q&& q)
        : queue(std::forward<Q>(q))
    { }

    template <typename Queue>
    constexpr decltype(auto) async_queue<Queue>::front() {
        return map([this](auto&&) noexcept -> decltype(auto) {
            assert(queue.size() > 0);
            return queue.front();
        });
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
