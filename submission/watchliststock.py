# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_watchlists_stock = "http://172.26.89.53:8080/db/watchlistsstock"

#ticker
#pricelist
#volumelist

def rh_get_watchliststock(ticker):
    r = requests.get(rh_http_watchlists_stock+"?filter={'ticker':\'"+ ticker +"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
#uses defualt  values
def rh_new_watchliststock(ticker, company):
    data_json = {
        "ticker": ticker,
        "company": company,
        "pricelist": [],
        "volumelist": []
    }
    r = requests.post(rh_http_watchlists_stock, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_watchliststock(ticker, company):
    existing = json.loads(rh_get_watchliststock(ticker))["_embedded"][0]
    existing_pricelist = existing["pricelist"]
    existing_volumelist = existing["volumelist"]
    data_json = {
        "ticker": ticker,
        "company": company,
        "pricelist": existing_pricelist,
        "volumelist": existing_pricelist
    }
    r = requests.put(rh_http_watchlists_stock+"/"+rh_get_watchliststock_id(ticker), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_update_watchliststock_pricevolume(ticker, price, volume):
    existing = json.loads(rh_get_watchliststock(ticker))["_embedded"][0]
    upsert_pricelist = existing["pricelist"]
    upsert_volumelist = existing["volumelist"]
    #####
    #Operations
    
    
    
    
    
    
    #####

    data_json = {
        "ticker":  existing["ticker"],
        "company":  existing["company"],
        "pricelist": upsert_pricelist,
        "volumelist": upsert_volumelist
    }
    r = requests.put(rh_http_watchlists_stock+"/"+rh_get_watchliststock_id(ticker), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_watchliststock_id(ticker):
    r = requests.get(rh_http_watchlists_stock+"?filter={'ticker':\'"+ticker+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]


class watchliststock(object):
    def on_get(self, req, res):
        ticker = req.params["ticker"]
  
        list = json.loads(rh_get_watchliststock(ticker))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(list)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        ticker = req_json["ticker"]
        company = req_json["company"]
        rh_new_watchliststock(ticker, company)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        ticker = req_json["ticker"]
        company = req_json["company"]
      
        rh_edit_watchliststock(ticker, company)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })




# class watchliststock(object):
#     def on_put(self, req, res):
#         req_json = json.loads(req.stream.read().decode("utf-8"))
#         ticker = req_json["ticker"]
#         price = req_json["price"]
#         volume = req_json["volume"]

      
#         rh_update_watchliststock_pricevolume(ticker, price, volume)

#         res.status = falcon.HTTP_200

#         res.body = json.dumps({
#             "edited": True
#         })
    
        #update price,get prices etc.c..
    
        

app = falcon.API()


app.add_route('/watchlistsstock', watchliststock())

# app.add_route('/watchlistsstockup', watchliststockup())

#SET UP CHROME
# #stream websocekt price and volunme
#crawl price and volume
# crawl yahoo financials
# crawl news
# versions crud
# indicators 
# price crud ws
# buy sell client
#educatiron info
#calender curd
#algo controller
#elasticsearch clsuter nad integrationb ?
# KUBE PROXY1 M5