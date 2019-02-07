# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_users = "http://172.26.89.53:8080/db/users"
rh_http_usersjwt = "http://172.26.89.53:8080/db/usersjwt"

jtwsecret = "secret2"

def rh_get_user(username):
    r = requests.get(rh_http_users+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
def rh_get_user_id(username):
    r = requests.get(rh_http_users+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
def rh_new_user(username, password, phone, email, address):
    data_json = {
        "username": username,
        "password": base64.b64encode(str.encode(password)).decode("utf-8"),
        "phone": phone,
        "email": email,
        "address": address,
    }
    r = requests.post(rh_http_users, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_user(username, password, phone, email, address):
    data_json = {
        "username": username,
        "password": base64.b64encode(str.encode(password)).decode("utf-8"),
        "phone": phone,
        "email": email,
        "address": address,
    }
    r = requests.put(rh_http_users+"/"+rh_get_user_id(username), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text



def rh_get_userjwt(username):
    r = requests.get(rh_http_usersjwt+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
      
def rh_get_userjwt_id(username):
    r = requests.get(rh_http_usersjwt+"?filter={'username':\'"+username+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]
    
def rh_new_userjwt(username, jwt):
    data_json = {
        "username": username,
        "jwt": jwt
    }
    r = requests.post(rh_http_usersjwt, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
        

def rh_delete_userjwt(username):
    r = requests.delete(rh_http_usersjwt+"/"+rh_get_userjwt_id(username), auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

class user(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        password = req_json["password"]
        phone = req_json["phone"]
        email = req_json["email"]
        address = req_json["address"]

        rh_new_user(username, password, phone, email, address)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })

    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        password = req_json["password"]
        phone = req_json["phone"]
        email = req_json["email"]
        address = req_json["address"]

        rh_new_user(username, password, phone, email, address)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })

    def on_get(self, req, res):
        username = req.params["username"]
  
        user = json.loads(rh_get_user(username))["_embedded"][0]
        res.status = falcon.HTTP_200
        res.body = json.dumps(user)

        

class userauthin(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        password = req_json["password"]
        password = base64.b64encode(str.encode(password)).decode("utf-8") 

        user = json.loads(rh_get_user(username))["_embedded"][0]
        if(user["password"] == password):
            res.status = falcon.HTTP_200
            
            if(len(json.loads(rh_get_userjwt(username)["_embedded"]) == 0)):
                token = jwt.encode({'username': username}, 'secret', algorithm='HS256')
                rh_new_userjwt(username, token)
                res.body = json.dumps({
                    "auth": True,
                    "token": token
                })
            else:
                res.body = json.dumps({
                    "auth": True,
                    "token": "Given"
                })
        else:
            res.status = falcon.HTTP_200

            res.body = json.dumps({
                "auth": False
            })

class userauthout(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        username = req_json["username"]
        password = req_json["password"]        
        password = base64.b64encode(str.encode(password)).decode("utf-8") 


        user = json.loads(rh_get_user(username))["_embedded"][0]
        if(user["password"] == password):
            res.status = falcon.HTTP_200
            
            if(len(json.loads(rh_get_userjwt(username)["_embedded"]) == 0)):
                rh_delete_userjwt(username)
                res.body = json.dumps({
                    "deleted": True,
                    })
            else:
                res.body = json.dumps({
                    "deleted": False
                })
        else:
            res.status = falcon.HTTP_200

            res.body = json.dumps({
                "auth": False
            })

    
app = falcon.API()


app.add_route('/users', user())
app.add_route('/login', userauthin())
app.add_route('/logout', userauthout())
