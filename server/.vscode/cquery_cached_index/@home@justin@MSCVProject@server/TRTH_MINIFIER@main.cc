#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <libdill.h>
#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;
using namespace std;

string auth_token;
string ric_list;
vector<string> global_ric_list;
vector<string> search_strings;
string search_string;
int tracker = 0;

string prev_ric = "";

int main(){

    ofstream iexx_file("IEXTRTHSymbolsListMinified.csv");
    //  Open IEX File
    ifstream iex_file;
    iex_file.open("IEXTRTHSymbolsList.csv");
    if (iex_file.is_open()){
        string iex_buffer;
        vector<string> split_one;
        while (getline(iex_file, iex_buffer)){
          boost::replace_all(iex_buffer, "RIC,", "");
          boost::split(split_one, iex_buffer, boost::is_any_of("."));
          cout << iex_buffer << endl;
          if (split_one.empty())
              cout << "NULL" << endl;

//          if (prev_ric != split_one[0]){

//            iexx_file << split_one[0];
//            //if (split_one[1] != ""){
//              iexx_file << ".";
//              iexx_file << split_one[1];
//             // }
//            iexx_file << "\n";
//          }else{
//          }
//
//          cout << split_one[0];
//          prev_ric = split_one[0];
        }
    }
    iex_file.close();
    iexx_file.close();
}

