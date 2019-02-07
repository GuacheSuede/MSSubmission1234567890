//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: WebSocket server, asynchronous
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <nlohmann/json.hpp>


#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>

#include <bsoncxx/types.hpp>



using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using json = nlohmann::json;


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



mongocxx::uri uri("mongodb://restheart:R3ste4rt!@localhost:27017");
mongocxx::database db;
mongocxx::collection prices_collection;

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session>
{
    websocket::stream<tcp::socket> ws_;
    net::strand<
    net::io_context::executor_type> strand_;
    beast::multi_buffer buffer_;

public:
    // Take ownership of the socket
    explicit
    session(tcp::socket socket)
            : ws_(std::move(socket))
            , strand_(ws_.get_executor())
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        // Accept the websocket handshake
        ws_.async_accept(
                net::bind_executor(
                        strand_,
                        std::bind(
                                &session::on_accept,
                                shared_from_this(),
                                std::placeholders::_1)));
    }

    void
    on_accept(beast::error_code ec)
    {
        if(ec)
            return fail(ec, "accept");

        // Read a message
        do_read();
    }

    void
    do_read()
    {
        // Read a message into our buffer
        ws_.async_read(
                buffer_,
                net::bind_executor(
                        strand_,
                        std::bind(
                                &session::on_read,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2)));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred){
        boost::ignore_unused(bytes_transferred);

        json json_string;
        std::string request_string =  beast::buffers_to_string(buffer_.data());
        if (!request_string.empty()){
            json_string = json::parse(request_string);
        }else{
            boost::beast::ostream(buffer_) <<  "Invalid";

            ws_.async_write(
                    buffer_.data(),
                    net::bind_executor(
                            strand_,
                            std::bind(
                                    &session::on_write,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2)));

            return;
        }

        if(ec == websocket::error::closed) {
            std::cout << "Closed" << std::endl;
            return;
        }
        if(ec) {fail(ec, "read");}

        // Clear Buffer after read
        buffer_.consume(buffer_.size());

        // .. our open/start handler
        if(json_string["command"].get<std::string>() == "START"){
            boost::beast::ostream(buffer_) <<  "Starting Socket";
            ws_.async_write(
                    buffer_.data(),
                    net::bind_executor(
                            strand_,
                            std::bind(
                                    &session::on_write,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2)));
        }

        // Echo the message
        ws_.text(ws_.got_text());
        ws_.async_write(
                buffer_.data(),
                net::bind_executor(
                        strand_,
                        std::bind(
                                &session::on_write,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2)));
    }


















    void
    on_write(
            beast::error_code ec,
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        // Clear the buffer
        buffer_.consume(buffer_.size());

        // Do another read
        do_read();
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    tcp::acceptor acceptor_;
    tcp::socket socket_;

public:
    listener(
            net::io_context& ioc,
            tcp::endpoint endpoint)
            : acceptor_(ioc)
            , socket_(ioc)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
                net::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        if(! acceptor_.is_open())
            return;
        do_accept();
    }

    void
    do_accept()
    {
        acceptor_.async_accept(
                socket_,
                std::bind(
                        &listener::on_accept,
                        shared_from_this(),
                        std::placeholders::_1));
    }

    void
    on_accept(beast::error_code ec)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(std::move(socket_))->run();
        }

        // Accept another connection
        do_accept();
    }
};

//------------------------------------------------------------------------------

int main()
{
    int page = 0;

    mongocxx::client client(uri);
    db = client["db"];
    prices_collection = db["prices"];

    auto order = bsoncxx::builder::stream::document{} << "$orderby" << -1 << bsoncxx::builder::stream::finalize;

    auto opts = mongocxx::options::find{};
    opts.sort(order.view());
    opts.limit(1);
    opts.skip(page * 1);

    auto cursor = prices_collection.find({}, opts);

//        std::cout << bsoncxx::to_json(rrr.view()) << std::endl;

////    auto cursor = prices_collection.find(
////            make_document(kvp("$orderby", make_document(kvp("datetime", 1))))
////
//    );
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

//
//    auto const address = net::ip::make_address("0.0.0.0");
//    auto const port = static_cast<unsigned short>(8080);
//    auto const threads = std::max<int>(1, 1);
//
//    // The io_context is required for all I/O
//    net::io_context ioc{threads};
//
//    // Create and launch a listening port
//    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();
//
//    // Run the I/O service on the requested number of threads
//    std::vector<std::thread> v;
//    v.reserve(threads - 1);
//    for(auto i = threads - 1; i > 0; --i)
//        v.emplace_back(
//                [&ioc]
//                {
//                    ioc.run();
//                });
//    ioc.run();

    return EXIT_SUCCESS;
}