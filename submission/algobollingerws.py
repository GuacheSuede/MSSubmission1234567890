
from autobahn.twisted.websocket import WebSocketServerProtocol, \
    WebSocketServerFactory
import time
import json
from math import sqrt

class PriceWS(WebSocketServerProtocol):
    def __init__(self):
        self.sddaynum = 0
        self.smadaynum = 0
        self.smaret = 0
        self.sma20totallist = []
        self.sdret = 0
        self.sd20totallist = []
        

    def algo_sma_20(self, value):
        # 20 sma
        
                
            # sma20 section
        if self.smadaynum < 20:
            self.sma20totallist.append(value)
        elif self.smadaynum == 20:
            self.sma20totallist.append(value)
            sma20total = 0
            for xx in range(20):
                sma20total+=self.sma20totallist[xx]
            self.smaret = sma20total/20
        elif self.smadaynum > 20:
            del self.sma20totallist[0]
            self.sma20totallist.append(value)
            sma20total = 0
            for xx in range(20):
                sma20total+=self.sma20totallist[xx]
            self.smaret = sma20total/20


        self.smadaynum += 1

        
    
    def algo_sd_20(self, value):
        # 20 sd

                
        # sd20 section
        if self.sddaynum < 20:
            self.sd20totallist.append(value)
        elif self.sddaynum == 20:
            self.sd20totallist.append(value)
            s20total = 0
            for xx in range(20):
                s20total+=self.sd20totallist[xx]
            s20mean  = s20total/20

            sd20total = 0
            for xx in range(20):
                sd20total+= pow(self.sd20totallist[xx] - s20mean, 2)

            self.sdret = sqrt(sd20total / 20)

        elif self.sddaynum > 20:
            del self.sd20totallist[0]
            self.sd20totallist.append(value)
            s20total = 0
            for xx in range(20):
                s20total+=self.sd20totallist[xx]
            s20mean  = s20total/20

            sd20total = 0
            for xx in range(20):
                sd20total+= pow(self.sd20totallist[xx] - s20mean, 2)

            self.sdret = sqrt(sd20total / 20)


        self.sddaynum += 1

    def algo_bollinger(self, value):
        self.bolret = {}
        
        self.algo_sma_20(value)
        self.algo_sd_20(value)

        self.bolret["mid"] = self.smaret
        self.bolret["up"] = self.smaret + (self.sdret * 2)
        self.bolret["dw"] = self.smaret - (self.sdret * 2)
       



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
            self.algo_bollinger(int(mesg_json["value"]))
            # print(json.dumps(json.loads(self.bolret)).encode())
            self.sendMessage(json.dumps((self.bolret)).encode(), isBinary)
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