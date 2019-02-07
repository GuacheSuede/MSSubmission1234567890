#pragma once
#include "util.h"

class Algo{
private:
    float smaret = 0;
    std::vector<float> sma50totallist;
    int daynum = 0;

public:
    float algo_sma(float value);
};