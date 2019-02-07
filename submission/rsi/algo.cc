#include "algo.h"

float Algo::algo_rsi(float value){
   if (daynum < 14){
        if (daynum == 0){
            prevval = value;
            sumgain += value;
            daynum += 1;
            // # for now still ignore rsi.
        }else{
            prevval = value - prevval;
            if (prevval < 0){ //loss
                sumloss += (prevval * -1); // #ngative here
            }else{ //# gain
                sumgain += prevval;
                daynum += 1;
            }
        }
    }else if (daynum == 14){
            prevval = value - prevval;
            if (prevval < 0){ // #loss
                sumloss += (prevval * -1); //#unngative here
            }else{ //# gain
                sumgain += prevval;
            }

            avgain = sumgain / 14;
            avloss = sumloss / 14;

            if (avloss == 0){
                rsiret = 100 - (100/(1+(avgain/1)));
            }else{
                rsiret = 100 - (100/(1+(avgain/avloss)));
            }

            daynum += 1;
    }else if (daynum > 14){
            prevval = value - prevval;
            if (prevval < 0){ //#loss
                avgain = ((avgain * 13) + 0 )/14;
                avloss = ((avloss * 13) + (prevval * -1))/14;
            }else{// # gain
                avgain = ((avgain * 13) + prevval)/14;
                avloss = ((avloss * 13) + 0)/14;
            }
                
            if (avloss == 0){
                rsiret = 100 - (100/(1+(avgain/1)));
            }else{
                rsiret = 100 - (100/(1+(avgain/avloss)));
            }
            daynum += 1;
    }
    return rsiret;
}