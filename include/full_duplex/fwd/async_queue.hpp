//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_ASYNC_QUEUE_HPP
#define FULL_DUPLEX_FWD_ASYNC_QUEUE_HPP

namespace full_duplex {
    template <typename Queue, typename Events>
    struct async_queue {
        template <typename Q, typename E1, typename ...Es>
        constexpr async_queue(Q&&, E1&&, Es&& ...);

        constexpr async_queue(std::queue<T, Container>);

        constexpr auto front(); // -> Promise
        constexpr auto push();  // -> Promise
        constexpr auto pop();   // -> Promise
    };
}

#endif
