//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_RUN_ASYNC_HPP
#define FULL_DUPLEX_RUN_ASYNC_HPP

#include <full_duplex/detail/promise_join.hpp>
#include <full_duplex/fwd/run_async.hpp>

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

    template <typename Holder>
    struct promise_loop_tail {
        using hana_tag = promise_tag;

        Holder* holder;

        promise_loop_tail(Holder* e)
            : holder(e)
        { }

        template <typename Input>
        void operator()(Input&& input) {
            static_assert(not hana::is_an<error_tag, Input>, "Unhandled Promise Error!");

            if constexpr(hana::is_a<terminate, Input>) {
                delete holder;
            }
            else {
                // start the promise over again
                holder->promise_sum(std::forward<Input>(input));
            }
        }
    };

    template <typename PromiseImpl, template <typename> typename Tail>
    struct promise_sum_holder
    {
        using PromiseSum = decltype(detail::promise_join(
            std::declval<PromiseImpl>(),
            std::declval<Tail<promise_sum_holder>>()
        ));

        PromiseSum promise_sum;

        promise_sum_holder(PromiseImpl&& p)
            : promise_sum(promise_join(std::move(p), Tail<promise_sum_holder>(this)))
        { }

        promise_sum_holder(promise_sum_holder const&) = delete;

        void invoke() { promise_sum(void_input); }
    };
}

namespace full_duplex {
    namespace hana = boost::hana;

    template <typename ...Xs>
    constexpr auto run_async_fn::operator()(Xs&&... xs) const {
        using Impl = decltype(hana::make_basic_tuple(std::forward<Xs&&>(xs)...));
        auto p = new detail::promise_sum_holder<Impl, detail::promise_tail>{
            hana::make_basic_tuple(std::forward<Xs&&>(xs)...)
        };

        p->invoke();
    }

    template <typename ...Xs>
    constexpr auto run_async_loop_fn::operator()(Xs&&... xs) const {
        using Impl = decltype(hana::make_basic_tuple(std::forward<Xs&&>(xs)...));
        auto p = new detail::promise_sum_holder<Impl, detail::promise_loop_tail>{
            hana::make_basic_tuple(std::forward<Xs&&>(xs)...)
        };

        p->invoke();
    }
}

#endif
