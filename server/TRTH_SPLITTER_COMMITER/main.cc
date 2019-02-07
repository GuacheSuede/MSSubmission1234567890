#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <thread>
#include <unistd.h>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;
using namespace std;

string auth_token;

vector<string> part_instruments_ids;

vector<future<void>> async_instrument_create_requests;
vector<future<void>> async_instrument_add_requests;

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

void create_instrument_list(string file_name){
    string instrument_creation_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Extractions/InstrumentLists";
    json request;
    request["@odata.type"] = "#ThomsonReuters.Dss.Api.Extractions.SubjectLists.InstrumentList";
    request["Name"] =  file_name + "_" + to_string(time(0));


    auto r = cpr::Post(
            cpr::Url{instrument_creation_url},
            cpr::VerifySsl(false),
            cpr::Body{request.dump()},
            cpr::Header{
                    {"Content-Type", "application/json"},
                    {"Prefer", "respond-async"},
                    {"Authorization", "Token "+ auth_token}
            }
    );

    json response = json::parse(r.text);
    part_instruments_ids.push_back(response["ListId"]);
}

void add_to_instrument_list(string pid, vector<string> rics){
    string instrument_add_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Extractions/InstrumentLists('"+pid+"')/ThomsonReuters.Dss.Api.Extractions.InstrumentListAppendIdentifiers";


    json request;
    json identifiers;

    for(auto &ric: rics){
        json identifier;
        identifier["Identifier"] = ric;
        identifier["IdentifierType"] = "Ric";
        identifiers.push_back(identifier);
    }

    request["Identifiers"] = identifiers;
    request["KeepDuplicates"] = false;




    auto r = cpr::Post(
            cpr::Url{instrument_add_url},
            cpr::VerifySsl(false),
            cpr::Body{request.dump()},
            cpr::Header{
                    {"Content-Type", "application/json"},
                    {"Prefer", "respond-async"},
                    {"Authorization", "Token "+ auth_token}
            }
    );

}

int main(){
    int part = 0;
    int part_tracker = 0;
    vector<string> part_vec;
    vector<string> part_names;


    // ofstream iex_trth("IEXTRTHSymbolsList.csv");

//    //  Open IEX File
    ifstream iex_file;
    iex_file.open("FinalIEXTRTHSymbolsList.csv");
    if (iex_file.is_open()){
        string iex_buffer;

        while (getline(iex_file, iex_buffer)){
          ++part_tracker;
          if(part_tracker < 10){
              part_vec.push_back(iex_buffer);
          }else{
            ++part;
            string name = "FinalSymbolsListPart"+to_string(part)+".csv";
            part_names.push_back(name);
            ofstream iex_trth(name);
            for(auto &pv: part_vec){
                iex_trth << pv << "\n";
            }
            iex_trth.close();
            part_tracker = 0;
            part_vec.clear();
          }
        }
    }
        
    iex_file.close();



    auth_token = auth();
    for (int pni = 0; pni < part_names.size(); ++pni) {

        async_instrument_create_requests.push_back(async(launch::async, create_instrument_list, part_names[pni]));
        usleep(50000);

    }

    for (auto &icr: async_instrument_create_requests) {
        icr.wait();
    }
    for (auto &icr: async_instrument_create_requests) {
        icr.get();
    }

    for (int pni = 0; pni < part_names.size(); ++pni) {


        vector<string> rics;

        ifstream iexxxxx_file(part_names[pni]);
        if (iex_file.is_open()){
            string iexxxxxx_buffer;
            while (getline(iexxxxx_file, iexxxxxx_buffer)){
                rics.push_back(iexxxxxx_buffer);
            }
        }
        iexxxxx_file.close();

        async_instrument_add_requests.push_back(async(launch::async, add_to_instrument_list, part_instruments_ids[pni], rics));
        usleep(50000);

    }

    for(auto &iar: async_instrument_add_requests){
        iar.wait();
    }
    for(auto &iar: async_instrument_add_requests){
        iar.get();
    }






}





