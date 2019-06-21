//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/error.hpp>

#include <boost/hana/assert.hpp>

namespace hana = boost::hana;

int main() {
    {
      full_duplex::terminate x;
      BOOST_HANA_RUNTIME_CHECK(full_duplex::is_terminate<decltype(x)>());
    }
    {
      auto x = full_duplex::make_error(42);
      BOOST_HANA_RUNTIME_CHECK(full_duplex::is_error<decltype(x)>());
    }
}
