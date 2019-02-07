#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int main(){
    int part = 0;
    int part_tracker = 0;
    vector<string> part_vec;

    // ofstream iex_trth("IEXTRTHSymbolsList.csv");

//    //  Open IEX File
    ifstream iex_file;
    iex_file.open("FinalIEXTRTHSymbolsList.csv");
    if (iex_file.is_open()){
        string iex_buffer;

        while (getline(iex_file, iex_buffer)){
          ++part_tracker;
          if(part_tracker < 3000){
              part_vec.push_back(iex_buffer);
          }else{
            ++part;
            ofstream iex_trth("FinalIEXTRTHSymbolsListPart"+to_string(part)+".csv");
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
    
}

//thank god progres, fixed the bu aswell as enabeld std async for fast code
