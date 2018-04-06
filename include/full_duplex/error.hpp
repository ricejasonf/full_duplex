//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ERROR_HPP
#define FULL_DUPLEX_ERROR_HPP

#include <full_duplex/fwd/error.hpp>

#include <utility>

namespace full_duplex::detail {
    template <typename T>
    struct error_value {
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

#endif
