//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_EVENT_HPP
#define FULL_DUPLEX_EVENT_HPP

#include <full_duplex/fwd/event.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/hash.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;

    template <typename T>
    template <typename P>
    constexpr auto event_t<T>::operator=(P&& p) const {
        return hana::make_pair(event_t<T>{}, std::forward<P>(p));
    }
}

namespace boost::hana {
    // Comparable

    template <>
    struct equal_impl<full_duplex::event_tag, full_duplex::event_tag> {
        template <typename T, typename U>
        static constexpr auto apply(full_duplex::event_t<T>, full_duplex::event_t<U>)
            -> hana::bool_<std::is_same_v<T, U>>
        { return {}; }

        template <typename T, typename U>
        static constexpr auto apply(T, T)
            -> hana::true_
        { return {}; }
    };

    // Hashable

    template <>
    struct hash_impl<full_duplex::event_tag> {
        template <typename T>
        static constexpr auto apply(T)
            -> hana::type<T>
        { return {}; }
    };
}

#endif
