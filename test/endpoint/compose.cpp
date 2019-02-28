//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex/endpoint.hpp>
#include <full_duplex/endpoint_open.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/equal.hpp>
#include <functional>
#include <vector>

namespace hana = boost::hana;
namespace event = full_duplex::event;
using full_duplex::do_;
using full_duplex::endpoint;
using full_duplex::endpoint_compose;
using full_duplex::endpoint_open;
using full_duplex::make_error;
using full_duplex::map;
using full_duplex::promise;

namespace {
    template <int i>
    constexpr auto push_back = promise([](auto& resolve, auto&&) {
        resolve.get_state().push_back(i);
        resolve(full_duplex::void_input);
    });

    template <int i>
    constexpr auto ep = full_duplex::endpoint(
        full_duplex::event::init          = push_back<i>,
        full_duplex::event::read_message  = push_back<i>,
        full_duplex::event::write_message = push_back<i> 
    );

    constexpr auto term_ep = full_duplex::endpoint(
        full_duplex::event::init          = [](auto&) {
            return promise([](auto& resolve, auto&&) { resolve(make_error(42)); });
        },
        full_duplex::event::read_message  = [](auto&) { 
            return promise([](auto& resolve, auto&&) { resolve(make_error(42)); });
        },
        full_duplex::event::write_message = [](auto&) {
            return promise([](auto& resolve, auto&&) { resolve(make_error(42)); });
        }
    );

    constexpr auto term_ep2 = full_duplex::endpoint(
        full_duplex::event::init          = promise([](auto& r, auto&&) { r(make_error(42)); }),
        full_duplex::event::read_message  = promise([](auto& r, auto&&) { r(make_error(42)); }),
        full_duplex::event::write_message = promise([](auto& r, auto&&) { r(make_error(42)); })
    );
}

int main() {
    {
        auto endpoint = endpoint_compose(ep<1>, ep<2>, ep<3>, term_ep);

        std::vector<int> xs{};

        endpoint_open(
            std::ref(xs),
            std::queue<int>{},
            endpoint
        );

        BOOST_HANA_RUNTIME_CHECK(hana::equal(
            xs,
            std::vector<int>{1, 2, 3}
        ));
    }
    {
        auto endpoint = endpoint_compose(ep<1>, ep<2>, ep<3>, term_ep2);

        std::vector<int> xs{};

        endpoint_open(
            std::ref(xs),
            std::queue<int>{},
            endpoint
        );

        BOOST_HANA_RUNTIME_CHECK(hana::equal(
            xs,
            std::vector<int>{1, 2, 3}
        ));
    }
}
