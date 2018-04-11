


    endpoint_join = [](auto T, auto U) {
        return endpoint_spec(
            event::init             = do_(T.init, U.init),
            event::read_message     = do_(T.read_message, U.read_message),
            event::write_message    = do_(U.write_message, T.write_message)
        );
    };

    auto websocket_client = endpoint_compose(endpoint_class(
        event::init = do_(
            // lower-level stuff happens here
            connect, // this is lower-level (asio)
            handshake_request,
            handshake_response,
            parse_auth_token
            // user stuff happens here
            // then "start_messaging" gets called
        ),
        event::read_message = do_(
            protocol_read_stuff,
            deserialize, // the user would add this
            user_message_handler // via tap maybe
        ),
        event::write_message = do_(
            // could be helper to own and pop from an std::queue
            pop_from_queue,
            websocket_send_stuff
        )
    ));

    auto endpoint = endpoint_open(
        initial_value,
        queue,
        endpoint_compose(asio_tcp_client, websocket_client, my_client),
        unregister
    );

    endpoint.send_message(msg);
    endpoint.stop();
