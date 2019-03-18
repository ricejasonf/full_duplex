//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/async_queue.hpp>
#include <full_duplex/do.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <queue>
#include <vector>


namespace hana = boost::hana;
using full_duplex::async_queue;
using full_duplex::catch_error;
using full_duplex::do_;
using full_duplex::map;
using full_duplex::promise;
using full_duplex::promise_lift;
using full_duplex::run_async;
using full_duplex::run_async_loop;
using full_duplex::tap;

int main() {
    // Test std::queue specialization of async_queue 

    {
        std::vector<int> result{};

        auto q = async_queue(std::queue<int>{});
        auto take_next = do_(
            q.front()
        ,   tap([&](int& value) { result.push_back(value); })
        ,   q.pop()
        );

        run_async(
            q.size()
        ,   tap([](std::size_t size) { BOOST_HANA_RUNTIME_ASSERT(size == 0); })
        ,   promise_lift(1)
        ,   q.push()
        ,   promise_lift(2)
        ,   q.push()
        ,   promise_lift(3)
        ,   q.push()
        ,   promise_lift(4)
        ,   q.push()
        ,   q.size()
        ,   tap([](std::size_t size) { BOOST_HANA_RUNTIME_ASSERT(size == 4); })
        ,   take_next // 1
        ,   take_next // 2
        ,   take_next // 3
        ,   take_next // 4
        ,   q.size()
        ,   tap([](std::size_t size) { BOOST_HANA_RUNTIME_ASSERT(size == 0); })
        ,   catch_error([](auto&&) { BOOST_HANA_RUNTIME_ASSERT(false); })
        );

        BOOST_HANA_RUNTIME_ASSERT(result == std::vector<int>{1, 2, 3, 4});
    }

    // Test `max_size` check in default `push` of `async_queue`

    {
        auto q = async_queue(std::queue<int>{});
        bool is_queue_full = false;

        run_async_loop(
            promise_lift(0)
        ,   q.push()
        ,   catch_error([&](full_duplex::queue_full_t) { is_queue_full = true; })
        ,   catch_error([](auto&&) { BOOST_HANA_RUNTIME_ASSERT(false); })
        );

        BOOST_HANA_RUNTIME_ASSERT(is_queue_full);
        BOOST_HANA_RUNTIME_ASSERT(q.queue.size() == q.max_size());
    }
}
