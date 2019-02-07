#ifndef includes_hub
#define includes_hub

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <r3/r3.hpp>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using http_response =  http::response<http::string_body>;
using http_response_none =  http::response<http::empty_body>;

using json = nlohmann::json;

// Report a failure
void fail(beast::error_code ec, char const* what){
    std::cerr << what << ": " << ec.message() << "\n";
}

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}


void new_watchlist(std::string body);

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.`
template< class Body, class Allocator, class Send>
void handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send){
    if (req.method() == http::verb::options && req.target() == "/") {

        http_response_none res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::access_control_allow_origin, "*");
        res.set(http::field::access_control_allow_headers, "*");
        res.set(http::field::access_control_allow_methods, "*");
        res.set(http::field::allow, "OPTIONS, GET, HEAD, POST");
        res.prepare_payload();
        return send(std::move(res));
    }
    if (req.method() == http::verb::post && req.target() == "/"){
        // already string due to string body in sesion.h
            new_watchlist(req.body());

            http_response res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.set(http::field::access_control_allow_origin, "*");


            res.body() = "{\"status\": \"DONE\"}";

            res.prepare_payload();
            return send(std::move(res));
        }
    }

void new_watchlist(std::string body){  // verify ssl is quick fix

    std::cout << body << std::endl;
    std::string restheart_url = "http://localhost:8080/db/watchlist";
    auto r = cpr::Post(
                cpr::Url{restheart_url},
                cpr::VerifySsl(false),
                cpr::Authentication{"admin", "changeit"},
                cpr::Body{body},
                cpr::Header{{"Content-Type", "application/json"}}
            );
    std::cout << r.status_code << std::endl;



}
//9620 0092

#endif
