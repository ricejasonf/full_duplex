//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DO_HPP
#define FULL_DUPLEX_DO_HPP

#include <full_duplex/fwd/do.hpp>
#include <full_duplex/fwd/error.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/core/is_a.hpp>

namespace full_duplex::detail {
    template <typename Holder>
    struct promise_tail {
        using hana_tag = promise_tag;

        Holder* holder;

        promise_tail(Holder* e)
            : holder(e)
        { }

        template <typename Input>
        void operator()(Input const&) {
            static_assert(not hana::is_an<error_tag, Input>, "Unhandled Promise Error!");

            delete holder;
        }
    };

    template <typename InputPromise>
    struct promise_sum_holder
    {
        using PromiseSum = decltype(detail::promise_join(
            std::declval<InputPromise>(),
            std::declval<promise_tail<promise_sum_holder>>()
        ));

        PromiseSum promise_sum;

        promise_sum_holder(InputPromise&& p)
            : promise_sum(promise_join(std::move(p), promise_tail<promise_sum_holder>(this)))
        { }

        promise_sum_holder(promise_sum_holder const&) = delete;
    };
}
namespace full_duplex {
    namespace hana = boost::hana;

    template <typename ...Xs>
    do_fn::operator()(Xs&&... xs) const {
        using Promise = promise_t<decltype(hana::make_basic_tuple(std::forward<Xs&&>(xs)...))>;
        auto p = promise_sum_holder<Promise>{
            Promise{hana::make_basic_tuple(std::forward<Xs&&>(xs)...)}
        };

        p(void_input);
    }
}

#endif
