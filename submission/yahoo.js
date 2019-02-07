const CDP = require('chrome-remote-interface');
const fs = require('fs');
var request = require('sync-request');


var cnbc_url = "http://www.cnbc.com/finance/?page="
var nyt_url = "https://www.nytimes.com/search?sort=best&query="
var alphabets = " abcdefghijklmnopqrstuvwxyz" // for each and times 3 inceemnmtally in nyt search
var url_debugger = null
var detailsarr = []

// attempt dynamic selctor
var selector = {
    yahoo_profile_name: "#Col1-0-Profile-Proxy > section > div.asset-profile-container > div > h3",
    yahoo_profile_contact: "#Col1-0-Profile-Proxy > section > div.asset-profile-container > div > div > p",
    yahoo_profile_desc: "#Col1-0-Profile-Proxy > section > div.asset-profile-container > div > div > p:nth-child(2)",
    yahoo_profile_executives: "#Col1-0-Profile-Proxy > section > section.quote-subsection.undefined > table > tbody > tr:nth-child({row}) > td:nth-child({column})",
    yahoo_financials_revenue_section: "#Col1-1-Financials-Proxy > section > div > table > tbody > tr:nth-child({row}) > td:nth-child({column}) > span",
    yahoo_financials_operating_section: "#Col1-1-Financials-Proxy > section > div > table > tbody > tr:nth-child(6 + n) > td:nth-child(n)",
    yahoo_financials_operationsincomes_section: "#Col1-1-Financials-Proxy > section > div > table > tbody > tr:nth-child(13 + n) > td:nth-child(n)",
    yahoo_financials_reccuringevents_section: "#Col1-1-Financials-Proxy > section > div > table > tbody > tr:nth-child(21 + n) > td",
    yahoo_financials_income_section: "#Col1-1-Financials-Proxy > section > div > table > tbody > tr:nth-child(26 + n) > td:nth-child(n)",

    yahoo_analaysis_earnings_estimate_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child({table}) > tbody > tr:nth-child({row}) > td:nth-child({column})",

    
    yahoo_analaysis_earnings_estimate_titles_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child({table}) > thead > tr > th:nth-child({column})",



    yahoo_analaysis_revenue_estimate_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child(3) > tbody > tr:nth-child(1) > td:nth-child(2)",
    yahoo_analaysis_earnings_history_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child(4) > tbody > tr:nth-child(1) > td:nth-child(2) ",
    yahoo_analaysis_eps_trend_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child(5) > tbody > tr:nth-child(1) > td:nth-child(2)",
    yahoo_analaysis_eps_revisions_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child(6) > tbody > tr:nth-child(1) > td:nth-child(2)",
    yahoo_analaysis_growth_estimates_section: "#Col1-0-AnalystLeafPage-Proxy > section > table:nth-child(7) > tbody > tr:nth-child(2) > td:nth-child(3)",
    yahoo_statistics_valuation_measures_section: "#Col1-0-KeyStatistics-Proxy > section > div> div > div:nth-child(2) > div > table > tbody > tr:nth-child({title}) > td:nth-child({info})",
    yahoo_statistics_financial_highlights_section: "#Col1-0-KeyStatistics-Proxy > section > div > div > div:nth-child(4) > div:nth-child({section}) > table > tbody > tr:nth-child({title}) > td:nth-child({info})", // n title, n info, nn section
    yahoo_statistics_trading_information_section: "#Col1-0-KeyStatistics-Proxy > section > div > div > div > div:nth-child({section}) > table:nth-child(2) > tbody > tr:nth-child({title}) > td:nth-child({info}) > span",  // n title, n info, nn section
    yahoo_summary_section: "#quote-summary > div:nth-child({section}) > table > tbody > tr:nth-child({title}) > td:nth-child({info})", // n title, n info, nn section
    yahoo_holders_major_holders_info: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(2) > div > table > tbody >  tr:nth-child({row}) > td:nth-child(1)",
    //check for amount of hodlers, varies  
    yahoo_holders_major_holders_title: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(2) > div > table > tbody > tr:nth-child({row}) > td:nth-child(2) > span",
    yahoo_holders_inst_table: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(3) > table > tbody",
    yahoo_holders_mutal_table: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(4) > table > tbody",
    yahoo_holders_institutional_holders: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(3) > table > tbody > tr:nth-child({row}) > td:nth-child({column})",
    yahoo_holders_mutual_fund_holders: "#Col1-1-Holders-Proxy > section > div:nth-child(2) > div:nth-child(4) > table > tbody > tr:nth-child({row}) > td:nth-child({column})",

    cnbc_page_sel: "#pipeline_assetlist_0 > li > div > div > a", // title + href
    cnbc_page_date: "#cnbc-contents > div > div.story-top > div.story-header-left.twoCol > time:nth-child(4)",

    nyt_page_title: "#site-content > div > div > div:nth-child(2) > div > ol > li:nth-child(3) > div > div > a > h4",
    nyt_page_link: "#site-content > div > div > div:nth-child(2) > div > ol > li:nth-child(3) > div > div > a",
    nyt_archive_text: "#story > div > div.story-body > p",
    nyt_normal_text: "#story > section > div > div > p",
    nyt_normal_date: "#post-30026 > header > div.story-meta-footer > time"
}


