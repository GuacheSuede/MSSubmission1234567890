# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_calendermarkers = "http://172.26.89.53:8080/db/calendermarkers"


def rh_get_calendermarkers(username):
    r = requests.get(rh_http_calendermarkers+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

    #no cllot change for now, green, blue yellow, red
    
def rh_get_calendermarkers_id(username, color):
    r = requests.get(rh_http_calendermarkers+"?filter={'username':\'"+username+"\', 'color':\'"+color+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })

    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
# // one level section stylem, no segmentationj of pages
def rh_new_calendermarkers(username, color, description):
    data_json = {
        "username": username,
        "color": color,
        "description": description
    }
    r = requests.post(rh_http_calendermarkers, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_calendermarkers(username, color, description):
    data_json = {
        "username": username,
        "color": color,
        "description": description
    }
    r = requests.put(rh_http_calendermarkers+"/"+rh_get_calendermarkers_id(username, color), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text


class calendermarkers(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        color = req_json["color"]
        description = req_json["description"]

        rh_new_calendermarkers(username, color, description)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })

    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        color = req_json["color"]
        description = req_json["description"]
        
        rh_edit_calendermarkers(username, color, description)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })

    def on_get(self, req, res):
        username = req.params["username"]
  
        info = json.loads(rh_get_calendermarkers(username))["_embedded"] # returns multiple
        res.status = falcon.HTTP_200
        res.body = json.dumps(info)


app = falcon.API()


app.add_route('/calendermarkers', calendermarkers())
