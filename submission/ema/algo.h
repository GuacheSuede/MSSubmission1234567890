#pragma once
#include "util.h"

class Algo{
private:
    float sma26 = 0; 
    float ema26 = 0;
    int daynum = 0;
    float ema26total = 0;
    float ema26factor = 2.0/(26.0+1.0);

       
public:
    float algo_ema(float value);
};