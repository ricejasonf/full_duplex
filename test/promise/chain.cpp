//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/promise.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/transform.hpp>

namespace hana = boost::hana;

int main() {
    using full_duplex::promise;
    using full_duplex::promise_lift;

    // test using promise as chaining function
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            promise([](auto& resolve, auto x) { resolve(x * x); })
        ),
        promise_lift(int{25})
    ));

    // test using non-overloaded chaining function
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            [](int x) { return promise_lift(x * x); }
        ),
        promise_lift(int{25})
    ));

    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            hana::chain(
                promise_lift(int{5}),
                [](int x) { return promise_lift(x * x); }
            ),
            [](int x) { return promise_lift(x * x); }
        ),
        promise_lift(int{25 * 25})
    ));
}
