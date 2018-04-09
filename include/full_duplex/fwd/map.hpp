//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_MAP_HPP
#define FULL_DUPLEX_FWD_MAP_HPP

namespace full_duplex {
    // map - maps values excluding error<T> and terminate

    struct map_fn {
        template <typename Fn>
        constexpr auto operator()(Fn&& fn) const;
    };

    constexpr map_fn map{};

    // tap - taps values excluding error<T> and terminate

    struct tap_fn {
        template <typename Fn>
        constexpr auto operator()(Fn&& fn) const;
    };

    constexpr tap_fn tap{};

    // map_error - maps T in error<T> or propagates unchanged
    //             same as `adjust_if(F, is<error_tag>)`

    struct map_error_fn {
        template <typename Fn>
        constexpr auto operator()(Fn&& fn) const;
    };

    constexpr map_error_fn map_error{};

    // map_either - maps either a valid T or T in error<T> using the provided mapping functions
    //              (not implemented)

    struct map_either_fn {
        template <typename Fn, typename ErrorFn>
        constexpr auto operator()(Fn&& fn, ErrorFn error_fn) const;
    };

    // catch_error - Catches an error<T> where F(T) is invokable then 
    //               returns terminate.
    //               Otherwise it passes through unchanged.

    struct catch_error_fn {
        template <typename Fn>
        constexpr auto operator()(Fn&& fn) const;
    };

    constexpr catch_error_fn catch_error{};

    // map_any - maps any value including error<T> and terminate

    struct map_any_fn {
        template <typename Fn>
        constexpr auto operator()(Fn&& fn) const;
    };

    constexpr map_any_fn map_any{};

}

#endif
