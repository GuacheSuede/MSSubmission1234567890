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
#include <sstream>
#include <regex>
#include <boost/regex.hpp>

using namespace std;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using http_response =  http::response<http::string_body>;
using http_response_none =  http::response<http::empty_body>;

using json = nlohmann::json;

// Report a failure
void fail(beast::error_code ec, char const* what){
    std::cerr << what << ": " << ec.message() << "";
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



string init_code = " ";
string code = "";

string gen_syntax(string code_line);
string init_code_gen();
string loop_gen();
string loop_gen_slow();
string loop_code = "";

vector<string> tickers;
vector<string> fundamentals;
vector<string> algos;
vector<string> algos_t;

void load_script(string input_code){
    tickers.clear();
    fundamentals.clear();
    algos.clear();
    algos_t.clear();
    init_code = "";
    loop_code = "function loopyloop(){";
    code = "";

    init_code += "window.tickers = []; ";
    init_code += "window.price_page = 0; ";
    init_code += "window.portfolio = 10000; ";
    init_code += "window.buys = []; "; // for now buy and sell  a stock only limited
    init_code += "window.sells = []; ";
    init_code += "window.fundamentals = [];";


    istringstream f(input_code);
    string code_script_buf;
    while (getline(f, code_script_buf)){
        loop_code += gen_syntax(code_script_buf) + "";
    }
    loop_code += "};  setInterval(loopyloop, 1800); ";

    code.insert(0, loop_gen_slow());
    code.insert(0, loop_gen());
    code.insert(0, init_code_gen());
    code.insert(0, init_code);
    code.insert(0, "console.log(\"VM ALERT\");");
    code += loop_code;

}


string init_code_gen(){
    string i_code = "function price(ticker, container){   fetch( window.murl + ticker + window.url_end + \"&page=\"+window.price_page, {  method: \'get\',  headers:{  \'Authorization\': \'Basic '+btoa(\'admin:changeit\'),  \'Content-Type\': \'application/json\'  }}).then(response => {return response.json();  }).then(data => {";
        i_code += "container.push(data\[\"_embedded\"\]\[0\]\[\"price\"\]);";
    i_code += "});  } ";

    i_code +=  "function fetch_funda(ticker, quarter, field, cont){  fetch(window.furl + ticker + window.furl2 + quarter + window.furl3 + field + window.url_end, {  method: \'get\',  headers:{  \'Authorization\': \'Basic '+btoa(\'admin:changeit\'),  \'Content-Type\': \'application/json\'  }}).then(response => {return response.json();  }).then(data => {";
        i_code += "window.fundamentals[cont] = data\[\"_embedded\"\]\[0\]\[field\];";
    i_code += "});  } ";


    for(auto &f: fundamentals) {
            vector<string> tokens;
        boost::split(tokens, f, boost::is_any_of("_"));
        i_code += "fetch_funda(\"" + tokens[0] + "\",\"" + tokens[1] + "\",\"" + tokens[2] + "\",\"" + f + "\");";
    }

    for (auto &t: tickers) { // this happens after all tickers have been examined
        i_code += "var " + t + "_line = new window.TimeSeries;";
    }

    for (auto &at: algos_t) { // this happens after all algos have been examined
        i_code += "var " + at + "_line = new window.TimeSeries;";
    }

    i_code += " setInterval(function() { ";
    for (auto &tt: tickers) {

        i_code += tt + "_line.append(new Date().getTime(), window."+ tt +"_values.slice(-1).pop()); ";
    }
    for (auto &att: algos_t) {

        i_code += att + "_line.append(new Date().getTime(), window."+ att +"_values.slice(-1).pop()); ";
    }
    i_code += "}, 1000); ";
    for(auto &ttt: tickers) {
        i_code += "window.smoothie.addTimeSeries(" + ttt + "_line,  { strokeStyle: window.random_rgba(), lineWidth: 8 }); ";
    }

    for(auto &attt: algos_t) {
        i_code += "window.smoothie.addTimeSeries(" + attt + "_line,  { strokeStyle: window.random_rgba(), lineWidth: 8 }); ";
    }

    i_code += "window.smoothie.streamTo(document.getElementById(\"graphviewid\"), 1001);";
    return i_code;
}


string loop_gen(){
    string ret;
    ret += "function loop(){ ++window.price_page;";
    for(auto &ttt: tickers) {
        ret += "price(\"" + ttt + "\"," + "window." + ttt + "_values); ";
    }
//    for (auto &alg: algos_t){
//        vector<string> alg_p;
//        boost::split(alg_p, alg, boost::is_any_of("_"));
////        ret+= "window." + alg + "_socket.send(JSON.stringify({\"command\": \"STEP\", \"value\": window." + alg_p[1] + "_values." + "slice(-1).pop() })); ";
////        ret+= "console.log(window." + alg + "_socket.send(JSON.stringify({\"command\": \"STEP\", \"value\": window." + alg_p[1] + "_values." + "slice(-1).pop() }))) ";
////        ret += "console.log(JSON.stringify({\"command\": \"STEP\", \"value\": window.\" + alg_p[1] + \"_values.slice(-1).pop() }))";
//        ret+= "console.log(window."+alg+"_socket.send(JSON.stringify({\"command\": \"STEP\", \"value\": window." + alg_p[1] + "_values." + "slice(-1).pop() })))    ";
//
//    }
    ret += " }  setInterval(loop, 1000); ";
    return ret;
}



string loop_gen_slow(){
    string ret;
    ret += "function loop2(){ ";

    for (auto &alg: algos_t){
        vector<string> alg_p;
        boost::split(alg_p, alg, boost::is_any_of("_"));
        ret+= "console.log(window."+alg+"_socket.send(JSON.stringify({\"command\": \"STEP\", \"value\": window." + alg_p[1] + "_values." + "slice(-1).pop() })));";

    }
    ret += " }  setInterval(loop2, 1500); ";
    return ret;
}

string get_string(char s){

}

/* 1. Normal Loop Code - normal code
 * 2. Init Loop Code - start code that assigns next value
 * 3. Init code - global start code like websocekts
 */
string gen_syntax(string code_line){
//    /* Comment Declaration
//     * # This will do this thing
//     */
//    if (code_line.find("#") != string::npos){
//        // Do nothing
//    }
//
//    /* Variable Declaration
//     * variable(graft,1)
//     */
//    if (code_line.find("variable") != string::npos){
//        boost::replace_all(code_line, "variable(", "window.");
//        boost::replace_all(code_line, ",", "=");
//        boost::replace_all(code_line, ")", ";");
//    }
//
//    /* Array Init Declaration
//     * array(graft,1)
//     */
//    if (code_line.find("array(") != string::npos){
//        boost::replace_all(code_line, "array(", "window.");
//        boost::replace_first(code_line, ",", "=[\"");
//        boost::replace_all(code_line, ",", "\",\"");
//        boost::replace_all(code_line, ")", "\"];");
//    }
//
//    /* Array Push Declaration
//     * array_push(graft,1)
//     */
//    if (code_line.find("array_push") != string::npos){
//        boost::replace_all(code_line, "array_push(", "");
//        boost::replace_all(code_line, ",", ".push(\"");
//        boost::replace_all(code_line, ")", "\")");
//        code_line.insert(0, "window.");
//    }
//
//    /* Array Pop Declaration
//     * array_pop(graft)
//     */
//    if (code_line.find("array_pop") != string::npos){
//        boost::replace_all(code_line, "array_pop(", "");
//        boost::replace_all(code_line, ")", ".pop();");
//        code_line.insert(0, "window.");
//    }
//
//
//    /* Array Has Declaration
//     * array_has(graft, "1")
//     */
//    if (code_line.find("array_has") != string::npos){
//        boost::replace_all(code_line, "array_has(", "");
//        boost::replace_first(code_line, ",", ".includes(\"");
//        boost::replace_all(code_line, ")", "\")");
//        code_line += ";";
//        code_line.insert(0, "window.");
//    }


// lower case == algo, uppercase == ticker, jsut for the distinction thoguth no real benefit
    boost::regex r_algo_current{"\\b(?:current_)([a-z]*)(?:_)(\\w*)\\b"};
    boost::smatch r_algo_current_m;

    while(boost::regex_search(code_line, r_algo_current_m, r_algo_current)){
        boost::replace_first(code_line, r_algo_current_m[0].str(), "window." + r_algo_current_m[1].str() + "_" + r_algo_current_m[2].str()  + "_values." + "slice(-1).pop();");
        string algoname_unique = r_algo_current_m[1].str() + "_" + r_algo_current_m[2].str();
        string algoname = r_algo_current_m[1].str();

        boost::smatch empty_match;
        r_algo_current_m = empty_match;
        if(std::find(algos.begin(), algos.end(), algoname_unique) == algos.end()) {
            algos.push_back(algoname_unique);
            algos_t.push_back(algoname_unique);

            init_code += "window." + algoname_unique + "_values = [\"0\"]; ";

            string ws_port = "";

            if (algoname == "bollinger"){ ws_port = "30090"; }
            if (algoname == "rsi"){ ws_port = "30091"; }
            if (algoname == "ema"){ ws_port = "30092"; }
            if (algoname == "sma"){ ws_port = "30093"; }
            if (algoname == "macd"){ ws_port = "30094"; }


            init_code += "window." + algoname_unique+ "_socket = new WebSocket(\"ws://127.0.0.1:" + ws_port + "\"); ";
            init_code += "window." + algoname_unique + "_socket.onmessage = function (event) {  \t" +"window."+algoname_unique+ "_values.push(event.data); }; ";
            init_code += "window." + algoname_unique + "_socket.onerror = function (error) {  \t console.log(error);  };";
        }
    }

    boost::regex r_algo_ago{"\\b(\\d*)(?:_ago_)([a-z]*)(?:_)(\\w*)\\b"};
    boost::smatch r_algo_ago_m;

    while(boost::regex_search(code_line, r_algo_ago_m, r_algo_ago)){
        string algoname = r_algo_ago_m[2].str();
        string algoname_unique = r_algo_current_m[2].str() + "_" + r_algo_current_m[3].str();

        boost::replace_first(code_line, r_algo_ago_m[0].str(), "window."+ algoname_unique +"_values[window."+  algoname_unique +"_values.length-"+r_algo_ago_m[1].str()+"]);");



        boost::smatch empty_match;
        r_algo_ago_m = empty_match;

        if(std::find(algos.begin(), algos.end(), algoname_unique) == algos.end()) {
            algos.push_back(algoname_unique);
            algos_t.push_back(algoname_unique);


            init_code += "window." + algoname_unique + "_values = [\"0\"]; ";

            string ws_port = "";

            if (algoname == "bollinger"){ ws_port = "30090"; }
            if (algoname == "rsi"){ ws_port = "30091"; }
            if (algoname == "ema"){ ws_port = "30092"; }
            if (algoname == "sma"){ ws_port = "30093"; }
            if (algoname == "macd"){ ws_port = "30094"; }


            init_code += "window." + algoname_unique+ "_socket = new WebSocket(\"ws://127.0.0.1:" + ws_port + "\"); ";
            init_code += "window." + algoname_unique + "_socket.onmessage = function (event) {  \t" +"window."+algoname_unique+ "_values.push(event.data); } ";
            init_code += "window." + algoname_unique + "_socket.onerror = function (error) {  \t console.log(error);  }";
        }
    }

    boost::regex r_price{"\\b(\\w{1,5})(?:_price)\\b"};
    boost::smatch r_price_m;

    while(boost::regex_search(code_line, r_price_m, r_price)){
        string ticker = r_price_m[1].str();

        boost::replace_first(code_line, r_price_m[0].str(), "window." + ticker + "_values.slice(-1).pop();");

        boost::smatch empty_match;
        r_price_m = empty_match;

        if(std::find(tickers.begin(), tickers.end(), ticker) == tickers.end()) {
            tickers.push_back(ticker);

            init_code += "window." + ticker + "_values = [\"0\"]; ";
            init_code += "window.tickers.push(\"" + ticker + "\"); ";
        }
    }

    boost::regex r_ago_price ("\\b(\\d*)(?:_ago_)(\\w{1,5})(?:_price)\\b");
    boost::smatch r_ago_price_m;
    while(boost::regex_search(code_line, r_ago_price_m, r_ago_price)){
        boost::replace_first(code_line, r_ago_price_m[0].str(), "window."+r_ago_price_m[2].str()+"_values[window."+r_ago_price_m[2].str()+"_values.length-"+r_ago_price_m[1].str()+"]);");
        string ticker = r_ago_price_m[2].str();

        boost::smatch empty_match;
        r_ago_price_m = empty_match;

        if(std::find(tickers.begin(), tickers.end(), ticker) == tickers.end()) {
            tickers.push_back(ticker);

            init_code += "window." + ticker + "_values = [\"0\"]; ";
            init_code += "window.tickers.push(\"" + ticker + "\"); ";
        }
    }


    boost::replace_all(code_line, "more_than", ">");
    boost::replace_all(code_line, "more_than_or_equals", ">=");
    boost::replace_all(code_line, "less_than", "<");
    boost::replace_all(code_line, "less_than_or_equals", "<=");
    boost::replace_all(code_line, "equals", "==");
    boost::replace_all(code_line, "not_equals", "!=");
    boost::replace_all(code_line, "and", "&&");
    boost::replace_all(code_line, "or", "||");



    // var_bing_2
    boost::regex r_var{"\\b(?:var_)(\\w*)(?:_)(\\w*)\\b"};
    boost::smatch r_var_m;
    while(boost::regex_search(code_line, r_var_m, r_var)) {
        boost::replace_first(code_line, r_var_m[0].str(), "var " + r_var_m[1].str() + " = " + r_var_m[2].str() + ";");
        boost::smatch empty_match;
        r_var_m = empty_match;
    }

    // TODO:, algo finish, total, legend ?
    //buy,sell, fundamentals

    boost::regex r_buy{"\\b(?:buy_)(\\w*)\\b"};
    boost::smatch r_buy_m;

    while(boost::regex_search(code_line, r_buy_m, r_buy)) {
        boost::replace_first(code_line, r_buy_m[0].str(), "var bo = [\"" + r_buy_m[1].str() +  "\", window." + r_buy_m[1].str() + "_values.slice(-1).pop()];  window.buys.push(bo);");
        boost::smatch empty_match;
        r_buy_m = empty_match;
    }


    boost::regex r_sell{"\\b(?:sell_)(\\w*)\\b"};
    boost::smatch r_sell_m;

    while(boost::regex_search(code_line, r_sell_m, r_sell)) {
        boost::replace_first(code_line, r_sell_m[0].str(), "var so = [\"" + r_sell_m[1].str() +  "\", window." + r_sell_m[1].str() + "_values.slice(-1).pop()];  window.sells.push(so);");
        boost::smatch empty_match;
        r_sell_m = empty_match;
    }


    // fundamentals_AAPL_2014q3_tic
    boost::regex r_fundamentals{"\\b(?:fundamentals_)(\\w*)(?:_)(\\w*)(?:[_])(\\w*)\\b"}; boost::smatch r_fundamentals_m;

    while(boost::regex_search(code_line, r_fundamentals_m, r_fundamentals)) {
        string funda = r_fundamentals_m[1].str() + "_" + r_fundamentals_m[2].str() + "_" + r_fundamentals_m[3].str(); // putting this after oost replace corrupts it..

        boost::replace_first(code_line, r_fundamentals_m[0].str(), "window.fundamentals[\""+r_fundamentals_m[1].str() + "_" + r_fundamentals_m[2].str() + "_" + r_fundamentals_m[3].str() + "\"]");

        boost::smatch empty_match;
        r_fundamentals_m = empty_match;

        if(std::find(fundamentals.begin(), fundamentals.end(), funda) == fundamentals.end()) {
            fundamentals.push_back(funda);
        }
    }

    boost::regex r_if ("(?:if)[ ](.*)[ ](?:{)");
    boost::smatch r_if_m;
    while(boost::regex_search(code_line, r_if_m, r_if)){
        string replacement = r_if_m[1].str();
        boost::replace_all(replacement, ";", "");
        boost::replace_first(code_line, r_if_m[0].str(), "if("+ replacement +"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }

    boost::regex r_else_if ("(?:}else if)[ ](.*)[ ](?:{)");
    boost::smatch r_else_if_m;
    while(boost::regex_search(code_line, r_else_if_m, r_else_if)){
        string replacement = r_else_if_m[1].str();
        boost::replace_all(replacement, ";", "");
        boost::replace_first(code_line, r_else_if_m[0].str(), "}else if("+replacement+"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }

    boost::regex r_while ("(?:while)[ ](.*)[ ](?:{)");
    boost::smatch r_while_m;
    while(boost::regex_search(code_line, r_while_m, r_while)){
        string replacement = r_while_m[1].str();
        boost::replace_all(replacement, ";", "");
        boost::replace_first(code_line, r_while_m[0].str(), "while("+ replacement +"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }



    return code_line;

}

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
        http_response res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");
        load_script(req.body());
        json req_code;
        req_code["code"] = code;
        res.body() = req_code.dump();
        code = "";
        init_code = "";

        res.prepare_payload();
        return send(std::move(res));
    }
}


#endif
