//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ASYNC_HPP
#define FULL_DUPLEX_ASYNC_HPP

#include <full_duplex/fwd/promise.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/fwd/chain.hpp>
#include <boost/hana/fwd/lift.hpp>
#include <boost/hana/fwd/transform.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

namespace full_duplex::detail {
    template <typename Impl, typename = void>
    struct promise_t;

    template <typename Impl>
    struct promise_t<Impl, std::enable_if_t<hana::is_a<basic_tuple_tag, Impl>>> {
        using hana_tag = promise_tag;

        Impl impl;

        template <typename Input>
        auto operator()(Input&& input) noexcept {
            // fold promise and use future to
            // return a promise object
            // but the chain function can optimize this
        }
    };
} 

namespace full_duplex {
    template <typename AsyncFn>
    constexpr auto promise_fn::operator()(AsyncFn&& f) const {
        // TODO wrap in overloads to handle error types
        auto impl = detail::async_t{std::forward<AsyncFn>(f))};
        return detail::promise_t<decltype(impl)>{impl};
    }

    template <typename F>
    constexpr auto pmap_fn::operator()(F&& f) const {
        // TODO wrap in overloads to handle error types
        auto impl = pmap_t{std::forward<F>(f)};
        return detail::promise_t<decltype(impl)>{impl};
    }
}

namespace boost::hana
{
    // Functor

    template <>
    struct transform_impl<full_duplex::promise_tag> {
        template <typename Promise, typename F>
        static constexpr auto apply(Promise&& p, F&& f) {
            auto impl = pmap_t{std::forward<F>(f)};
            return hana::chain(
                std::forward<Promise>(p),
                detail::promise_t<decltype(impl)>{std::move(impl)}
            );
        }
    };

    // Applicative

    template <>
    struct lift_impl<full_duplex::promise_tag> {
        template <typename X>
        static constexpr auto apply(X&& x) {
            auto ret = hana::always(std::forward<X>(x));
            return promise([ret{std::move(ret)}](auto& resolve, auto&&) { resolve(ret()); });
        }
    };

    // Monad

    template <>
    struct chain_impl<full_duplex::promise_tag> {
        template <typename Impl>
        static constexpr auto wrap(Impl&& impl) {
            if constexpr(hana::is_a<hana::basic_tuple_tag>) {
                return std::forward<Impl>(impl);
            }
            else {
                return hana::make_basic_tuple(std::forward<Impl>(impl));
            }
        }

        template <typename M, typename F>
        static constexpr auto apply(M&& m, F&& f) {
            auto impl = hana::concat(wrap(std::forward<M>(m).impl), wrap(std::forward<F>(f)));

            return detail::promise_t<declype(impl)>{std::move(impl)};
        }
    };
}

#endif
