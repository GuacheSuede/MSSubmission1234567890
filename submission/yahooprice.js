const CDP = require('chrome-remote-interface');


var alphabets = " abcdefghijklmnopqrstuvwxyz" // for each and times 3 inceemnmtally in nyt search



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
                            price_res = await Runtime.evaluate({
                                expression: "document.querySelector('#quote-header-info > div > div > div:nth-child(1) > span').innerText"
                            })
                            price = price_res.result.value

                            // DB Ops
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