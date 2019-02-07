# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_education = "http://172.26.89.53:8080/db/education"


def rh_get_edu(method):
    r = requests.get(rh_http_education+"?filter={'method':\'"+method+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

    
def rh_get_eduall():
    r = requests.get(rh_http_education, auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
def rh_get_edu_id(method):
    r = requests.get(rh_http_education+"?filter={'method':\'"+method+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
def rh_new_edu(method, headline, description, example):
    data_json = {
        "method": method,
        "headline": headline,
        "description": description,
        "example": example
    }
    r = requests.post(rh_http_education, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

#condier onyl Id, thogu htis is nto a fool proof btu only pow
def rh_edit_edu(method, headline, description, example):
    data_json = {
        "method": method,
        "headline": headline,
        "description": description,
        "example": example
    }
    r = requests.put(rh_http_education+"/"+rh_get_edu_id(method), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text


# implment jwt check
# rmeeber to d0uble check routes
#today finish up routes incdluign algo service
# tm finish up elasticsearc + chrome
# the day afte deployment on kubernetes 
class educations(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        method = req_json["method"]
        headline = req_json["headline"]
        description = req_json["description"]
        example = req_json["example"]

        rh_new_edu(method, headline, description, example)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })

    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        method = req_json["method"]
        headline = req_json["headline"]
        description = req_json["description"]
        example = req_json["example"]
      
        rh_edit_edu(method, headline, description, example)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })

    def on_get(self, req, res):
        method = req.params["method"]
  
        info = json.loads(rh_get_edu(method))["_embedded"][0]
        res.status = falcon.HTTP_200
        res.body = json.dumps(info)

class educationall(object):
    def on_get(self, req, res):
  
        info = json.loads(rh_get_eduall())["_embedded"]
        res.status = falcon.HTTP_200
        res.body = json.dumps(info)



app = falcon.API()


app.add_route('/education', educations())
app.add_route('/educationall', educationall())
