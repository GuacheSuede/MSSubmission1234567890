#pragma once
#include "util.h"
#include "session.h"

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>{
    tcp::acceptor acceptor_;
    tcp::socket socket_;

public:
    listener( net::io_context& ioc, tcp::endpoint endpoint);

    void run();
    void do_accept();
    void on_accept(beast::error_code ec);
};