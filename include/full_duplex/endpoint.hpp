//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ENDPOINT_HPP
#define FULL_DUPLEX_ENDPOINT_HPP

#include <full_duplex/fwd/endpoint.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/do.hpp>

#include <boost/hana/at_key.hpp>
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/sum.hpp>
#include <boost/hana/zero.hpp>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;

    template <typename Storage>
    struct endpoint_t<Storage> {
        using hana_tag = endpoint_tag;
        Storage storage;
    };

    template <typename ...Events>
    constexpr auto endpoint_fn::operator()(Events&& ...events) const {
        using Storage = decltype(hana::make_map(std::forward<Events>(events)...));
        return endpoint_t<Storage>{hana::make_map(std::forward<Events>(events)...)};
    }

    template <typename ...Xs>
    constexpr auto endpoint_compose_fn::operator()(Xs&& ...xs) const {
        return hana::sum<endpoint_tag>(hana::make_basic_tuple(std::forward<Xs>(xs)...));
    }

    template <typename T>
    template <typename P>
    constexpr auto event_t<T>::operator=(P&& p) const {
        return hana::make_pair(event_t<T>{}, std::forward<P>(p));
    }
}

namespace boost::hana {
    //
    // event
    //

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

    //
    // endpoint
    //

    // Monoid (non-commutative)

    template <>
    struct plus_impl<full_duplex::endpoint_tag, full_duplex::endpoint_tag> {
        template <typename T, typename U>
        static constexpr auto apply(T&& t, U&& u) {
            using full_duplex::do_;
            using full_duplex::endpoint;
            namespace event = full_duplex::event;

            return endpoint(
                event::init = do_(
                    std::forward<T>(t).storage[event::init],
                    std::forward<U>(u).storage[event::init]
                ),
                event::read_message = do_(
                    std::forward<T>(t).storage[event::read_message],
                    std::forward<U>(u).storage[event::read_message]
                ),
                event::write_message = do_(
                    std::forward<U>(u).storage[event::write_message],
                    std::forward<T>(t).storage[event::write_message]
                )
            );
        }
    };

    template <>
    struct zero_impl<full_duplex::endpoint_tag> {
        static constexpr auto apply() {
            using full_duplex::do_;
            using full_duplex::endpoint;
            namespace event = full_duplex::event;

            return endpoint(
                event::init = do_(),
                event::read_message = do_(),
                event::write_message = do_()
            );
        }
    };
}

#endif
