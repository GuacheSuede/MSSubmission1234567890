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
//    commit_document_direct(ticker_row_json);
    // actio keidl and lme signappreo saf army

    /// peo w lnto suaoppsoe pap and army osidotic,dicatatiorship wihto reaosn,sutpidity system
    //woamn hve otperonsalsie
    // sjut lie kgycncocentrism in lfie ,teh pcats arefavoured fvedr the dogs for looks and affection apeal,sfiotnes

    //wshitellbeor ana leakers uspally do for good of people
//    /stupdi woman mean leak andcheat on toehguys steial fro jgirfiend
//.//// lol hahaha, aloycisou pang kield by amry ,.. acitidnetlal padn
// //erely on ahdnsoem guys lol. ijmerelyl chai obnixious
//
//
// /hj ian wr
// // jiulia rich eocsoyte mlriabreia and support
//jlaii shoud lstare to be used by hedge funds
//gsg no surival ready
//dare to say and stop
//'m doing some pretty simple machine learning with Flux.jl. The thing I like most about doing ML in Julia is not so much the ML library itself but the fact that working in a fast language changes everything. Most of the work in ML (at least in my experience) is in collecting data, preparing data, analyzing data, etc., and having a language where simple constructs like loops and functions are fast means that all of my data processing can be fast and simple. I don't have to jump through Google's protobuf hoops just to feed my data into TensorFlow. Instead, I just have a Julia Vector of my custom Sample type and iterate through it in a loop, which gives me performance as good as C with very little code. My actual training loop is literally just

// no need work so har nejoy ?
// //dotn dvie to ofasrt
// nwo mroe pseignaproe nwilspeka out agisnt he en nd for it,diotici signapoe takek a alrg accient before the ycan doanythjing... senslels luselsl army pwihjto u eaosn nro logica ndmenaing
//
//onyl popaulr peop iwl lthe ysl;eak and tlak about
//wehn you do it smothly tyou dotn ahte it, change msient ,elarnt olvowe the prose and sytle of it

//Julia Language Co-Creators Win James H. Wilkinson Prize for Numerical Software (siam.org) you can win rpiszes fo goodo software
//    cout << prev_ticker << "=====" << ticker << endl;
// chiense afraid admit faults
    // IF END, CLEAN and COMMIT
//    if (ticker != "" && ticker != prev_ticker && ticker_rows.size() != 0){
//        transient_row.clear();

        //send to db and the ncontinuel
//        for (int ticker_index = 0; ticker_index < transient_row.size(); ++ticker_index) {

//            data_rows.push_back(ticker_row_json);
//            ++thousand_tracker;
//            if (thousand_tracker == 100){
//                cout << time(0)  << endl;
//                commit_document(data_rows);
//                data_rows.clear();
//                thousand_tracker = 0;
//                sleep(2);
//            }
//        }

        //        cout << data.dump() << endl;
        //learn to smoke peope ,whait onyl nenes to look as usch..
//        cout << "===========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================" << endl;

//        ticker_rows.clear();
//        ticker = "";
//// ahve cofndience 9in ur discoveries
// womane only appeal via sex
//the yvisually ugly whe nsex
//dare to ahv eocnfidnece
        /*
         * TSLA: [
         *  {
         *   k:v
         *  }
         *  {
         *  }
         *  {
         *  }
         * ]
         *
         *
         *
         *
         */
    // ELSE CONTINUE
//    }else{
//        ticker_rows.push_back(transient_row);
//        transient_row.clear();
//        prev_ticker = ticker;
//
//    }


    //jsoeph lvoe change htigns when peope arebusy..dotn stuppify self by follwogn toehrs
    //narcarcism will brign u dow ncos uthink uare abvoe othe and ahrdhsip is fine.. or perhpa laziness
//    the smart man edits hsi coe to serve hismelf nto try to coempsate for it or be lazy
//fast grpase fo scipritn fjs fast .. can see as c++ fst to dotn hin sk omuch
//do tnhink os much no nend ovoe htink
// dont be narcsitist, try thing
//bash tosl as apis hmm
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

