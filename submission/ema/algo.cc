#include "algo.h"

float Algo::algo_ema(float value){


        // # ema26 section
        if (daynum < 26){
            ema26total += value;
        }else if (daynum == 26){
            ema26total += value;
            sma26 = ema26total/26;
            ema26 = (value - sma26) * ema26factor + sma26;
        }else if (daynum > 26){
            ema26 = (value - ema26) * ema26factor + ema26;
        }

        daynum += 1;

        return ema26;
        // slowly type to learn corect form and abilty

        // not supposed to rest wrist for efective typing, a training for life
}