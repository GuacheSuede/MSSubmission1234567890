
from autobahn.twisted.websocket import WebSocketServerProtocol, \
    WebSocketServerFactory
import time
import json

class PriceWS(WebSocketServerProtocol):
    def __init__(self):
                # 26 ema
        self.sma26 = 0 
        self.ema26 = 0

        self.daynum = 0
        self.ema26total = 0
        
        self.ema26factor = 2/(26+1)
    
    def algo_ema(self, value):


        # ema26 section
        if self.daynum < 26:
            self.ema26total += value
        elif self.daynum == 26:
            self.ema26total += value
            self.sma26 = self.ema26total/26
            self.ema26 = (value - self.sma26) * self.ema26factor + self.sma26
        elif self.daynum > 26:
            self.ema26 = (value - self.ema26) * self.ema26factor + self.ema26

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
            self.algo_ema(mesg_json["value"])
            self.sendMessage(str(self.ema26).encode(), isBinary)
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