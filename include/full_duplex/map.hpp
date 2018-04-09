//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_MAP_HPP
#define FULL_DUPLEX_MAP_HPP

#include <full_duplex/fwd/map.hpp>
#include <full_duplex/detail/promise_impl.hpp>
#include <full_duplex/promise.hpp>

#include <boost/hana/functional/capture.hpp>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;

    template <typename Fn>
    constexpr auto map_any_fn::operator()(Fn&& fn) const {
        using T = detail::pmap_raw_handler<std::decay_t<Fn>>;
        return detail::promise_t<T>{T{std::forward<Fn>(fn)}};
    }

    template <typename Fn>
    constexpr auto map_fn::operator()(Fn&& fn) const {
        using T = detail::pmap_handler<std::decay_t<Fn>>;
        return detail::promise_t<T>{T{std::forward<Fn>(fn)}};
    }

    template <typename Fn>
    constexpr auto map_error_fn::operator()(Fn&& fn) const {
        using T = detail::pmap_error_handler<std::decay_t<Fn>>;
        return detail::promise_t<T>{T{std::forward<Fn>(fn)}};
    }

    template <typename Fn>
    constexpr auto catch_error_fn::operator()(Fn&& fn) const {
        using T = detail::catch_error_handler<std::decay_t<Fn>>;
        return detail::promise_t<T>{T{std::forward<Fn>(fn)}};
    }

    template <typename Fn>
    constexpr auto tap_fn::operator()(Fn&& fn) const {
        auto fn_ = hana::capture()(std::forward<Fn>(fn));
        constexpr auto tap_helper = [fn_{std::move(fn_)}](auto&& input) {
            fn_(input);
            return std::forward<decltype(input)>(input);
        };
        using T = detail::pmap_handler<decltype(tap_helper)>;
        return detail::promise_t<T>{T{tap_helper}};
    }
}

#endif
