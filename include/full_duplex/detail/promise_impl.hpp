//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP
#define FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP

#include <boost/hana/core/is_a.hpp>
#include <utility>

namespace full_duplex::detail {
    namespace hana = boost::hana;

    //
    // async -  A promise operation with overloads for
    //          propagating errors

    struct async_tag { };

    template <typename Fn>
    struct async_t {
        using hana_tag = async_tag;

        Fn fn;
    };

    template <typename AsyncFn>
    struct async_handler {
        AsyncFn f;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const
        {
            if constexpr(hana::is_a<terminate, Input>) {
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                std::forward<ResolveFn>(resolve)(std::forward<Input>(input));
            }
            else {
                f(std::forward<ResolveFn>(resolve), std::forward<Input>(input));
            }
        }
    };

    template <typename AsyncFn>
    struct async_error_handler {
        AsyncFn f;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                f(std::forward<ResolveFn>(resolve), std::forward<Input>(input));
            }
            else {
                std::forward<ResolveFn>(resolve)(std::forward<Input>(input));
            }
        }
    };

    //
    // pmap - A partially applied mapping function
    //

    struct pmap_tag { };

    struct pmap_t {
        using hana_tag = pmap_tag;

        Fn fn;
    };

    template <typename F>
    struct pmap_handler {
        F f;

        template <typename Input>
        constexpr auto operator()(Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                return terminate{};
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                return std::forward<Input>(input);
            }
            else {
                return f(std::forward<Input>(input));
            }
        }
    };

    template <typename F>
    struct pmap_error_handler {
        F f;

        template <typename Input>
        constexpr auto operator()(Input&& input) const {
            if constexpr(hana::is_a<terminate, Input>) {
                return terminate{};
            }
            else if constexpr(hana::is_a<error_tag, Input>) {
                return f(std::forward<Input>(input));
            }
            else {
                return std::forward<Input>(input);
            }
        }
    };

    template <typename F>
    struct catch_error_handler {
        F f;

        template <typename ResolveFn, typename Input>
        constexpr void operator()(ResolveFn&& resolve, Input&& input) const {
            // uses libc++ impl details FIXME
            if constexpr(std::__invokable<F, Input>::value) {
                f(std::forward<Input>(input));
                std::forward<ResolveFn>(resolve)(terminate{});
            }
            else {
                return std::forward<Input>(input);
            }
        }
    };
}
