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
    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        hana::transform(
            hana::lift<full_duplex::promise_tag>(int{5}),
            hana::id
        ),
        hana::lift<full_duplex::promise_tag>(int{5})
    ));
}
