
from autobahn.twisted.websocket import WebSocketServerProtocol, \
    WebSocketServerFactory
import time
import json

class PriceWS(WebSocketServerProtocol):
    def __init__(self):
        self.smaret = 0
        self.sma50totallist = []
        self.daynum = 0
        
    def algo_sma(self, value):
        # 50 sma
   
                
            # sma50 section
        if self.daynum < 50:
            self.sma50totallist.append(value)
        elif self.daynum == 50:
            self.sma50totallist.append(value)
            sma50total = 0
            for xx in range(50):
                sma50total+=self.sma50totallist[xx]
            self.smaret = sma50total/50
        elif self.daynum > 50:
            del self.sma50totallist[0]
            self.sma50totallist.append(value)
            sma50total = 0
            for xx in range(50):
                sma50total+=self.sma50totallist[xx]
            self.smaret = sma50total/50


        self.daynum += 1



    def onConnect(self, request):
        print("Client connecting: {0}".format(request.peer))

    def onOpen(self):
        print("WebSocket connection open.") 
        #routing logic will be handled by client
        

    def onMessage(self, payload, isBinary):
        mesg = payload.decode('utf8')
        mesg_json = json.loads(mesg)
        
        if(mesg_json["command"] == "START"):
            self.sendMessage("ACKED".encode(), isBinary)
        elif(mesg_json["command"] == "STEP"):
            self.algo_sma(mesg_json["value"])
            self.sendMessage(str(self.smaret).encode(), isBinary)
        else:
            self.sendMessage("INVALID".encode(), isBinary)

    def onClose(self, wasClean, code, reason):
        print("WebSocket connection closed: {0}".format(reason))


if __name__ == '__main__':

    import sys

    from twisted.python import log
    from twisted.internet import reactor

    log.startLogging(sys.stdout)

    factory = WebSocketServerFactory(u"ws://127.0.0.1:9000")
    factory.protocol = PriceWS

    reactor.listenTCP(9000, factory)
    reactor.run()