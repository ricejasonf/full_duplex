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
#include <boost/hana/functional/id.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/transform.hpp>
#include <type_traits>

namespace hana = boost::hana;

int main() {
    using full_duplex::catch_error;
    using full_duplex::make_error;
    using full_duplex::map;
    using full_duplex::map_any;
    using full_duplex::map_error;
    using full_duplex::promise;
    using full_duplex::promise_lift;
    using full_duplex::promise_tag;
    using full_duplex::tap;
    using full_duplex::terminate;


    static_assert(hana::is_a<promise_tag>(map(hana::id)));

    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        map(hana::id)(42),
        promise_lift(42)
    ));

#ifdef FOR_EXPOSITION
    map(hana::id)(42) == promise_lift(42)
    chain(p1, p2) == concat(p1, p2)
#endif
}
