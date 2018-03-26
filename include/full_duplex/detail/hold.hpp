//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_HOLDER_HPP
#define FULL_DUPLEX_DETAIL_HOLDER_HPP

#include <full_duplex/detail/promise_impl.hpp>

#include <memory>
#include <utility>

namespace full_duplex::detail {

    //
    // holder
    //

    template <typename T>
    struct holder {
        T value;

        template <typename Resolve, typename Input>
        void operator()(Resolve& resolve, Input&&) /* mutable */ {
            resolve(value);
        }
    };

    constexpr auto make_holder = [](auto&& fn) {
        return holder<std::decay_t<decltype(fn)>(std::forward<decltype(fn)>(fn));
    };

    //
    // lazy_holder
    //

    template <typename Fn>
    struct lazy_holder_function {
      Fn fn;
    };

    template <typename Fn, typename T>
    struct lazy_holder
      : lazy_holder_function<Fn>
    {
        lazy_holder(Fn fn)
            : lazy_holder_function<Fn>{std::move(fn)}
            , null_state()
            , engaged(false)
        { }

        // The move constructor default initializes
        // the value because it is assumed the instance
        // is settled after the promise is initialized
        // (ie it is never moved once `value` is set)
        lazy_holder(lazy_holder&& old)
            : lazy_holder_function<Fn>{std::move(old.fn)}
            , null_state()
            , engaged(false)
        {
            assert(!old.engaged);
        }

        ~lazy_holder() {
            if (engaged)
            {
                value.~T();
            }
        }

        template <typename Resolver, typename ...Xs>
        void operator()(Resolver& resolver, Xs&& ...xs) noexcept {
          new(std::addressof(value)) T(this->fn(resolver, std::forward<Xs>(xs)...));
          engaged = true;
        }

        private:

        union {
            char null_state;
            T value;
        };

        bool engaged;
    };

    constexpr auto has_return_type = hana::is_valid([](auto const& fn)
        -> ct::return_type_t<decltype(fn)>
    { });

    // make_lazy_promise_storage
    //      - returns Promise impl that stores
    //        intermediate promise returned by user
    //        fn if the return type is not dependent
    //        (used in chain_impl)
    template <typename Fn>
    constexpr auto make_lazy_promise_storage(Fn&& fn) {
        if constexpr(hana::is_a<promise_tag, Fn>) {
            return std::forward<Fn>(fn).impl;
        }
        else if constexpr(has_return_type(fn)) {
            return lazy_async<std::decay_t<Fn>>{std::forward<Fn>(fn)};
        }
        else {
            auto holder = make_holder(std::forward<Fn>(fn));
            auto new_fn = [holder{std::move(holder)}](auto& resolve, auto&& input) {
                auto intermediate_promise = holder.value(std::forward<decltype(input)>(input));
                intermediate_promise(resolve, std::forward<decltype(input)>(input));
                // THE LIFETIME OF `intermediate_promise` MUST BE MANAGED BY THE USER
            };

            return async_raw_handler<decltype(new_fn)>{std::move(new_fn)};
        }
    }
}

#endif
