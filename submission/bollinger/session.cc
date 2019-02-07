#include "session.h"

// Start the asynchronous operation
void session::run(){
    // Accept the websocket handshake
    ws_.async_accept(
        net::bind_executor(
            strand_,
            std::bind(
                &session::on_accept,
                shared_from_this(),
                std::placeholders::_1)));
}

void session::on_accept(beast::error_code ec){
    if(ec)
        return fail(ec, "accept");

    // Read a message
    do_read();
}

void session::do_read(){
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

void session::on_read(beast::error_code ec, std::size_t bytes_transferred){
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

    // This indicates that the session was closed
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
    
    if(json_string["command"].get<std::string>() == "STEP"){
 
        boost::beast::ostream(buffer_) <<  alg.algo_bollinger(std::stof(json_string["value"].get<std::string>(), 0)).dump();   

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

    // .. our close handler
    if(json_string["command"].get<std::string>() == "CLOSE"){
        std::cout << "Closing Socket" << std::endl;
        ws_.close(beast::websocket::close_code::normal, ec);
    }

}

void session::on_write(beast::error_code ec, std::size_t bytes_transferred){
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}