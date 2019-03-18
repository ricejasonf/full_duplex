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
    template <typename Queue>
    struct async_queue {
        template <typename Q>
        constexpr async_queue(Q&& q);

        async_queue(async_queue const&) = delete;

        constexpr decltype(auto) front(); // -> Promise
        constexpr decltype(auto) pop();   // -> Promise
        constexpr decltype(auto) push();  // -> Promise
        constexpr decltype(auto) size();  // -> Promise
        constexpr std::size_t max_size() { return 100; }

        Queue queue;
    };

    // queue_full - tag stating that the push failed
    //              because the queue reached max_size
    struct queue_full_t { } constexpr queue_full;
}

#endif
