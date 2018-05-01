//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ENDPOINT_OPEN_HPP
#define FULL_DUPLEX_ENDPOINT_OPEN_HPP

#include <full_duplex/detail/endpoint_inst.hpp>
#include <full_duplex/fwd/endpoint_open.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/do.hpp>

#include <utility>

namespace full_duplex {
    template <typename State, typename SendQueue, typename Endpoint>
    constexpr auto endpoint_open_fn::operator()(State&& state,
                                                SendQueue&& q,
                                                Endpoint&& e) const
    {
        using AsyncSendQueue = async_queue<std::decay_t<SendQueue>>;

        auto inst = std::make_shared<detail::endpoint_inst<std::decay_t<State>,
                                                           AsyncSendQueue,
                                                           std::decay_t<Endpoint>>>(
            std::forward<State>(state),
            std::forward<SendQueue>(q),
            std::forward<Endpoint>(e)
        );

        inst->_init();
        return inst;
    }
}

#endif
