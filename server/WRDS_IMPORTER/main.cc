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
using boost::tokenizer;
using boost::escaped_list_separator;

string prev_ticker = "";

vector<vector<string>> ticker_rows; int ticker_index = 9;

int tracker = 0;
int thousand_tracker = 0;

vector<string> titles;
bool titled = false;
json data_rows;

atomic<int> track_c = 0;

vector<future<string>> async_commits;
vector<string> commits_data;



mongocxx::uri uri("mongodb://restheart:R3ste4rt!@localhost:27017");
mongocxx::database db;
mongocxx::collection fundamentals_collection;

void commit_document(json data){
    std::string restheart_url = "http://localhost:8080/db/fundamentals";
    auto r = cpr::Post(
            cpr::Url{restheart_url},
            cpr::VerifySsl(false),
            cpr::Authentication{"admin", "changeit"},
            cpr::Body{data.dump()},
            cpr::Header{{"Content-Type", "application/json"}}
    );

}

void commit_document_direct(json data) {
    bsoncxx::stdx::optional <mongocxx::result::insert_one> result = fundamentals_collection.insert_one(bsoncxx::from_json(data.dump()));
}

string sort_and_commit_rows(string wrds_buffer){
    vector<string> transient_row;
    // FORMAT ROW
    typedef tokenizer<escaped_list_separator<char> > so_tokenizer;

    so_tokenizer tok(wrds_buffer, escaped_list_separator<char>('\\', ',', '\"'));

    for(so_tokenizer::iterator beg=tok.begin(); beg!=tok.end(); ++beg){
        transient_row.push_back(*beg);
    }
    string ticker = transient_row[9];

    json ticker_row_json;
    for (int ticker_index_row = 0; ticker_index_row < transient_row.size(); ++ticker_index_row) {
        ticker_row_json[titles[ticker_index_row]] = transient_row[ticker_index_row];
    }


//    bsoncxx::stdx::optional <mongocxx::result::insert_one> result = fundamentals_collection.insert_one(bsoncxx::from_json(ticker_row_json.dump()));

//    commit_document_direct(ticker_row_json);

//
//    std::string restheart_url = "http://localhost:8080/db/fundamentals";
//    auto r = cpr::Post(
//            cpr::Url{restheart_url},
//            cpr::VerifySsl(false),
//            cpr::Authentication{"admin", "changeit"},
//            cpr::Body{ticker_row_json.dump()},
//            cpr::Header{{"Content-Type", "application/json"}}
//    );
//
//    cout << "COMMITED ON:" << time(0) <<  " FOR " << ticker << endl;
    transient_row.clear();
    cout << ticker << endl;
    return ticker_row_json.dump();

}

int main(){
    mongocxx::client client(uri);
    db = client["db"];
    fundamentals_collection = db["fundamentals"];

    ifstream wrds_file;
    wrds_file.open("iex_na_all_variables_fundamentals_quarterly_excerpt.csv");
    if (wrds_file.is_open()){
        string wrds_buffer;

        while (getline(wrds_file, wrds_buffer)) {
            if (titled == true) {
                ++track_c;
                if(track_c > 10000){
                    for(auto &ac: async_commits){
                        ac.wait();
                    }
                    for(auto &ac: async_commits){
                        commits_data.push_back(ac.get());
                    }
//            cout << commits_data[0];
                    auto bulk = fundamentals_collection.create_bulk_write();
////
                    cout << commits_data.size();
                    for(auto &cd: commits_data){
                        mongocxx::model::insert_one insert_op{bsoncxx::from_json(cd)};
                        bulk.append(insert_op);
                    }
                    auto be = bulk.execute();
                    async_commits.clear();
                    commits_data.clear();
                    track_c = 0;
                    sleep(30);
                }else{
                async_commits.push_back(async(launch::async, sort_and_commit_rows, wrds_buffer));
                }
            } else {
                boost::split(titles, wrds_buffer, boost::is_any_of(","));
                titled = true;
            }
        }

//            cout << be << endl;



        wrds_file.close();
                    for(auto &ac: async_commits){
                        ac.wait();
                    }
                    for(auto &ac: async_commits){
                        commits_data.push_back(ac.get());
                    }
//            cout << commits_data[0];
                    auto bulk = fundamentals_collection.create_bulk_write();
////
                    cout << commits_data.size();
                    for(auto &cd: commits_data){
                        mongocxx::model::insert_one insert_op{bsoncxx::from_json(cd)};
                        bulk.append(insert_op);
                    }
                    auto be = bulk.execute();
                    async_commits.clear();
                    commits_data.clear();
                    track_c = 0;
    }
}

