//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/map.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/run_async.hpp>

#include <array>
#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

namespace hana = boost::hana;

int main() {
    using full_duplex::map;
    using full_duplex::promise;
    using full_duplex::promise_lift;
    using full_duplex::run_async;
    using full_duplex::run_async_loop;
    using full_duplex::terminate;
    using full_duplex::void_input_t;

    // run_async
    {
        std::array<int, 3> xs{};

        run_async(
            promise_lift(xs.begin()),
            promise([](auto& resolve, auto itr) { *itr = 1; resolve(++itr); }),
            promise([](auto& resolve, auto itr) { *itr = 2; resolve(++itr); }),
            promise([](auto& resolve, auto itr) { *itr = 3; resolve(++itr); })
        );

        BOOST_HANA_RUNTIME_CHECK(hana::equal(xs, std::array<int, 3>{{1, 2, 3}}));
    }

    // run_async_loop
    {
        std::array<int, 7> xs{};

        run_async_loop(
            map(hana::overload_linearly([&](void_input_t) { return xs.begin(); }, hana::id)),
            promise([i{int{0}}](auto& resolve, auto itr) mutable { *itr = ++i; resolve(++itr); }),
            promise([&](auto& resolve, auto itr) {
                itr not_eq xs.end() ? resolve(itr) : resolve(terminate{});
            })
        );

        BOOST_HANA_RUNTIME_CHECK(hana::equal(xs, std::array<int, 7>{{1, 2, 3, 4, 5, 6, 7}}));
    }
}
