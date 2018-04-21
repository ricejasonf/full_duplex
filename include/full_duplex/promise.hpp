//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_PROMISE_HPP
#define FULL_DUPLEX_PROMISE_HPP

#include <full_duplex/detail/holder.hpp>
#include <full_duplex/run_async.hpp>
#include <full_duplex/fwd/map.hpp>
#include <full_duplex/fwd/promise.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/ap.hpp>
#include <boost/hana/chain.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/lift.hpp>
#include <boost/hana/tap.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <functional>
#include <utility>

namespace full_duplex::detail {
    template <typename Impl>
    struct promise_t {
        using hana_tag = promise_tag;

        Impl impl;

        // calling this directly would mostly certainly result
        // in heap allocation and type erasure
        template <typename Input>
        auto operator()(Input&& input) noexcept {
            using T = std::decay_t<Input>;
            auto h = holder<T>{std::forward<Input>(input)};

            return promise([h{std::move(h)}, this](auto& resolve, auto&&) {
                run_async(
                    promise(std::move(h).value),
                    *this,
                    tap(std::ref(resolve))
                );
            });
        }
    };

    constexpr auto promise_any = [](auto&& fn) {
        auto impl = detail::async_raw_handler<std::decay_t<decltype(fn)>>{
            std::forward<decltype(fn)>(fn)
        };

        return detail::promise_t<decltype(impl)>{impl};
    };
} 

namespace full_duplex {
    template <typename AsyncFn>
    constexpr auto promise_fn::operator()(AsyncFn&& fn) const {
        auto impl = detail::async_handler<std::decay_t<AsyncFn>>{std::forward<AsyncFn>(fn)};
        return detail::promise_t<decltype(impl)>{impl};
    }
}

namespace boost::hana
{
    // Functor

    template <>
    struct transform_impl<full_duplex::promise_tag> {
        template <typename Promise, typename Fn>
        static constexpr auto apply(Promise&& p, Fn&& fn) {
            using Impl = full_duplex::detail::pmap_raw_handler<std::decay_t<Fn>>;
            auto impl = Impl{std::forward<Fn>(fn)};
            return hana::chain(
                std::forward<Promise>(p),
                full_duplex::detail::promise_t<decltype(impl)>{std::move(impl)}
            );
        }
    };

    // Applicative

    template <>
    struct ap_impl<full_duplex::promise_tag> {
        template <typename F, typename X>
        static constexpr auto apply(F&& f, X&& x) {
            return hana::chain(
                std::forward<F>(f),
                hana::partial(hana::transform, std::forward<X>(x))
            );
        }
    };

    template <>
    struct lift_impl<full_duplex::promise_tag> {
        template <typename X>
        static constexpr auto apply(X&& x) {
            return full_duplex::promise(full_duplex::detail::make_holder(std::forward<X>(x)));
        }
    };

    // Monad

    template <>
    struct chain_impl<full_duplex::promise_tag> {
        template <typename Impl>
        static constexpr auto wrap(Impl&& impl) {
            if constexpr(hana::is_a<hana::basic_tuple_tag, Impl>) {
                return std::forward<Impl>(impl);
            }
            else {
                return hana::make_basic_tuple(std::forward<Impl>(impl));
            }
        }

        template <typename M, typename Fn>
        static constexpr auto apply(M&& m, Fn&& fn) {
            auto impl = hana::concat(
                wrap(std::forward<M>(m).impl),
                wrap(full_duplex::detail::make_lazy_promise_storage(std::forward<Fn>(fn)))
            );

            return full_duplex::detail::promise_t<decltype(impl)>{std::move(impl)};
        }
    };

    // Comparable - does not wait for asyncronous operations. References would dangle

    template <>
    struct equal_impl<full_duplex::promise_tag, full_duplex::promise_tag> {
        template <typename Px, typename Py>
        static bool apply(Px const& px, Py const& py) {
            using full_duplex::detail::promise_join;
            using full_duplex::detail::promise_any;
            using full_duplex::terminate;
            using full_duplex::void_input;

            bool result = false;
            promise_join(
                hana::chain(px, promise_any([&](auto&, auto const& x) {
                    promise_join(
                        hana::chain(py, promise_any([&](auto&, auto const& y) {
                            result = hana::equal(x, y);
                        })),
                        promise_any([](auto&, auto&&) { /* does nothing */ })
                    )(void_input);
                })),
                promise_any([](auto&, auto&&) { /* does nothing */ })
            )(void_input);

            return result;
        }
    };
}

#endif
