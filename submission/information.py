# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_information = "http://172.26.89.53:8080/db/information"


def rh_get_info(section):
    r = requests.get(rh_http_information+"?filter={'section':\'"+section+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
def rh_get_info_id(section):
    r = requests.get(rh_http_information+"?filter={'section':\'"+section+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
# // one level section stylem, no segmentationj of pages
def rh_new_info(section, content):
    data_json = {
        "section": section,
        "content": content
    }
    r = requests.post(rh_http_information, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_info(section, content):
    data_json = {
        "section": section,
        "content": content
    }
    r = requests.put(rh_http_information+"/"+rh_get_info_id(section), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text


# implment jwt check
# rmeeber to d0uble check routes
#today finish up routes incdluign algo service
# tm finish up elasticsearc + chrome
# the day afte deployment on kubernetes 
class information(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        section = req_json["section"]
        content = req_json["content"]
      
        rh_new_info(section, content)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })

    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        section = req_json["section"]
        content = req_json["content"]
      
        rh_edit_info(section, content)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })

    def on_get(self, req, res):
        section = req.params["section"]
  
        info = json.loads(rh_get_info(section))["_embedded"][0]
        res.status = falcon.HTTP_200
        res.body = json.dumps(info)


app = falcon.API()

#change created to edited


app.add_route('/information', information())
