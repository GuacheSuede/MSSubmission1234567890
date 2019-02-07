const CDP = require('chrome-remote-interface');
const fs = require('fs');

async function sleep() {
  await setTimeout(()=>{}, 5000)
}


async function main() {
    let client;
    try {
        // connect to endpoint
        client = await CDP()

        // extract domains
        const {Page, Runtime, Input} = client


      
        // enable events
        await Page.enable()
        
        // load page
        await Page.navigate({url: 'http://hosted.datascope.reuters.com/'})
        await Page.loadEventFired()

        // login
        await Runtime.evaluate({expression:"document.querySelector('#UserId').value = \"9017045\""})
        await Runtime.evaluate({expression:"document.querySelector('#Password').value = \"gcG0dAVQ\""})
        await Runtime.evaluate({expression:"document.querySelector('#signin-button').click()"})
        await Page.loadEventFired()

        // close modal
        await Runtime.evaluate({expression: "document.querySelector('#sdet-list-missing-reload-button').click()"})

        // go to search page
        await Runtime.evaluate({expression: "document.querySelector('#logo > div').click()"})
        await Runtime.evaluate({expression: "document.querySelector('#search-menu > li > a').click()"})
        await Page.loadEventFired()

         // set search identifier to ticker
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_IdentifierListBox').value = 'TICKER'"})

        // clear all and chekc only needed checkboxes on search page
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_Ctrl_SearchAllTable > tbody > tr:nth-child(2) > td:nth-child(4) > a').click()"})
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_Ctrl_SRCH_ECheckBox').click()"})
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_Ctrl_SRCH_PCheckBox').click()"})
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_Ctrl_SRCH_FCheckBox').click()"})

        // set results limit to max (5000)
        await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_LimitResultsListBox').value = '5000'"})
                
                // enter instrument to field and search
                await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_SRCH_IdentifierTextBox').value = 'GOOG'"})
                await Runtime.evaluate({expression: "document.querySelector('#SRCH_SearchButton').click()"})
                await Page.loadEventFired()

                // expands full list        
                await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_m_DataGrid_Link_ShowAll').click()"}) 
                // await sleep() 
                var links = await Runtime.evaluate({expression: "var links = [];var nodes = document.querySelectorAll('td:nth-child(2) > .tdl > a');for (let v of nodes){links.push(v.href);}JSON.stringify(links)"})              
                console.log(links)

                // selects all instruments and add         
                // for(var j = 0; j< 9; ++j){
                //         console.log(await Runtime.evaluate({expression: "var all_click = document.querySelector('tr > td:nth-child(1) > input');if ( all_click.checked != true){ all_click.click(); }"})  )
                // } 
                // await sleep()
                //  await Runtime.evaluate({expression: "document.querySelector('#addtolist').click()"})  
                // await Page.loadEventFired()


                // // // add to PersonalExchange
                // await Runtime.evaluate({expression: "document.querySelector('#list-manager-wrapper > section > div.ms-columns > div:nth-child(1) > div.inner-wrapper > ul > li:nth-child(6)').click()"}) 
                // await Runtime.evaluate({expression: "document.querySelector('#list-manager-wrapper > footer > div > div.span6.pull-right > button').click()"})                                 
                // await sleep()
                // await Runtime.evaluate({expression: "document.querySelector('#list-manager-wrapper > footer > div > div > button').click()"})   


                                // var links = await Runtime.evaluate({expression: "var links = [];var nodes = document.querySelectorAll('td:nth-child(2) > .tdl > a');for (let v of nodes){links.push(v.href);}links"})              
                // console.log(links)
                // await sleep()
                // console.log(await Runtime.evaluate({expression: "document.querySelector('#ctl00_DSS_MainContent_SRCH_Page_m_DataGrid_DG_Table_ctl01_CBX_CheckAll').click()"}))


                // await Input.dispatchMouseEvent({type: "mousePressed", x: 834, y: 22});



                // console.log(await Runtime.evaluate({expression: "var nodes = document.querySelectorAll('.tdc > input[type=\"checkbox\"]'); for (let v of nodes){ v.click();}"}))                
               
                              

        const {data} = await Page.captureScreenshot()
        fs.writeFileSync('scrot.png', Buffer.from(data, 'base64'))

    } catch (err) {
        console.error(err);
    } finally {
        if (client) {
            await client.close();
        }
    }
}

main();
