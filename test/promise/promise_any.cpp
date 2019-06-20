//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/do.hpp>
#include <full_duplex/map.hpp>
#include <full_duplex/promise.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/transform.hpp>
#include <type_traits>

namespace hana = boost::hana;

int main() {
    using full_duplex::make_error;
    using full_duplex::map;
    using full_duplex::map_error;
    using full_duplex::map_any;
    using full_duplex::promise;
    using full_duplex::promise_any;
    using full_duplex::promise_lift;
    using full_duplex::tap;
    using full_duplex::terminate;

    // promise_any transform valid values (non-error)
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            promise_any([](auto&resolve, auto x)
                { resolve(x * x); })
        ),
        promise_lift(int{25})
    ));

    // promise_any can map an error to a non-error
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            promise_any([](auto&resolve, auto x)
                { resolve(x.value * x.value); })
        ),
        promise_lift(int{25})
    ));

    // promise_any can map an error to a non-error
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            promise_any([](auto&resolve, auto x)
                { resolve(x.value * x.value); })
        ),
        promise_lift(int{25})
    ));
}
