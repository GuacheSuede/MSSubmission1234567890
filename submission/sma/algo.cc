#include "algo.h"

float Algo::algo_sma(float value){


        if (daynum < 50){
            sma50totallist.push_back(value);
        }else if(daynum == 50){
            sma50totallist.push_back(value);
            float sma50total = 0;
            for (int xx : boost::irange(0,50)) {
                sma50total+=sma50totallist[xx];
            }
            smaret = sma50total/50;
        }else if(daynum > 50){
            sma50totallist.erase(sma50totallist.begin());
            sma50totallist.push_back(value);
            float sma50total = 0;
            for (int xx : boost::irange(0,50)) {
                sma50total+=sma50totallist[xx];
            }
            smaret = sma50total/50;
        }


        daynum += 1;

        return smaret;
        // slowly type to learn corect form and abilty

        // not supposed to rest wrist for efective typing, a training for life
}