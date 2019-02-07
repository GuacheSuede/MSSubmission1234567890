#include "util.h"

#include "listener.h"
#include "session.h"

int main(){
    auto const address = net::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(std::atoi("8080"));
    auto const threads = std::max<int>(1, std::atoi("1"));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i){
        v.emplace_back([&ioc]{
            ioc.run();
        });
    }
    ioc.run();

    return EXIT_SUCCESS;
}
