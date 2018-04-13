//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_ENDPOINT_OPEN_HPP
#define FULL_DUPLEX_ENDPOINT_OPEN_HPP

#include <full_duplex/detail/endpoint_inst.hpp>
#include <full_duplex/fwd/endpoint.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/do.hpp>

#include <utility>

namespace full_duplex {
    template <typename Input, typename SendQueue, typename Endpoint>
    constexpr auto endpoint_open::operator()(Input&& input,
                                             SendQueue&& q,
                                             Endpoint&& e) const
    {
        return endpoint_inst<std::decay_t<SendQueue>, std::decay_t<Endpoint>>{
            std::forward<Input>(input),
            std::forward<SendQueue>(q),
            std::forward<Endpoint>(e)
        };
    }
}

#endif
