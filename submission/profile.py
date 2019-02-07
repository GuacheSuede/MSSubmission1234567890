# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_profile = "http://172.26.89.53:8080/db/profile"


def rh_get_profile(username):
    r = requests.get(rh_http_profile+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    

#uses defualt  values
def rh_new_profile(username, name, phone, email, address):
    data_json = {
        "username": username,
        "name": name,
        "phone": phone,
        "email": email,
        "address": address
    }
    r = requests.post(rh_http_profile, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_profile(username, name, phone, email, address):
    data_json = {
        "username": username,
        "name": name,
        "phone": phone,
        "email": email,
        "address": address
    }
    r = requests.put(rh_http_profile+"/"+rh_get_profile_id(username), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_profile_id(username):
    r = requests.get(rh_http_profile+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]

class profile(object):
    def on_get(self, req, res):
        username = req.params["username"]
  
        user = json.loads(rh_get_profile(username))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(user)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]

        rh_new_profile(username)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]  
        name = req_json["name"]
        phone = req_json["phone"]
        email = req_json["email"]
        address = req_json["address"]
        alarm = req_json["alarm"]
      
        rh_edit_profile(username, name, phone, email, address)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })
        

app = falcon.API()


app.add_route('/profile', profile())
