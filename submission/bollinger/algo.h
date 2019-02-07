#pragma once
#include "util.h"

class Algo{
private:
    int sddaynum = 0;
    int smadaynum = 0;
    float smaret = 0;
    std::vector<float> sma20totallist;
    float sdret = 0;  
    std::vector<float> sd20totallist;

    void algo_sma_20(float value);
    void algo_sd_20(float value);

public:
    json algo_bollinger(float value);
};