//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_HOLDER_HPP
#define FULL_DUPLEX_DETAIL_HOLDER_HPP

#include <full_duplex/detail/promise_impl.hpp>

#include <boost/callable_traits/return_type.hpp>
#include <boost/hana/type.hpp>
#include <memory>
#include <utility>

namespace full_duplex::detail {
    namespace ct    = boost::callable_traits;
    namespace hana  = boost::hana;

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
        using Holder = holder<std::decay_t<decltype(fn)>>;
        return Holder{std::forward<decltype(fn)>(fn)};
    };

    //
    // lazy_holder
    //

    template <typename T>
    struct lazy_holder
    {
        lazy_holder()
            : engaged(false)
            , null_state()
        { }

        lazy_holder(lazy_holder const& old)
            : engaged(old.engaged)
            , null_state()
        {
            if (old.engaged) {
                new(std::addressof(value)) T(old.value);
            }
        }

        lazy_holder(lazy_holder&& old)
            : engaged(old.engaged)
            , null_state()
        {
            if (old.engaged) {
                new(std::addressof(value)) T(std::move(old.value));
            }
        }

        ~lazy_holder() {
            if (engaged) {
                value.~T();
            }
        }

        template <typename X>
        void assign(X&& x) {
            new(std::addressof(value)) T(std::forward<X>(x));
            engaged = true;
        }

        bool is_engaged()
        { return engaged; }

        T const& force_get()
        { return value; }

    protected:

        bool engaged;
        union {
            char null_state;
            T value;
        };
    };

    template <typename Fn>
    struct lazy_holder_function
    {
        Fn fn;
    };

    //
    // lazy_holder_async_fn
    //

    template <typename T, typename Fn>
    struct lazy_holder_async_fn
        : lazy_holder_function<Fn>
        , lazy_holder<T>
    {
        template <typename FnArg>
        lazy_holder_async_fn(FnArg&& fn_arg)
            : lazy_holder_function<Fn>(std::forward<FnArg>(fn_arg))
            , lazy_holder<T>()
        { }

        template <typename ResolveFn, typename Input>
        void operator()(ResolveFn& resolve, Input&& input) noexcept {
            new(std::addressof(this->value)) T(this->fn(resolve, std::forward<Input>(input)));
            this->engaged = true;
        }
    };

    constexpr auto make_lazy_holder_async = [](auto&& fn) {
        using T = ct::return_type_t<decltype(fn)>;
        using Fn = decltype(fn);
        return lazy_holder_async_fn<T, std::decay_t<Fn>>(std::forward<Fn>(fn));
    };

    // make_lazy_promise_storage
    //      - returns Promise impl that stores
    //        intermediate promise returned by user
    //        fn if the return type is not dependent
    //        (used in chain_impl)
    constexpr auto has_return_type = hana::is_valid([](auto const& fn)
        -> ct::return_type_t<decltype(fn)>
    { });

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
