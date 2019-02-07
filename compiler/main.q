
var_tracker_0
if MSFT_price > 10 {
    if AAPL_price and GOOG_price {
        if current_rsi_AAPL !=  20 { 
            if current_sma_MSFT > -1 { 
                if fundamentals_AAPL_2013Q2_ajexq > 2 {
                        if TSLA_price < 5000 {
                            buy_TSLA
                            buy_AAPL
                            buy_MSFT
                            sell_GOOG
                        }
                }
                }else{
                    sell_MSFT
                }
        }
    }
}else{
    buy_AAPL
}