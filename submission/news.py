# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_news = "http://172.26.89.53:8080/db/news"


def rh_get_news(title):
    r = requests.get(rh_http_news+"?filter={'title':\'"+title+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_get_newsall():
    r = requests.get(rh_http_news, auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text
    
    

#uses defualt  values
def rh_new_news(title, content, date, link):
    data_json = {
        "title": title,
        "content": content,
        "date": date,
        "link": link
    }
    r = requests.post(rh_http_news, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    return r.text

def rh_edit_news(title, content, date, link):
    data_json = {
        "title": title,
        "content": content,
        "date": date,
        "link": link
    }
    r = requests.put(rh_http_news+"/"+rh_get_news_id(title), auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    
    return r.text

def rh_get_news_id(title):
    r = requests.get(rh_http_news+"?filter={'title':\'"+title+"\'}", auth=('admin', 'changeit'), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    jsont = json.loads(r.text)["_embedded"][0]
    return jsont["_id"]["$oid"]

class news(object):
    def on_get(self, req, res):
        title = req.params["title"]
  
        list = json.loads(rh_get_news(title))["_embedded"][0]

        res.status = falcon.HTTP_200

        res.body = json.dumps(list)

    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        title = req.params["title"]
        content = req_json["content"]
        date = req_json["date"]
        link = req_json["link"]

        rh_new_news(title, content, date, link)

        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "created": True
        })


    def on_put(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        title = req.params["title"]
        content = req_json["content"]
        date = req_json["date"]
        link = req_json["link"]

        rh_edit_news(title, content, date, link)
        res.status = falcon.HTTP_200

        res.body = json.dumps({
            "edited": True
        })
    
class newsall(object):
    def on_get(self, req, res):
        list = json.loads(rh_get_newsall())["_embedded"]

        res.status = falcon.HTTP_200

        res.body = json.dumps(list)


app = falcon.API()


app.add_route('/news', news())
app.add_route('/newsall', news())
