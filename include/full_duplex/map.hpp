//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_MAP_HPP
#define FULL_DUPLEX_MAP_HPP

#include <full_duplex/detail/promise_impl.hpp>
#include <utility>

namespace full_duplex {
    template <typename Fn>
    constexpr auto map_fn::operator()(Fn&& fn) const {
        return pmap_handler<std::decay_t<Fn>>{std::forward<Fn>(fn)};
    }

    constexpr auto map_error_fn::operator()(Fn&& fn) const {
        return pmap_error_handler<std::decay_t<Fn>>{std::forward<Fn>(fn)};
    }

    constexpr auto map_raw_fn::operator()(Fn&& fn) const {
        return pmap_raw_handler<std::decay_t<Fn>>{std::forward<Fn>(fn)};
    }

    constexpr auto catch_error_fn::operator()(Fn&& fn) const {
        return catch_handler<std::decay_t<Fn>>{std::forward<Fn>(fn)};
    }
}

#endif
