//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FUTURE_HPP
#define FULL_DUPLEX_FUTURE_HPP

#include <full_duplex/fwd/future.hpp>

#include <boost/hana/fwd/transform.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>

namespace full_duplex::detail {
    template <typename Fn>
    struct future_transform_function {
        Fn fn;
    };
}

namespace full_duplex {
    template <typename T, typename Transform>
    struct future_t
        : detail::future_transform_function<Transform>
    {
        using hana_tag = future_tag;
        using type = T;
        detail::lazy_holder<T> storage;

        future_t() = default;

        template <typename Holder, typename TransformArg>
        future_t(Holder&& h, TransformArg&& t)
            : detail::future_transform_function<Transform>(
                std::forward<TransformArg>(t))
            , storage(std::forward<Holder>(h))
        { }

        // for use with promises via `map(future<T>)`
        template <typename X>
        T& operator()(X&& x) {
            storage.assign(std::forward<X>(x));
            return this->fn(storage.force_get());
        }
    };
}

namespace boost::hana
{
    //
    // eval
    //

    template <>
    struct eval_impl<full_duplex::future_tag> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x) {
            if (x.storage.is_engaged()) {
                return x.fn(x.storage.force_get());
            }
            else {
                return x.fn(full_duplex::unresolved);
            }
        }
    };

    // Functor

    template <>
    struct transform_impl<full_duplex::future_tag> {
        template <typename F, typename Fn>
        static constexpr auto apply(F&& f, Fn&& fn) {
            using T = typename std::decay_t<F>::type;
            auto new_fn = hana::compose(std::forward<Fn>(fn), std::forward<F>(f).fn);
            return full_duplex::future_t<T, decltype(new_fn)>(
                std::forward<F>(f).storage,
                std::move(new_fn)
            );
        }
    };
}

#endif
