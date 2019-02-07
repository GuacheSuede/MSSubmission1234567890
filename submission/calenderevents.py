# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_calenderevents = "http://172.26.89.53:8080/db/calenderevents"


def rh_get_calenderevent(username):
    r = requests.get(rh_http_calenderevents+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    

    
def rh_get_calenderevent_id(name):
    r = requests.get(rh_http_calenderevents+"?filter={'name':\'"+name+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
# // one level section stylem, no segmentationj of pages
def rh_new_calenderevent(name, description, marker, date, time):
    data_json = {
        "name": name,
        "description": description,
        "marker": marker,
        "date": date,
        "time": time
    }
    r = requests.post(rh_http_calenderevents, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_calenderevent(name, description, marker, date, time):
    data_json = {
        "name": name,
        "description": description,
        "marker": marker,
        "date": date,
        "time": time
    }
    r = requests.put(rh_http_calenderevents+"/"+rh_get_calenderevent_id(name), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text


# implment jwt check
# rmeeber to d0uble check routes
#today finish up routes incdluign algo service
# tm finish up elasticsearc + chrome
# the day afte deployment on kubernetes 
class calenderevents(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        name = req_json["name"]
        description = req_json["description"]
        marker = req_json["marker"]
        date = req_json["date"]
        time = req_json["time"]

        rh_new_calenderevent(name, description, marker, date, time)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })

    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        name = req_json["name"]
        description = req_json["description"]
        marker = req_json["marker"]
        date = req_json["date"]
        time = req_json["time"]

        rh_edit_calenderevent(name, description, marker, date, time)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })

    def on_get(self, req, res):
        username = req.params["username"]
  
        # info = json.loads(rh_get_calenderevent(name))["_embedded"][0]
        res.status = falcon.HTTP_200
        res.body = json.dumps(json.loads(rh_get_calenderevent(username))["_embedded"])


app = falcon.API()


app.add_route('/calenderevents', calenderevents())
