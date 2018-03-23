//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_PROMISE_JOIN_HPP
#define FULL_DUPLEX_DETAIL_PROMISE_JOIN_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/fold_right.hpp>
#include <utility>

namespace full_duplex::detail {
    namespace hana = boost::hana;

    template <typename Current, typename Next>
    auto promise_join_fn::operator()(Current&& current, Next&& next) const {
        using Impl = std::decay_t<decltype(current.impl)>;

        if constexpr(hana::is_a<hana::basic_tuple_tag, Impl>) {
            return hana::fold_right(
                std::forward<Current>(current).impl,
                std::forward<Next>(next),
                detail::promise_join_fn{}
            );
        }
        else if constexpr(hana::is_a<async_tag, Impl>) {
            return final_promise<std::decay_t<Current>, std::decay_t<Next>>(
                std::forward<Current>(current).impl.fn,
                std::forward<Next>(next)
            );
        }
        else if constexpr(hana::is_a<pmap_tag, Impl>) {
            return hana::compose(
                std::forward<Next>(next),
                std::forward<Current>(current).impl
            );
        }
        else if constexpr(hana::is_a<future_tag, Impl>) {
            // TODO
            static_assert(
                std::is_void_v<Current>,
                "detail::promise_join for future_tag not implemented"
            );
        }
        else {
            static_assert(
                std::is_void_v<Current>,
                "detail::promise_join requires a valid promise type"
            );
        }
    }
}

#endif
