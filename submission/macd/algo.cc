#include "algo.h"

float Algo::algo_macd(float value){
        // # 12 ema and 26 ema
    
        // # ema12 section
        if (daynum < 12){
            ema12total += value;
        }else if (daynum == 12){
            ema12total += value;
            sma12 = ema12total/12;
            ema12 = (value - sma12) * ema12factor + sma12;
        }else if (daynum > 12){
            ema12 = (value - ema12) * ema12factor + ema12;
        }

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
        std::cout << ema26factor << std::endl;
        std::cout << daynum << std::endl;

       
        if (daynum >= 26){
            macdret = ema12 - ema26;
        }

        daynum += 1;
        return macdret;
        // slowly type to learn corect form and abilty

        // not supposed to rest wrist for efective typing, a training for life
}