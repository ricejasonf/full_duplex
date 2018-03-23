//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ASYNC_HPP
#define FULL_DUPLEX_ASYNC_HPP

#include <full_duplex/fwd/promise.hpp>

#include <boost/hana/fwd/ap.hpp>
#include <boost/hana/fwd/chain.hpp>
#include <boost/hana/fwd/lift.hpp>
#include <boost/hana/fwd/transform.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>

namespace full_duplex::detail {
    template <typename Fn>
    struct p_function {
        Fn fn;
    };

    template <typename Fn>
    struct t_function {
        Fn fn;
    };

    // returns a FinalPromise
    template <typename Xs, typename ResolveFn>
    auto fold_promise_list(Xs&& xs, ResolveFn&& resolve) {
        return fold_right(
            std::forward<Xs>(xs),
            std::forward<ResolveFn>(resolve),
            detail::promise_join_fn{}
        );
    };
}

namespace full_duplex {
    // Resolve is the final promise which is usually a noop
    template <typename PromiseList, typename Resolve>
    struct promise_t {
        using hana_tag = promise_tag;

        Actions actions;
        ResolveFn resolve;
        Transforms transforms;

        template <typename Input>
        promise_t& operator()(Input&& input) noexcept {
            fn(resolve, std::forward<Input>(input));
            return (*this);
        }
    };
} 

namespace boost::hana
{
    // Functor

    template <>
    struct transform_impl<full_duplex::promise_tag> {
        template <typename Promise, typename F>
        static constexpr auto apply(Promise&& p, F&& f) {
            using Fn = decltype(promise.fn);
            using ResolveFn = decltype(compose(std::forward<Promise>(p).resolve, std::forward<F>(f)));

            return promise_t<Fn, ResolveFn>{
                std::forward<Promise>(p).fn,
                compose(std::forward<Promise>(p).resolve, std::forward<F>(f))
            };
        }
    };

    // Applicative

    template <>
    struct ap_impl<full_duplex::promise_tag> {
        template <typename PromiseFn, typename PromiseValue>
        static constexpr auto apply(PromiseFn&& promise_fn, PromiseValue&& promise_value) {
            return // TODO or not
        }
    };

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
        template <typename Promise, typename F>
        static constexpr auto apply(Promise&& p, F&& f) {
            using Fn = decltype(promise.fn);

            return promise_t<Fn, std::decay_t<F>>{
            };
        }
    };
}

#endif
