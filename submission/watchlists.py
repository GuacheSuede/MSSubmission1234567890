# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_watchlists = "http://172.26.89.53:8080/db/watchlists"


def rh_get_watchlist(name):
    r = requests.get(rh_http_watchlists+"?filter={'name':\'"+name+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    

#uses defualt  values
def rh_new_watchlist(name, username, stocks):
    data_json = {
        "username": username,
        "name": name,
        "stocks": stocks
    }
    r = requests.post(rh_http_watchlists, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_watchlist(name, username, stocks):
    data_json = {
        "username": username,
        "name": name,
        "stocks": stocks
    }
    r = requests.put(rh_http_watchlists+"/"+rh_get_watchlist_id(name), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_watchlist_id(name):
    r = requests.get(rh_http_watchlists+"?filter={'name':\'"+name+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]

class watchlist(object):
    def on_get(self, req, res):
        name = req.params["name"]
  
        list = json.loads(rh_get_watchlist(name))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(list)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        name = req_json["name"]
        username = req_json["username"]
        stocks = req_json["stocks"]

        rh_new_watchlist(name, username, stocks)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        name = req_json["name"]
        username = req_json["username"]
        stocks = req_json["stocks"]
      
        rh_edit_watchlist(name, username, stocks)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })
        

app = falcon.API()


app.add_route('/watchlists', watchlist())
