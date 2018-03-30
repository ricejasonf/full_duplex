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
    template <typename T, typename Transform = hana::id_t>
    struct future_t<T, Transform>
        : future_transform_function<Transform>
    {
        using hana_tag = future_tag;
        using type = T;
        lazy_holder<T> storage;

        future_t(Holder&& h, TransformArg&& t)
            : future_transform_function<Transform>(std::forward<TransformArg>(t))
            , storage(std::forward<Holder>(h))
        { }
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
                return x.fn(x.force_get());
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
            return future_t<T, decltype(new_fn)>(
                std::forward<F>(f).storage,
                std::move(new_fn)
            );
        }
    };
}

#endif
