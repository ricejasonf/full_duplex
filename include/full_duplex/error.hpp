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
#include <boost/hana/fwd/equal.hpp>
#include <utility>

namespace full_duplex::detail {
    template <typename T>
    struct error_value {
        using hana_tag = error_tag;
        using value_type = T;

        T value;
    };
}

namespace full_duplex {
    template <typename T>
    struct error<T> : detail::error_value<T> { };

    template <typename T>
    constexpr auto make_error_fn::operator()(T&& t) const {
        return error<std::decay_t<T>>{{std::forward<T>(t)}};
    };
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
