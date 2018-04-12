//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/endpoint.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/equal.hpp>
#include <vector>

namespace hana = boost::hana;
namespace event = full_duplex::event;
using full_duplex::do_;
using full_duplex::endpoint;
using full_duplex::endpoint_compose;
using full_duplex::promise;
using full_duplex::map;

template <int i>
constexpr auto push_back = map([](auto&& vec) {
    vec.push_back(i);
    return std::forward<decltype(vec)>(vec);
});

template <int i>
constexpr auto ep = full_duplex::endpoint(
    full_duplex::event::init          = push_back<i>,
    full_duplex::event::read_message  = push_back<i>,
    full_duplex::event::write_message = push_back<i> 
);

constexpr auto term_ep = full_duplex::endpoint(
    full_duplex::event::init          = map(hana::always(full_duplex::terminate{})),
    full_duplex::event::read_message  = map(hana::always(full_duplex::terminate{})),
    full_duplex::event::write_message = map(hana::always(full_duplex::terminate{})) 
);

int main() {
    {
        auto endpoint = endpoint_compose(ep<1>, ep<2>, term_ep);
        (void)endpoint;

#if 0
        // uhh need interface or something
        BOOST_HANA_RUNTIME_CHECK(hana::equal(
            
        ));
#endif
    }
}