async function yahoo_fiancials_crawl() {
    let client;
    try {
        // connect to endpoint
        client = await CDP({'host': '172.26.89.53', 'port': 9223});
        
        // extract domains
        const {Network, Page, Runtime} = client;
        
        // setup handlers
        Network.requestWillBeSent((params) => {
            console.log(params.request.url);
        });
        
        // enable events then start!
        await Page.enable();
        for(var i = 0; i < alphabets.length; ++i){
            for(var ii = 0; ii < alphabets.length; ++ii){
                for(var iii = 0; iii < alphabets.length; ++iii){
                    for(var iiii = 0; iiii < alphabets.length; ++iiii){
                        var combi = alphabets.charAt(i)+alphabets.charAt(ii)+alphabets.charAt(iii)+alphabets.charAt(iiii)
                        combi.replace(/\s/g,'')

                        // var combi = "MSFT"
                        // navigate to page
                        await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi});
                        await Page.loadEventFired();
                        
                        // check for page validity
                        page_url_res = await Runtime.evaluate({
                            expression: "location.href"
                        })        
                        page_url = page_url_res.result.value
                        if(page_url.includes("lookup") == false){
                            // crawl operations

                            // summary page, same page
                            summary_details = {}
                            for(let index = 1; index <= 2; ++index ){
                                for(let indexes = 1; indexes <= 8; ++indexes ){

                                    yahoo_summary_title_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_summary_section.replace("{section}", index).replace("{title}", indexes).replace("{info}", 1) + "').innerText"
                                    })
                                    yahoo_summary_info_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_summary_section.replace("{section}", index).replace("{title}", indexes).replace("{info}", 2) + "').innerText"
                                    })
                                    
                                    summary_details[yahoo_summary_title_res.result.value] = String(yahoo_summary_info_res.result.value)
                                }
                            }

                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/key-statistics?p="+combi});
                            await Page.loadEventFired();

                            // statistics page
                            statistics_valuation_details = {}
                            for(let indexes = 1; indexes <= 9; ++indexes ){

                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_valuation_measures_section.replace("{title}", indexes).replace("{info}", 1) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_valuation_measures_section.replace("{title}", indexes).replace("{info}", 2) + "').innerText"
                                })
                                statistics_valuation_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            statistics_financial_details = {}
                            // fiscal year
                            for(let indexes = 1; indexes <= 2; ++indexes ){
                                let section = 1
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                             // proftiablity
                             for(let indexes = 1; indexes <= 2; ++indexes ){
                                let section = 2
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            
                             // management effectivenes
                             for(let indexes = 1; indexes <= 2; ++indexes ){
                                let section = 3
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            // income statment
                            for(let indexes = 1; indexes <= 8; ++indexes ){
                                let section = 4
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            // balance sheet
                            for(let indexes = 1; indexes <= 6; ++indexes ){
                                let section = 5
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            // balance sheet
                            for(let indexes = 1; indexes <= 2; ++indexes ){
                                let section = 6
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_financial_highlights_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_financial_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                        
                            
                            // commit to db
    
                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/key-statistics?p="+combi});
                            await Page.loadEventFired();

                            // trading infomation section
                            statistics_trading_details = {}
                            
                            for(let indexes = 1; indexes <= 7; ++indexes ){
                                let section = 1
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_trading_information_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_valuation_measures_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_trading_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            for(let indexes = 1; indexes <= 11; ++indexes ){
                                let section = 2
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_trading_information_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_valuation_measures_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_trading_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }

                            for(let indexes = 1; indexes <= 10; ++indexes ){
                                let section = 3
                                yahoo_statistics_title_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_trading_information_section.replace("{title}", indexes).replace("{info}", 1).replace("{section}", section) + "').innerText"
                                })
                                yahoo_statistics_info_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_statistics_valuation_measures_section.replace("{title}", indexes).replace("{info}", 2).replace("{section}", section) + "').innerText"
                                })
                                statistics_trading_details[yahoo_statistics_title_res.result.value] = String(yahoo_statistics_info_res.result.value)
                            }


                            
                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/profile?p="+combi});
                            await Page.loadEventFired();

                            // profile page
                            profile_details = {}
                            
                            yahoo_profile_name_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.yahoo_profile_name+"').innerText"
                            })
                            profile_details["name"] = String(yahoo_profile_name_res.result.value)

                            yahoo_profile_contact_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.yahoo_profile_contact+"').innerText"
                            })
                            profile_details["contact"] = String(yahoo_profile_contact_res.result.value)

                            yahoo_profile_desc_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.yahoo_profile_desc+"').innerText"
                            })
                            profile_details["desc"] = String(yahoo_profile_desc_res.result.value)

                            profile_details["executives"] = []
                            for(let rowind = 1; rowind <= 5; ++rowind){
                                execs_sec = []
                                for(let colind = 1; colind <= 5; ++colind){

                                    // name, title, pay, excercised, year born
                                    yahoo_profile_exec_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_profile_executives.replace("{row}", rowind).replace("{column}", colind)+"').innerText"
                                    })
                                    execs_sec.push(String(yahoo_profile_exec_res.result.value))
                                }
                                profile_details["executives"].push(execs_sec)
                            }

                            console.log(profile_details)

                            // Financials section
                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/financials?p="+combi});
                            await Page.loadEventFired();

                            financials_details = {}
                            financials_quarters = []
                            //get titles
                            for(let finindex = 2; finindex < 6; ++finindex){
                                yahoo_financials_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", 1).replace("{column}", finindex) +"').innerText"
                                })
                                financials_quarters.push(yahoo_financials_res.result.value)
                            }
                            financials_details["quarters"] = financials_quarters
                            
                            for(let finrow = 2; finrow <= 28; ++finrow){
                                if (finrow != 5 || 12 || 20 || 25){
                                    yahoo_financials_title_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", finrow).replace("{column}", 1) +"').innerText"
                                    })
                                    financials_data = []
                                    for(let finindex = 2; finindex < 6; ++finindex){ 
                                        yahoo_financials_data_res = await Runtime.evaluate({
                                            expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", finrow).replace("{column}", finindex) +"').innerText"
                                        })
                                        data_res = yahoo_financials_data_res.result.value
                                        if (data_res == undefined){
                                            data_res = "-"
                                        }
                                        financials_data.push(data_res)
                                    }
                                    financials_details[yahoo_financials_title_res.result.value] = financials_data
                                }
                            }


                            // Analysis section
                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/analysis?p="+combi});
                            await Page.loadEventFired();

                            analysis_details = {}

                            for(let fintab = 2; fintab <= 7; ++fintab){
                                // Get Titles
                                table_titles = []
                                table_section = ""

                                yahoo_financials_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_analaysis_earnings_estimate_titles_section.replace("{column}", 1).replace("{table}",fintab) +"').innerText"
                                })
                                table_section = yahoo_financials_res.result.value
                         

                                for(let fincol = 2; fincol <= 5; ++fincol){
                                    yahoo_financials_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_analaysis_earnings_estimate_titles_section.replace("{column}", fincol).replace("{table}",fintab) +"').innerText"
                                    })
                                    table_titles.push(yahoo_financials_res.result.value)
                                }
                                console.log(table_titles)
                                analysis_details[table_section] = {}
                                analysis_details[table_section]["headings"] = table_titles

                            }

                            var row_records = "564546"
                            var fintab = 0
                            for(let rrl = 0; rrl < row_records.length; ++rrl){
                                row_data = []
                                var row_title = ""
                                for(let finrow = 2; finrow <= parseInt(row_records.charAt(rrl))+1; ++finrow){   
                                    fintab += 1
                                    for(let fincol = 2; fincol <= 4; ++fincol){   

                                        yahoo_financials_res = await Runtime.evaluate({
                                            expression: "document.querySelector('"+selector.yahoo_analaysis_earnings_estimate_section.replace("{column}", fincol).replace("{table}",fintab).replace("{row}", finrow) +"').innerText"
                                        })
                                        row_data.push(yahoo_financials_res.result.value)
                                    }
                                    yahoo_financials_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_analaysis_earnings_estimate_section.replace("{column}", 1).replace("{table}",fintab).replace("{row}", finrow) +"').innerText"
                                    })
                                    row_title = yahoo_financials_res.result.value
                                }
                              

                                analysis_details[table_section]["headings"][row_title] = row_data
                            }   
                            

                            // Analysis section
                            // navigate to page
                            await Page.navigate({url: "https://finance.yahoo.com/quote/"+combi+"/holders?p="+combi});
                            await Page.loadEventFired();

                            holders_details = {}
                            holders_details["major_holders"] = {}
                            holders_details["inst_holders"] = {}
                            holders_details["mutal_holders"] = {}
                            holders_details["inst_holders"]["headers"] = ["holder", "shares", "Date" ,"Reported", "% Out", "Value"]
                            holders_details["mutal_holders"]["headers"] = ["holder", "shares", "Date" ,"Reported", "% Out", "Value"]
                            holders_details["inst_holders"]["data"] = []
                            holders_details["mutal_holders"]["data"] = []
                            
                            for(let index = 1; index <= 4; ++index){
                                yahoo_financials_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_holders_major_holders_title.replace("{row}", index)+"').innerText"
                                })
                                title = yahoo_financials_res.result.value
                                
                                yahoo_financials_res = await Runtime.evaluate({
                                    expression: "document.querySelector('"+selector.yahoo_holders_major_holders_info.replace("{row}", index)+"').innerText"
                                })
                                info = yahoo_financials_res.result.value
                                holders_details["major_holders"][title] = info
                            }

                            yahoo_financials_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.yahoo_holders_inst_table+"').childElementCount"
                            })
                            yahoo_inst_table_length = parseInt(yahoo_financials_res.result.value)
                            
                            yahoo_financials_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.yahoo_holders_mutal_table+"').childElementCount"
                            })
                            yahoo_mutual_table_length = parseInt(yahoo_financials_res.result.value)
                            

                            for(let index = 1; index <= yahoo_inst_table_length; ++index){
                                let data_arry = []
                                for(let colindex = 1; colindex <= 5; ++colindex){

                                    yahoo_financials_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_holders_institutional_holders.replace("{row}", index).replace("{column}", colindex)+"').innerText"
                                    })
                                    data_arry.push(yahoo_financials_res.result.value)
                                }
                                holders_details["inst_holders"]["data"].push(data_arry)
                            }

                            for(let index = 1; index <= yahoo_mutual_table_length; ++index){
                                let data_arry = []
                                for(let colindex = 1; colindex <= 5; ++colindex){

                                    yahoo_financials_res = await Runtime.evaluate({
                                        expression: "document.querySelector('"+selector.yahoo_holders_mutual_fund_holders.replace("{row}", index).replace("{column}", colindex)+"').innerText"
                                    })
                                    data_arry.push(yahoo_financials_res.result.value)
                                }
                                holders_details["mutal_holders"]["data"].push(data_arry)
                            }
                            
                            result_details = {}
                            result_details["summary_details"] = summary_details
                            result_details["analysis_details"] = analysis_details
                            result_details["financials_details"] = financials_details
                            result_details["profile_details"] = profile_details
                            result_details["statistics_trading_details"] = statistics_trading_details
                            result_details["statistics_financial_details"] = statistics_financial_details
                            result_details["statistics_valuation_details"] = statistics_valuation_details
                            result_details["holders_details"] = holders_details

             
                            console.log(result_details)





                            
                            
                            // console.log(analysis_details)
                            // for(let finrow = 2; finrow <= 28; ++finrow){
                            //     if (finrow == 5 || 12 || 20 || 25){
                                            
                            //         //get titles
                            //         for(let finindex = 2; finindex < 6; ++finindex){
                            //             yahoo_financials_res = await Runtime.evaluate({
                            //                 expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", 1).replace("{column}", finindex) +"').innerText"
                            //             })
                            //             financials_quarters.push(yahoo_financials_res.result.value)
                            //         }
                            //         financials_details["quarters"] = financials_quarters
                            //     }else{
                            //         yahoo_financials_title_res = await Runtime.evaluate({
                            //             expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", finrow).replace("{column}", 1) +"').innerText"
                            //         })
                            //         financials_data = []
                            //         for(let finindex = 2; finindex < 6; ++finindex){ 
                            //             yahoo_financials_data_res = await Runtime.evaluate({
                            //                 expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", finrow).replace("{column}", finindex) +"').innerText"
                            //             })
                            //             data_res = yahoo_financials_data_res.result.value
                            //             if (data_res == undefined){
                            //                 data_res = "-"
                            //             }
                            //             financials_data.push(data_res)
                            //         }
                            //         financials_details[yahoo_financials_title_res.result.value] = financials_data
                            //     }
                            // }



                            // for(let finindex = 1; finindex < 5; ++finindex){
                            //     for(let finindexcol = 1; finindexcol < 6; ++finindexcol){
                            //         yahoo_financials_res = await Runtime.evaluate({
                            //             expression: "document.querySelector('"+selector.yahoo_financials_revenue_section.replace("{row}", finindex).replace("{column}", finindexcol) +"').innerText"
                            //         })
                            //         console.log(yahoo_financials_res)
                            //     }
                            // }

                          
                            // profile_details["name"] = String(yahoo_profile_name_res.result.value)

                            
                        
                        }



                    }
                }
            }   
        }


    // catch errors
    } catch (err) {
        console.error(err);
    } finally {
        // close client
        if (client) {
            await client.close();
        }
    }
}

async function cnbc_crawl(){

}

async function nyt_crawl(){
    
}
async function main(){
    await yahoo_fiancials_crawl()
}

main()