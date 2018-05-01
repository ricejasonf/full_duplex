//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_SEND_MESSAGE_HPP
#define FULL_DUPLEX_FWD_SEND_MESSAGE_HPP

#include <full_duplex/detail/endpoint_inst.hpp>

#include <memory>

namespace full_duplex {
    struct send_message_fn {
        template <typename Endpoint, typename Message>
        void operator()(std::shared_ptr<Endpoint>& e, Message&& m) const {
            e->send_message(std::forward<Message>(m));
        }

        template <typename ...E, typename Message>
        void operator()(detail::endpoint_inst<E...>& e, Message&& m) const {
            e.send_message(std::forward<Message>(m));
        }
    };

    constexpr send_message_fn send_message{};
}

#endif
