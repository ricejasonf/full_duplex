//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ENDPOINT_HPP
#define FULL_DUPLEX_ENDPOINT_HPP

#include <full_duplex/event.hpp>
#include <full_duplex/fwd/endpoint.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/do.hpp>

#include <boost/hana/at_key.hpp>
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/unpack.hpp>
#include <utility>

namespace full_duplex {
    namespace hana = boost::hana;

    namespace detail {
        constexpr auto get_endpoint_event = [](auto key, auto& map, auto& input) {
            auto e = hana::find(map, key).value_or(do_());
            if constexpr(hana::is_a<promise_tag, decltype(e)>) {
                return e;
            } else {
                // inject fake "self" param
                // should return a promise
                return e(input);
            }
        };
    }

    template <typename Storage>
    struct endpoint_t<Storage> {
        using hana_tag = endpoint_tag;

        template <typename Input>
        constexpr auto init(Input& input) {
            return hana::unpack(storage, [&](auto& ...map) {
                return do_(detail::get_endpoint_event(event::init,
                                                      map, input)...);
            });
        }

        template <typename Input>
        constexpr auto read_message(Input& input) {
            return hana::unpack(storage, [&](auto& ...map) {
                return do_(detail::get_endpoint_event(event::read_message,
                                                      map, input)...);
            });
        }

        template <typename Input>
        constexpr auto write_message(Input& input) {
            return hana::unpack(hana::reverse(storage), [&](auto ...map) {
                return do_(detail::get_endpoint_event(event::write_message,
                                                      map, input)...);
            });
        }

        template <typename Input>
        constexpr auto error(Input& input) {
            return hana::unpack(storage, [&](auto& ...map) {
                return do_(detail::get_endpoint_event(event::error, map,
                                                      input)...);
            });
        }

        template <typename Input>
        constexpr auto handle_terminate(Input& input) {
            return hana::unpack(storage, [&](auto& ...map) {
                return do_(detail::get_endpoint_event(event::terminate,
                                                      map, input)...);
            });
        }

        Storage storage;
    };

    template <typename ...Events>
    constexpr auto endpoint_fn::operator()(Events&& ...events) const {
        using Storage = decltype(hana::make_basic_tuple(
            hana::make_map(std::forward<Events>(events)...))
        );
        return endpoint_t<Storage>{hana::make_basic_tuple(
            hana::make_map(std::forward<Events>(events)...)
        )};
    }

    template <typename ...Xs>
    constexpr auto endpoint_compose_fn::operator()(Xs&& ...xs) const {
        using Storage = decltype(
            hana::flatten(hana::make_basic_tuple(std::forward<Xs>(xs).storage...))
        );
        return endpoint_t<Storage>{
            hana::flatten(hana::make_basic_tuple(std::forward<Xs>(xs).storage...))
        };
    }
}

#endif
