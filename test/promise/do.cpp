//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/do.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/run_async.hpp>

#include <array>
#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>

namespace hana = boost::hana;

int main() {
    using full_duplex::do_;
    using full_duplex::promise;
    using full_duplex::promise_lift;
    using full_duplex::run_async;

    std::array<int, 3> xs{};

    run_async(
        do_(
            promise_lift(xs.begin()),
            promise([](auto& resolve, auto itr) { *itr = 1; resolve(++itr); }),
            promise([](auto& resolve, auto itr) { *itr = 2; resolve(++itr); }),
            promise([](auto& resolve, auto itr) { *itr = 3; resolve(++itr); })
        )
    );

    BOOST_HANA_RUNTIME_CHECK(hana::equal(xs, std::array<int, 3>{{1, 2, 3}}));
}
