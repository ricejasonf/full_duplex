//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_ENDPOINT_INST_HPP
#define FULL_DUPLEX_DETAIL_ENDPOINT_INST_HPP

#include <full_duplex/async_queue.hpp>
#include <full_duplex/promise.hpp>
#include <full_duplex/run_async.hpp>

#include <memory>
#include <utility>

namespace full_duplex::detail {
    template <typename State, typename AsyncSendQueue, typename Endpoint>
    struct endpoint_inst : std::enable_shared_from_this<endpoint_inst<State, AsyncSendQueue, Endpoint>> {
        template <typename State_, typename Q, typename E>
        endpoint_inst(State_&& state_, Q&& q, E&& e)
            : send_queue(std::forward<Q>(q))
            , endpoint(std::forward<E>(e))
            , state_(std::forward<State_>(state_))
            , is_started(false)
            , is_stopped(false)
            , is_send_queue_running(false)
        { }

        endpoint_inst(endpoint_inst const&) = delete;

        void _init() {
            // TODO The running promises could be stored
            // in this struct since its lifetime is tied to it.
            // This would prevent unneccessary allocations.
            run_async(
                endpoint.init(*this),
                tap([this](auto&&) {
                    is_started = true;
                    flush_send_queue();
                    keep_reading();
                }),
                error_catcher()
            );
        }

        template <typename Message>
        void send_message(Message&& m) {
            run_async(
                promise_lift(std::forward<Message>(m)),
                send_queue.push(),
                tap([this](void_input_t) { flush_send_queue(); }),
                error_catcher()
            );
        }

        void flush_send_queue() {
            if (    is_send_queue_running
                 or is_stopped
             or not is_started)
            { return; }

            is_send_queue_running = true;

            auto terminate_if_done = do_(
                send_queue.size(),
                promise([this](auto& resolve, std::size_t size) {
                    if (size > 0) {
                        resolve(void_input);
                    }
                    else {
                        is_send_queue_running = false;
                        resolve(terminate{});
                    }
                })
            );

            run_async_loop(
                terminate_if_done,
                terminate_if_stopped(),
                send_queue.front(),
                terminate_if_stopped(),
                endpoint.write_message(*this),
                send_queue.pop(),
                error_catcher()
            );
        }

        auto& state() { return std::ref(state_).get(); }

    private:
        AsyncSendQueue send_queue;
        Endpoint endpoint;
        State state_;
        bool is_started             : 1;
        bool is_stopped             : 1;
        bool is_send_queue_running  : 1;

        void keep_reading() {
            run_async_loop(
                terminate_if_stopped(),
                endpoint.read_message(*this),
                error_catcher()
            );
        }

        // every promise with error_catcher() owns a "shared this"
        auto error_catcher() {
            return catch_error([this, _{this->shared_from_this()}](auto&& error) {
                if (is_stopped) return;
                is_stopped = true;
                run_async(
                    promise_lift(this->shared_from_this()),
                    promise_lift(std::forward<decltype(error)>(error)),
                    endpoint.error(*this)
                );
            });
        }

        auto terminate_if_stopped() {
            return promise([this](auto& resolve, auto&& input) {
                static_assert(not hana::is_a<terminate, decltype(input)>());
                if (is_stopped) { resolve(terminate{}); }
                else            { resolve(void_input); }
            });
        }
    };
}

#endif
