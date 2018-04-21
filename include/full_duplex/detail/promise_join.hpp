//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_PROMISE_JOIN_HPP
#define FULL_DUPLEX_DETAIL_PROMISE_JOIN_HPP

#include <full_duplex/detail/final_promise.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/fold_right.hpp>
#include <boost/hana/functional/compose.hpp>
#include <utility>

namespace full_duplex::detail {
    namespace hana = boost::hana;

    struct promise_join_fn
    {
        template <typename Current, typename Next>
        constexpr auto operator()(Current&& current, Next&& next) const;
    };

    constexpr promise_join_fn promise_join;

    template <typename Current, typename Next>
    constexpr auto promise_join_fn::operator()(Current&& current, Next&& next) const {
        using Impl = std::decay_t<decltype(current)>;

        if constexpr(hana::is_a<promise_tag, Impl>) {
            return promise_join_fn{}(
                std::forward<Current>(current).impl,
                std::forward<Next>(next)
            );
        }
        else if constexpr(hana::is_a<hana::basic_tuple_tag, Impl>) {
            return hana::fold_right(
                std::forward<Current>(current),
                std::forward<Next>(next),
                detail::promise_join_fn{}
            );
        }
        else if constexpr(hana::is_a<async_tag, Impl>) {
            return final_promise<std::decay_t<Current>, std::decay_t<Next>>(
                std::forward<Current>(current),
                std::forward<Next>(next)
            );
        }
        else if constexpr(hana::is_a<lazy_async_tag, Impl>) {
            using LazyHolderAsync = decltype(
                make_lazy_holder_async<promise_join_fn>(std::forward<Current>(current).fn)
            );

            return final_promise<LazyHolderAsync, std::decay_t<Next>>(
                make_lazy_holder_async<promise_join_fn>(std::forward<Current>(current).fn),
                std::forward<Next>(next)
            );
        }
        else if constexpr(hana::is_a<pmap_tag, Impl>) {
            return hana::compose(
                std::forward<Next>(next),
                std::forward<Current>(current)
            );
        }
#if 0 // no future!
        else if constexpr(hana::is_a<future_tag, Impl>) {
            // TODO
            static_assert(
                std::is_void_v<Current>,
                "detail::promise_join for future_tag not implemented"
            );
        }
#endif
        else {
            static_assert(
                std::is_void_v<Current>,
                "detail::promise_join requires a promise or promise impl"
            );
        }
    }
}

#endif
