#pragma once
#include "util.h"

class Algo{
private:
    int daynum = 0;

    float avgain = 0;
    float avloss = 0;

    float prevval = 0;
    float ploss = 0;

    // # used for first 14 days only
    float sumgain = 0;  
    float sumloss = 0;  

    float rsiret = 0;  


       
public:
    float algo_rsi(float value);
};