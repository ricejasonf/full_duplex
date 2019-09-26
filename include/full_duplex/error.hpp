//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ERROR_HPP
#define FULL_DUPLEX_ERROR_HPP

#include <full_duplex/fwd/error.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <utility>

namespace full_duplex {
    template <typename T>
    struct error {
        using hana_tag = error_tag;
        using value_type = T;

        T value;
    };

    template <typename T>
    constexpr auto make_error_fn::operator()(T&& t) const {
        // requires copyable<T>
        return error<std::decay_t<T>>{t};
    };

    template <typename T>
    constexpr auto is_error = boost::hana::is_a<error_tag, T>;

    template <typename T>
    constexpr auto is_terminate = boost::hana::is_a<terminate, T>;
}

namespace boost::hana {
    template <>
    struct equal_impl<full_duplex::error_tag, full_duplex::error_tag> {
        template <typename X, typename Y>
        static constexpr auto apply(X const& x, Y const& y) {
            return hana::equal(x.value, y.value);
        }
    };

    template <>
    struct equal_impl<full_duplex::terminate, full_duplex::terminate> {
        static constexpr hana::true_ apply(full_duplex::terminate, full_duplex::terminate)
        { return {}; }
    };
}

#endif
