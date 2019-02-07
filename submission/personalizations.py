# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_personalizations = "http://172.26.89.53:8080/db/personalizations"


def rh_get_personalization(username):
    r = requests.get(rh_http_personalizations+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    

#uses defualt  values
def rh_new_personalization(username):
    data_json = {
        "username": username,
        "theme": "white",
        "main_font": "lato",
        "code_font": "consolas",
        "pointer": "cursor",
        "alarm": "beep"
    }
    r = requests.post(rh_http_personalizations, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_personalization(username, theme, main_font, code_font, pointer, alarm):
    data_json = {
        "username": username,
        "theme": theme,
        "main_font": main_font,
        "code_font": code_font,
        "pointer": pointer,
        "alarm": alarm
    }
    r = requests.put(rh_http_personalizations+"/"+rh_get_personalization_id(username), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_personalization_id(username):
    r = requests.get(rh_http_personalizations+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]

class personalization(object):
    def on_get(self, req, res):
        username = req.params["username"]
  
        user = json.loads(rh_get_personalization(username))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(user)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]

        rh_new_personalization(username)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        theme = req_json["theme"]
        main_font = req_json["main_font"]
        code_font = req_json["code_font"]
        pointer = req_json["pointer"]
        alarm = req_json["alarm"]
      
        rh_edit_personalization(username, theme, main_font, code_font, pointer, alarm)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })
        

app = falcon.API()


app.add_route('/personalizations', personalization())
