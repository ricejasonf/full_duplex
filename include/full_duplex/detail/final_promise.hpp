//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_FINAL_PROMISE_HPP
#define FULL_DUPLEX_DETAIL_FINAL_PROMISE_HPP

#include <utility>

namespace full_duplex {
    template <typename Current, typename Next>
    struct _p {
        _p(Current&& c, Next&& n)
            : current(std::move(c))
            , next(std::move(n))
        { }

        template <typename Input>
        void operator()(Input&& input) {
            current(next, std::forward<Input>(input));
        }

        decltype(auto) get_state() {
            return next.get_state();
        }

    private:
        Current current;
        Next next;
    };

    // composed function where Current is just
    // a function that returns a value
    template <typename Current, typename Next>
    struct _pc {
        _pc(Current&& c, Next&& n)
            : current(std::move(c))
            , next(std::move(n))
        { }

        template <typename Input>
        void operator()(Input&& input) {
            next(current(std::forward<Input>(input)));
        }

        decltype(auto) get_state() {
            return next.get_state();
        }

    private:
        Current current;
        Next next;
    };
}

namespace full_duplex::detail {
    template <typename Current, typename Next>
    using final_promise = _p<Current, Next>;

    template <typename Current, typename Next>
    using final_promise_compose = _pc<Current, Next>;
}

#endif
