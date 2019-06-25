//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <full_duplex.hpp>

#include <boost/hana/assert.hpp>
#include <string>
#include <vector>

namespace event = full_duplex::event;
using full_duplex::do_;
using full_duplex::endpoint;
using full_duplex::endpoint_compose;
using full_duplex::endpoint_open;
using full_duplex::map;
using full_duplex::send_message;
using full_duplex::tap;
using full_duplex::void_input;

int main() {
    {
        std::vector<std::string> xs{};

        auto e = endpoint_open(
            void_input,
            std::queue<std::string>{},
            endpoint(
                event::read_message = [](auto&) {
                    return map([](auto&&) {
                        return full_duplex::terminate{};
                    });
                },
                event::write_message = [&xs](auto&) {
                    return tap([&xs](auto& message) {
                        xs.push_back(std::move(message));
                    });
                }
            )
        );

        send_message(e, std::string("message1"));
        send_message(e, std::string("message2"));
        send_message(e, std::string("message3"));
        send_message(e, std::string("message4"));
        send_message(e, std::string("message5"));


        BOOST_HANA_RUNTIME_CHECK(
            xs == std::vector<std::string>{
                "message1",
                "message2",
                "message3",
                "message4",
                "message5"
            }
        );
    }
    {
        // `endpoint_compose` should order
        // write_message events in reverse order
        std::vector<std::string> xs{};
        auto read_message = map([](auto&&) {
            return full_duplex::terminate{};
        });

        auto write_message = [](std::string name) {
            return map([=](std::string msg) {
                return msg + " " + name;
            });
        };

        auto ep = endpoint_compose(
            endpoint(
                event::read_message = read_message,
                event::write_message = tap([&xs](auto& message) {
                    xs.push_back(std::move(message));
                })
            ),
            endpoint(
                event::write_message = do_(write_message("baz"),
                                           write_message("boo"))
            ),
            endpoint(
                event::write_message = do_(write_message("foo"),
                                           write_message("bar"))
            )
        );

        auto e = endpoint_open(
            void_input,
            std::queue<std::string>{},
            ep
        );

        send_message(e, std::string("message1"));
        send_message(e, std::string("message2"));
        send_message(e, std::string("message3"));


        BOOST_HANA_RUNTIME_CHECK(
            xs == std::vector<std::string>{
                "message1 foo bar baz boo",
                "message2 foo bar baz boo",
                "message3 foo bar baz boo"
            }
        );
    }
}
