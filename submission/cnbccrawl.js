const CDP = require('chrome-remote-interface');
const fs = require('fs');
var request = require('request');


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

    cnbc_page_sel: "#pipeline_assetlist_0 > li:nth-child({res}) > div > div > a", // title + href
    cnbc_page_date: ".datestamp",
    cnbc_text: ".article_body",
    nyt_page_title: "#site-content > div > div > div:nth-child(2) > div > ol > li:nth-child({res}) > div > div > a",
    nyt_page_link: "#site-content > div > div > div:nth-child(2) > div > ol > li:nth-child({res}) > div > div > a",
    nyt_text: "#story",
    nyt_date: ".dateline",
    nyt_results:"#site-content > div > div > div:nth-child(2) > div.css-46b038 > ol > li"
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
        var plimit = 100
            for (let pagenum = 1; pagenum <= plimit; ++pagenum){
                // navigate to page
                await Page.navigate({url: "https://cnbc.com/finance/?page="+pagenum});
                await Page.loadEventFired();

                // check for page validity
                amount_results = await Runtime.evaluate({
                    expression: "document.querySelectorAll('#pipeline_assetlist_0 > li > div > div > a').length"
                })        
                amount = amount_results.result.value

                if(amount > 0){
                    for(let i = 1; i <= amount; ++i){
                        // crawl operations
                        article_title_res = await Runtime.evaluate({
                            expression: "document.querySelector('"+selector.cnbc_page_sel.replace("{res}", i)+"').innerText"
                        })
                        article_link_res = await Runtime.evaluate({
                            expression: "document.querySelector('"+selector.cnbc_page_sel.replace("{res}", i)+"').href"
                        })
                            
                        article_title = article_title_res.result.value
                        article_link = article_link_res.result.value
                    }
                        if(article_link_res.result.value != undefined){

                        //     // navigate to page
                            await Page.navigate({url: article_link});
                            await Page.loadEventFired(); 


                            article_text_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.cnbc_text+"').innerText"
                            })
                            
                            article_text = article_text_res.result.value


                            article_date_res = await Runtime.evaluate({
                                expression: "document.querySelector('"+selector.cnbc_page_date+"').innerText"
                            })

                            article_date = article_date_res.result.value.replace("Published ", "")
                            console.log(article_date)

                        //     request.post({
                        //         headers: {'content-type' : 'application/json'},
                        //         'auth': {
                        //             'user': 'admin',
                        //             'pass': 'changeit',
                        //         },
                        //         url: 'http://172.26.89.53:8080/db/news',
                        //         body: JSON.stringify({
                        //             title: article_title,
                        //             link: article_link,
                        //             date: article_date,
                        //             text: article_text
                        //         })
                        //         }, function(error, response, body){
                        //             console.log(body);
                        //     });
                        // }
                        // }
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