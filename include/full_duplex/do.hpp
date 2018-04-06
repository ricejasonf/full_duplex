//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DO_HPP
#define FULL_DUPLEX_DO_HPP

#include <full_duplex/fwd/do.hpp>
#include <full_duplex/promise.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <utility>

namespace full_duplex {
    template <typename ...Xs>
    constexpr auto do_fn::operator()(Xs&& ...xs) const {
        using Impl = decltype(hana::make_basic_tuple(std::forward<Xs&&>(xs)...));
        return detail::promise_t<Impl>{hana::make_basic_tuple(std::forward<Xs&&>(xs)...)};
    }
}

#endif
