//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_ENDPOINT_INST_HPP
#define FULL_DUPLEX_DETAIL_ENDPOINT_INST_HPP

#include <full_duplex/promise.hpp>
#include <full_duplex/run_async.hpp>

#include <utility>

namespace full_duplex::detail {
    template <typename SendQueue, typename Endpoint>
    struct endpoint_inst {
        template <typename Input, typename Q, typename E>
        endpoint_inst(Input&& input, Q&& q, E&& e)
            : send_queue(std::forward<Q>(q))
            , endpoint(std::forward<E>(e))
        {
            // Perhaps the running promise could be stored
            // in this struct since its lifetime is tied to it
            nbdl::run_async(
                promise_lift(std::forward<Input>(input)),
                endpoint.init(),
                tap([this](auto&&) {
                    is_started = true;
                    start_send_queue();
                    keep_reading();
                }),
                error_catcher()
            );
        }

        endpoint_inst(endpoint_inst const&) = delete;

        template <typename Message>
        void send_message(Message&& m) {
            run_async(
                endpoint.send_queue_push(),
                tap([this, shared_this](auto&&) { flush_send_queue(); }),
                error_catcher()
            );
            
        }

        void flush_send_queue() {
            if (    is_send_queue_running
                 or send_queue.size() == 0
                 or is_stopped
             or not is_started)
            { return; }

            is_send_queue_running = true;

            run_async_loop(
                map([this](auto&&) -> decltype(auto) { return (send_queue); }),
                endpoint.send_queue_front(),
                endpoint.write_message(),
                map([this](auto&&) -> decltype(auto) { return (send_queue); }),
                endpoint.send_queue_pop(),
                promise([&send_queue](auto& resolve, auto&&) {
                    if (send_queue.size() > 0) {
                        resolve(void_input);
                    }
                    else {
                        is_send_queue_running = false;
                        resolve(terminate{});
                    }
                }),
                error_catcher()
            );
        }

    private:
        SendQueue send_queue;
        Endpoint endpoint;
        bool is_started             : 1 = false;
        bool is_stopped             : 1 = false;
        bool is_send_queue_running  : 1 = false;

        void keep_reading() {
            run_async_loop(
                endpoint.read_message(),
                error_catcher()
            );
        }

        auto error_catcher() {
            return catch_error([this, shared_this](auto&& error) {
                endpoint.error(std::forward<decltype(error)>(error));
            });
        }
    };
}

#endif
