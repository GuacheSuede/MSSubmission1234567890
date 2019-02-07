# things.py

# Let's get this party started!
import falcon
import json
import base64
import requests
import uuid
import jwt
import datetime

rh_http_logs = "http://172.26.89.53:8080/db/logs"


def rh_new_log(userid, text, service):
    data_json = {
        "date": str(datetime.datetime.now().date()),
        "time": str(datetime.datetime.now().time()),
        "log": text,
        "service": service,
        "user": userid

    }
    print(json.dumps(data_json))
    r = requests.post(rh_http_logs, auth=('admin', 'changeit'), data=json.dumps(data_json), headers={
        "Content-Type": "application/json",
        "Accept": "application/json"
    })
    

class log(object):
    def on_post(self, req, res):
        req_json = json.loads(req.stream.read().decode("utf-8"))
        userid = req_json["userid"]
        text = req_json["text"]
        service = req_json["service"]


        res.status = falcon.HTTP_200

        rh_new_log(userid, text, service)
        res.body = json.dumps({
            "logged": True
        })

app = falcon.API()


app.add_route('/log', log())
