//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP
#define FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP

#include <full_duplex/fwd/promise.hpp>
#include <full_duplex/error.hpp>

#include <boost/hana/core/is_a.hpp>
#include <utility>

namespace full_duplex::detail {
    namespace hana = boost::hana;

    //
    // async -  A promise operation with overloads for
    //          propagating errors

    struct async_tag { };

    template <typename AsyncFn>
    struct async_raw_handler {
        using hana_tag = async_tag;
        AsyncFn fn;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            fn(std::forward<ResolveFn>(resolve), std::forward<Input>(input));
        }
    };

    template <typename AsyncFn>
    struct async_handler {
        using hana_tag = async_tag;
        AsyncFn fn;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                std::forward<ResolveFn>(resolve)(std::forward<Input>(input));
            }
            else {
                fn(std::forward<ResolveFn>(resolve), std::forward<Input>(input));
            }
        }
    };

    template <typename AsyncFn>
    struct async_error_handler {
        using hana_tag = async_tag;
        AsyncFn fn;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                fn(std::forward<ResolveFn>(resolve), std::forward<Input>(input));
            }
            else {
                std::forward<ResolveFn>(resolve)(std::forward<Input>(input));
            }
        }
    };

    //
    // lazy_async - used to lazily construct a lazy_holder since it is not copyable
    //

    struct lazy_async_tag { };

    template <typename Fn>
    struct lazy_async {
        using hana_tag = lazy_async_tag;
        Fn fn;
    };

    //
    // pmap - A partially applied mapping function
    //

    struct pmap_tag { };

    template <typename Fn>
    struct pmap_raw_handler {
        using hana_tag = pmap_tag;

        Fn fn;

        template <typename Input>
        constexpr auto operator()(Input&& input) const {
            return fn(std::forward<Input>(input));
        }
    };

    template <typename Fn>
    struct pmap_handler {
        using hana_tag = pmap_tag;
        Fn fn;

        template <typename Input>
        constexpr auto operator()(Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                return terminate{};
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                return std::forward<Input>(input);
            }
            else {
                return fn(std::forward<Input>(input));
            }
        }
    };

    template <typename Fn>
    struct pmap_error_handler {
        using hana_tag = pmap_tag;
        Fn fn;

        template <typename Input>
        constexpr auto operator()(Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                return terminate{};
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                return fn(std::forward<Input>(input));
            }
            else {
                return std::forward<Input>(input);
            }
        }
    };

    template <typename Fn>
    struct catch_error_handler {
        using hana_tag = pmap_tag;
        Fn fn;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            // uses libc++ impl details FIXME
            if constexpr(std::__invokable<Fn, Input>::value) {
                fn(std::forward<Input>(input));
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else {
                return std::forward<Input>(input);
            }
        }
    };
}

#endif
