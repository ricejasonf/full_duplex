//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_HOLDER_HPP
#define FULL_DUPLEX_DETAIL_HOLDER_HPP

#include <full_duplex/detail/promise_impl.hpp>
#include <full_duplex/error.hpp>

#include <boost/callable_traits/return_type.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/type.hpp>
#include <cstdlib>
#include <functional>
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
    // erased_holder
    //

    struct erased_holder {
        using destroyer_fn = void(*)(void*);

        void* data_;
        void(*destroy)(void*);

        constexpr erased_holder()
            : data_(nullptr)
            , destroy(noop())
        { }

        constexpr erased_holder(void* data_, void(*destroy)(void*))
            : data_(data_)
            , destroy(destroy)
        { }

        constexpr erased_holder(erased_holder&& old)
            : data_(old.data_)
            , destroy(old.destroy)
        {
            old.invalidate();
        }

        constexpr erased_holder& operator=(erased_holder&& old) {
            data_ = old.data_;
            destroy = old.destroy;
            old.invalidate();
            return *this;
        }

        ~erased_holder() {
            destroy(data_);
        }

    private:
        constexpr void invalidate() {
            destroy = noop();
        }

        constexpr void(*noop())(void*) { return [](void*) { }; }
    };

    template <typename T>
    erased_holder make_erased_holder(void* data_) {
        return erased_holder{
            data_, 
            [](void* data_) {
                reinterpret_cast<T*>(data_)->~T();
                std::free(data_);
            }
        };
    }

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

        template <typename FnArg>
        lazy_holder_function(FnArg&& fn_arg)
            : fn(std::forward<FnArg>(fn_arg))
        { }
    };

    //
    // lazy_holder_async_fn
    //

    template <typename PromiseJoinFn, typename Fn>
    struct lazy_holder_async_fn
        : lazy_holder_function<Fn>
    {
        erased_holder holder_;

        explicit lazy_holder_async_fn(Fn&& fn)
            : lazy_holder_function<Fn>(std::move(fn))
            , holder_()
        { }

        template <typename ResolveFn, typename Input>
        void operator()(ResolveFn& resolve, Input&& input) noexcept {
            using Joined = decltype(PromiseJoinFn{}(
                this->fn(std::forward<Input>(input)),
                std::ref(resolve)
            ));

            void* ptr = std::malloc(sizeof(Joined));

            if (ptr == nullptr) {
                throw std::runtime_error("malloc failed");
                // Could possibly handle this but it complicates the Laws tests
                //resolve(make_error(std::runtime_error("malloc failed")));
            }
            else {
                new(ptr) Joined(PromiseJoinFn{}(this->fn(input), std::ref(resolve)));
                holder_ = make_erased_holder<Joined>(ptr);
                reinterpret_cast<Joined*>(ptr)->operator()(std::forward<Input>(input));
            }
        }
    };

    template <typename PromiseJoinFn>
    constexpr auto make_lazy_holder_async = [](auto&& fn) {
        using Fn = decltype(fn); // function that returns a promise

        return lazy_holder_async_fn<PromiseJoinFn, std::decay_t<Fn>>(std::forward<Fn>(fn));
    };

    // make_lazy_promise_impl
    //      - returns Promise impl that stores the
    //        dependent promise using type erasure
    //        unless the fn is a promise itself
    //        (used in chain_impl)
    template <typename Fn>
    constexpr auto make_lazy_promise_storage(Fn&& fn) {
        if constexpr(hana::is_a<promise_tag, Fn>) {
            return std::forward<Fn>(fn).impl;
        }
        else {
            return lazy_async<std::decay_t<Fn>>{std::forward<Fn>(fn)};
        }
    }
}

#endif
