#include <iostream> 
#include <fstream>
#include <string> 
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

ifstream code_script;
string init_code;
string code;

string gen_syntax(string code_line);

void load_script(string name){ 
    code_script.open(name);
        if (code_script.is_open()){
            string code_script_buf;
            while (getline(code_script, code_script_buf)){
                code += gen_syntax(code_script_buf) + "\n";
            }
            code_script.close();
        }
}

/* 1. Normal Loop Code - normal code
 * 2. Init Loop Code - start code that assigns next value
 * 3. Init code - global start code like websocekts
 */
string gen_syntax(string code_line){
    /* Comment Declaration
     * # This will do this thing
     */
    if (code_line.find("#") != string::npos){
        // Do nothing
    }
    
    /* Variable Declaration
     * variable(graft,1)
     */
    if (code_line.find("variable") != string::npos){
        boost::replace_all(code_line, "variable(", "var ");
        boost::replace_all(code_line, ",", "=");
        boost::replace_all(code_line, ")", ";");
    }

    /* Array Init Declaration
     * array(graft,1)
     */
    if (code_line.find("array(") != string::npos){
        boost::replace_all(code_line, "array(", "var ");
        boost::replace_first(code_line, ",", "=[\"");
        boost::replace_all(code_line, ",", "\",\"");
        boost::replace_all(code_line, ")", "\"];");
    }

    /* Array Push Declaration
     * array_push(graft,1)
     */
    if (code_line.find("array_push") != string::npos){
        boost::replace_all(code_line, "array_push(", "");
        boost::replace_all(code_line, ",", ".push(\"");
        boost::replace_all(code_line, ")", "\")");
    }

    /* Array Pop Declaration
     * array_pop(graft)
     */
    if (code_line.find("array_pop") != string::npos){
        boost::replace_all(code_line, "array_pop(", "");
        boost::replace_all(code_line, ")", ".pop();");
    }


    /* Array Has Declaration
     * array_has(graft, "1")
     */
    if (code_line.find("array_has") != string::npos){
        boost::replace_all(code_line, "array_has(", "");
        boost::replace_first(code_line, ",", ".includes(\"");
        boost::replace_all(code_line, ")", "\")");
        code_line += ";";
    }

    /* Algo Init Declaration
     * use algo_name eg. use sma
     */
    if (code_line.find("use") != string::npos){
        boost::replace_first(code_line, "use", "");
        boost::replace_all(code_line, " ", "");
        string algo_name = code_line;
        code_line = "";
        code_line += "var " +algo_name+ "_values = []; \n";
        code_line += "var " +algo_name+ "_socket = new WebSocket(" +algo_name+ "_socket_url); \n";
        code_line += algo_name + "_socket.onmessage = function (event) { \n \t" +algo_name+ "_values.push(event.data)); \n} \n";
        // TODO: Add aupdate sectoin here andsperate into init and init loop code
    }

    
    /* Algo Reference Declaration, Current Variant
     * current_algo_name. eg. current_macd
     * Unable to use tokenisation due to space not consistent
     */
    if (code_line.find("current_") != string::npos){
        string alphabets = "abcdefghijklmnopqrstuvwxyz";
        int algo_name_length = 0;
        bool algo_name_end = false;
        string algo_name;
        int algo_name_start = code_line.find("current_")+7;
        
        while (algo_name_end == false){
            ++algo_name_start;
            if(alphabets.find(code_line.at(algo_name_start)) != string::npos){
                    algo_name += code_line.at(algo_name_start);
            }else{
                algo_name_end = true;
            }
        }

        boost::replace_first(code_line, "current_", "");
        // due to the nature of algorthims having variable amount of fields, wedo not specify what current_algo_name returns but rather it retursn an obbject if it's multiple values and an integer if it's a single value. data agnostic parser and tokeniser. 
        boost::replace_first(code_line, algo_name, algo_name+"_values.slice(-1).pop()");
         
    }

    /* Algo Reference Declaration, Ago Variant
     * days_ago_algo_name. eg. 2_ago_algo_name 
     * Unable to use tokenisation due to space not consistent
     */
    if (code_line.find("_ago_") != string::npos){
        string numbers = "0123456789";
        string alphabets = "abcdefghijklmnopqrstuvwxyz";
        int algo_name_length = 0;
        bool algo_name_end = false;
        string algo_name;
        int algo_name_start = code_line.find("_ago_")+4;
        
        while (algo_name_end == false){
            ++algo_name_start;
            if(alphabets.find(code_line.at(algo_name_start)) != string::npos){
                    algo_name += code_line.at(algo_name_start);
            }else{
                algo_name_end = true;
            }
        }


        boost::replace_first(code_line, "_ago_", "");
         
        int days_end = code_line.find(algo_name);
        int days_ended = false;
        int actual_days = 0;
        string days = "";
        
        while(days_ended == false){
            --days_end;
            if(numbers.find(code_line.at(days_end)) != string::npos){
                    days += code_line.at(days_end);
            }else{
                days_ended = true;
                boost::replace_first(code_line, days, "");
            }
        }

        boost::replace_first(code_line, algo_name, algo_name+"_values["+algo_name+"_values.length-"+days+"]");
    }

    // TODO: version with at vlaue
    return code_line;
}

int main() {
    load_script("script.q");
    cout << code << endl;
    return 0;
}


