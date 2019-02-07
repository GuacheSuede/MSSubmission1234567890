# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_versions = "http://172.26.89.53:8080/db/versions"


def rh_get_version(vhash):
    r = requests.get(rh_http_versions+"?filter={'vhash':\'"+vhash+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    

def rh_get_versions(parent):
    r = requests.get(rh_http_versions+"?filter={'parent':\'"+parent+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

#uses defualt  values
def rh_new_version(vhash, parent, username, code, date, time):
    data_json = {
        "vhash": vhash,
        "parent": parent,
        "username": username,
        "code": code,
        "date": date,
        "time": time
    }
    r = requests.post(rh_http_versions, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_version(vhash, parent, username, code, date, time):
    data_json = {
        "vhash": vhash,
        "parent": parent,
        "username": username,
        "code": code,
        "date": date,
        "time": time
    }
    r = requests.put(rh_http_versions+"/"+rh_get_version_id(vhash), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_version_id(vhash):
    r = requests.get(rh_http_versions+"?filter={'vhash':\'"+vhash+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]

class versions(object):
    def on_get(self, req, res):
        vhash = req.params["vhash"]
  
        list = json.loads(rh_get_version(vhash))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(list)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        vhash = req_json["vhash"]
        parent = req_json["parent"]
        username = req_json["username"]
        code = req_json["code"]
        date = req_json["date"]
        time = req_json["time"]

        rh_new_version(vhash, parent, username, code, date, time)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        vhash = req_json["vhash"]
        parent = req_json["parent"]
        username = req_json["username"]
        code = req_json["code"]
        date = req_json["date"]
        time = req_json["time"]
      
        rh_edit_version(vhash, parent, username, code, date, time)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })
        

app = falcon.API()


app.add_route('/versions', versions())
