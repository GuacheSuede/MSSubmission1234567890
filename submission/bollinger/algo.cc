#include "algo.h"

void Algo::algo_sma_20(float value){
    // # sma20 section
    if (smadaynum < 20){
        sma20totallist.push_back(value);
    }else if (smadaynum == 20){
        sma20totallist.push_back(value);
        float sma20total = 0;
        for (int xx : boost::irange(0,20)) {
            sma20total+=sma20totallist[xx];
        }
        smaret = sma20total/20;
     }else if (smadaynum > 20){
        sma20totallist.erase(sma20totallist.begin());
        sma20totallist.push_back(value);
        float   sma20total = 0;
       for (int xx : boost::irange(0,20)) {
            sma20total+=sma20totallist[xx];
        }
        smaret = sma20total/20;
     }
        
    smadaynum += 1;
}
        
    
void Algo::algo_sd_20(float value){
    // # sd20 section
    if (sddaynum < 20){
        sd20totallist.push_back(value);
    }else if (sddaynum == 20){
        sd20totallist.push_back(value);
        float s20total = 0;
        for (int xx : boost::irange(0,20)) {
            s20total+=sd20totallist[xx];
        }
        float s20mean = s20total/20;

        float sd20total = 0;
        for (int xx : boost::irange(0,20)) {
            sd20total+= pow(sd20totallist[xx] - s20mean, 2);
        }

        sdret = sqrt(sd20total / 20);

   }else if (sddaynum > 20){
        sd20totallist.erase(sd20totallist.begin());
        sd20totallist.push_back(value);
        float s20total = 0;
        for (int xx : boost::irange(0,20)) {
            s20total+=sd20totallist[xx];
        }
        float s20mean = s20total/20;

        float sd20total = 0;
        for (int xx : boost::irange(0,20)) {
            sd20total+= pow(sd20totallist[xx] - s20mean, 2);
        }
        sdret = sqrt(sd20total / 20);
   }

    sddaynum += 1;
}

json Algo::algo_bollinger(float value){
    json bolret;
    
    algo_sma_20(value);
    algo_sd_20(value);

    bolret["mid"] = smaret;
    bolret["up"] = smaret + (sdret * 2);
    bolret["dw"] = smaret - (sdret * 2);

    return bolret;
}