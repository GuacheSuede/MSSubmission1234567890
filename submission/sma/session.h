#pragma once
#include "util.h"
#include "algo.h"


// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session>{

websocket::stream<tcp::socket> ws_;
net::strand<net::io_context::executor_type> strand_;
beast::multi_buffer buffer_;
Algo alg;


public:

// Take ownership of the socket
explicit session(tcp::socket socket) : ws_(std::move(socket)) , strand_(ws_.get_executor()){}
void run();
void on_accept(beast::error_code ec);
void do_read();
void on_read(beast::error_code ec, std::size_t bytes_transferred);
void on_write(beast::error_code ec, std::size_t bytes_transferred);
};
