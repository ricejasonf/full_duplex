//
// Copyright Jason Rice 2018
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
#include <cassert>
#include <type_traits>

namespace hana = boost::hana;

int main() {
    using full_duplex::catch_error;
    using full_duplex::make_error;
    using full_duplex::map;
    using full_duplex::map_error;
    using full_duplex::map_any;
    using full_duplex::promise;
    using full_duplex::promise_lift;
    using full_duplex::tap;
    using full_duplex::terminate;

    // map transform valid values (non-error)
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            map([](auto x) { return x * x; })
        ),
        promise_lift(int{25})
    ));

    // map does not transform errors
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            map([](int x) { return x * x; })
        ),
        promise_lift(make_error(int{5}))
    ));


    // map does not transform `terminate`
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(terminate{}),
            map([](int x) { return x * x; })
        ),
        promise_lift(terminate{})
    ));

    // map_error transforms the contents errors
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            map_error([](int x) { return x * x; })
        ),
        promise_lift(make_error(int{25}))
    ));

    // map_error does not transform valid values (non-error)
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            map_error([](auto x) { return x * x; })
        ),
        promise_lift(int{5})
    ));

    // map_error does not transform `terminate`
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(terminate{}),
            map_error([](int x) { return x * x; })
        ),
        promise_lift(terminate{})
    ));

    // catch_error catches a subset of `error<T>` and resolves `terminate`
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            catch_error([](int) { /* side effects */ })
        ),
        promise_lift(terminate{})
    ));

    // catch_error does not catch error types outside of the subset (via SFINAE)
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            catch_error([](terminate) { assert(false); }) // not invokable
        ),
        promise_lift(make_error(int{5}))
    ));

    // catch_error does not catch valid values
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(int{5}),
            catch_error([](int x) { return x * x; })
        ),
        promise_lift(int{5})
    ));

    // catch_error does not catch `terminate`
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(terminate{}),
            catch_error([](auto x) { static_assert(std::is_void_v<decltype(x)>); })
        ),
        promise_lift(terminate{})
    ));

    // promise does not handle terminate
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(terminate{}),
            promise([](auto&, auto x) { static_assert(std::is_void_v<decltype(x)>); })
        ),
        promise_lift(terminate{})
    ));

    // promise does not handle error
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::chain(
            promise_lift(make_error(int{5})),
            promise([](auto&, auto x) { static_assert(std::is_void_v<decltype(x)>); })
        ),
        promise_lift(make_error(int{5}))
    ));
}
