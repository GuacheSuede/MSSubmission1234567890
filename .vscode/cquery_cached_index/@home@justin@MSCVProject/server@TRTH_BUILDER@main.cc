#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <libdill.h>
#include <mutex>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;
using namespace std;

string auth_token;
vector<string> global_ric_list;
vector<string> search_strings;
string search_string;
int tracker = 0;
int async_tracker = 0;
vector<future<string>> async_data_requests;
mutex async_mu;
atomic<int> tracker_counter;
atomic<int> trackered;

int get_tc(){
    return ++tracker_counter;
}

void reset_tc(){
    tracker_counter = 0;
}

int get_tcd(){
    return ++trackered;
}

void reset_tcd(){
    trackered = 0;
}
//void (json data){
//    std::string restheart_url = "http://localhost:8080/db/fundamentals";
//    auto r = cpr::Post(
//            cpr::Url{restheart_url},
//            cpr::VerifySsl(false),
//            cpr::Authentication{"admin", "changeit"},
//            cpr::Body{data.dump()},
//            cpr::Header{{"Content-Type", "application/json"}}
//    );
//
//}

// TRTH will not timeout if not too much

string auth(){
    string auth_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Authentication/RequestToken";

    // Building Auth Data
    json auth_data;
    json auth_data_2;
    auth_data_2["Username"] = "9017045";
    auth_data_2["Password"] = "gcG0dAVQ";
    auth_data["Credentials"] = auth_data_2;

    // Send Auth Request
    auto r = cpr::Post(
            cpr::Url{auth_url},
            cpr::VerifySsl(false),
            cpr::Body{auth_data.dump()},
            cpr::Header{
                {"Content-Type", "application/json"},
                {"Prefer", "respond-async"}
            }
    );

    // Get value and Return
    json auth_response = json::parse(r.text);
    return auth_response["value"];
}

coroutine string instrument_search(string ticker, int index){
    string instrument_search_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Search/InstrumentSearch";

    // Building Search Request Data
    json search_data  =  {
            {"InstrumentTypeGroups", {"Equities", "MortgageBackedSecurities", "Funds" }},
            {"IdentifierType", "Ticker"},
            {"Identifier", ticker},
            {"PreferredIdentifierType", "Ric"},
        };

    json search_req;
    search_req["SearchRequest"] = search_data;




    // Send Search Request
    auto r = cpr::Post(
            cpr::Url{instrument_search_url},
            cpr::VerifySsl(false),
            cpr::Body{search_req.dump()},
            cpr::Header{
                    {"Content-Type", "application/json"},
                    {"Prefer", "respond-async"},
                    {"Authorization", "Token "+ auth_token}
            }
    );

 

    cout << r.text;

    // Get value and Return list of RICs (current use case)
    json search_response;

    try {
        search_response = json::parse(r.text);
    } catch(json::parse_error) {
            cout << "PARSE ERROR" << endl;

            cout << r.status_code << endl;
            return "";
    }

    json value_list;
    try {
        value_list = search_response["value"];
    }catch (json::type_error){
        cout << "TYPE ERROR" << endl;
        cout << r.text << endl;
        return "";
    }




    // Commit first 3 RICs only - we could have changed to only get first 3 RICs from minifier but our previous implementation was buigged inthat it missed RIC's that were iportant, perhaps due to indeifier length limit
    if(!value_list.empty()) {
        string ric_list;
        for (int vll = 0; vll < 3; ++vll) {
            if (!value_list[vll].empty()) {
                ric_list += "RIC,";
                ric_list += value_list[vll]["Identifier"];
                ric_list += "\n";
                cout << index << "===" << ric_list << endl;

            }
        }
        return ric_list;
    }else{
        cout << "NO RICs" << endl;
        return "";
    }

}


int main(){

    auth_token = auth();
    ofstream iex_trth("IEXTRTHSymbolsList.csv");

//    //  Open IEX File
    ifstream iex_file;
    iex_file.open("IEXSymbolsList.txt");
    if (iex_file.is_open()){
        string iex_buffer;

        while (getline(iex_file, iex_buffer)){
            if (get_tc() < 100 ) { // too many request at once, will ahve trth error
                async_data_requests.push_back(async(launch::async, instrument_search, iex_buffer, get_tcd()));
            }else{
                for (auto &a: async_data_requests){
                    a.wait();
                }

                for (auto &a: async_data_requests){
                    iex_trth << a.get(); // required by c++ or seg fault
                }

                sleep(60);
                
                async_data_requests.clear();
                reset_tc();
            }
//            go(instrument_search(iex_buffer, tracker));
        }

           for (auto &a: async_data_requests){
                    a.wait();
                }

                for (auto &a: async_data_requests){
                    iex_trth << a.get(); // required by c++ or seg fault
                }

        

        
    iex_file.close();
    }


    iex_trth.close();

}

//thank god progres, fixed the bu aswell as enabeld std async for fast code
