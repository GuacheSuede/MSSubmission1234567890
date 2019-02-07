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


#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/uri.hpp>

using json = nlohmann::json;
using namespace std;

int main(){

//    //  Open IEX File
    ifstream iex_file;
    iex_file.open("ric.csv");
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

