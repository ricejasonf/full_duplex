//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/do.hpp>
#include <full_duplex/future.hpp>
#include <full_duplex/map.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/run_async.hpp>

#include <boost/hana/assert.hpp>
#include <string>

namespace hana = boost::hana;

int main() {
    using full_duplex::do_;
    using full_duplex::future;
    using full_duplex::map;
    using full_duplex::run_async;
    using full_duplex::tap;

    {
        bool result = false;

        run_async(
            do_(
                map([](auto&&) { return std::string("Don't dangle me, bro."); }),
                map(future<std::string>()),
                tap([&](std::string& str) {
                  result = (str == std::string("Don't dangle me, bro."));
                })
            )
        );

        BOOST_HANA_RUNTIME_CHECK(result = true);
    }

    {
        bool result = false;

        // construct with argument
        run_async(
            do_(
                map([](auto&&) { return "Don't dangle me, bro."; }),
                map(future<std::string>()),
                tap([&](std::string& str) {
                  result = (str == std::string("Don't dangle me, bro."));
                })
            )
        );

        BOOST_HANA_RUNTIME_CHECK(result = true);
    }
}
