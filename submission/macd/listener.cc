#include "listener.h"

listener::listener( net::io_context& ioc, tcp::endpoint endpoint): acceptor_(ioc), socket_(ioc){
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec){
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if(ec){
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if(ec){
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if(ec){
        fail(ec, "listen");
        return;
    }
}

 // Start accepting incoming connections
void listener::run(){
    if(! acceptor_.is_open())
        return;
    do_accept();
}

void listener::do_accept(){
    acceptor_.async_accept( 
        socket_, 
        std::bind(
            &listener::on_accept,
            shared_from_this(),
            std::placeholders::_1));
}

void listener::on_accept(beast::error_code ec){
    if(ec){ 
        fail(ec, "accept"); 
    }else{
        // Create the session and run it
        std::make_shared<session>(std::move(socket_))->run();
    }

    // Accept another connection
    do_accept();
}