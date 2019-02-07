#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <atomic>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include<boost/tokenizer.hpp>

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

atomic<int> track_c = 0;

vector<future<string>> async_commits;
vector<string> commits_data;
vector<string> titles;

bool titled = false;
string file_ticker = "GOOG";
int date_index = 2;
int price_index = 5;


mongocxx::uri uri("mongodb://restheart:R3ste4rt!@localhost:27017");
mongocxx::database db;
mongocxx::collection prices_collection;

string sort_and_commit_rows(string wrds_buffer){
    vector<string> transient_row;
    boost::split(transient_row, wrds_buffer, boost::is_any_of(","));

    json ticker_price_row;
    ticker_price_row["ticker"] = file_ticker;
    ticker_price_row["datetime"] = transient_row[date_index];
    ticker_price_row["price"] = transient_row[price_index];

    return ticker_price_row.dump();
}

int main(){

    mongocxx::client client(uri);
    db = client["db"];
    prices_collection = db["prices"];

    ifstream wrds_file;
    wrds_file.open("../../files/trth/"+file_ticker+".csv");
    if (wrds_file.is_open()) {
        string wrds_buffer;

        while (getline(wrds_file, wrds_buffer)) {
            if (titled == true) {
                ++track_c;
                if (track_c < 30000) {
                    async_commits.push_back(async(launch::async, sort_and_commit_rows, wrds_buffer));
                } else {
                    for (auto &ac: async_commits) {
                        ac.wait();
                    }
                    for (auto &ac: async_commits) {
                        commits_data.push_back(ac.get());
                    }

                    auto bulk = prices_collection.create_bulk_write();
                    for (auto &cd: commits_data) {
                        mongocxx::model::insert_one insert_op{bsoncxx::from_json(cd)};
                        bulk.append(insert_op);
                    }
                    auto be = bulk.execute();
                    async_commits.clear();
                    commits_data.clear();
                    track_c = 0;
                }
            } else {
                boost::split(titles, wrds_buffer, boost::is_any_of(","));
                titled = true;
            }
        }
        wrds_file.close();
    }

    // Clean Up
    for(auto &ac: async_commits){
        ac.wait();
    }
    for(auto &ac: async_commits){
        commits_data.push_back(ac.get());
    }

    auto bulk = prices_collection.create_bulk_write();

    for(auto &cd: commits_data){
        mongocxx::model::insert_one insert_op{bsoncxx::from_json(cd)};
        bulk.append(insert_op);
    }
    auto be = bulk.execute();

}

