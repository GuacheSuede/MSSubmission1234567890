
from autobahn.twisted.websocket import WebSocketServerProtocol, \
    WebSocketServerFactory
import time
import json

class PriceWS(WebSocketServerProtocol):
    def __init__(self):
        self.rsiret = 0

        self.avgain = 0
        self.avloss = 0

        self.prevval = 0
        self.ploss = 0

        self.daynum = 0

        # used for first 14 days only
        self.sumgain = 0
        self.sumloss = 0        


    def algo_rsi(self, value):
        if self.daynum < 14:
            if self.daynum == 0:
                self.prevval = value
                self.sumgain += value
                self.daynum += 1
                # for now still ignore rsi.
            else: 
                self.prevval = value - self.prevval
                if (self.prevval < 0): #loss
                    self.sumloss += (self.prevval * -1) #ngative here
                else: # gain
                    self.sumgain += self.prevval
                self.daynum += 1
        elif self.daynum == 14:
            self.prevval = value - self.prevval
            if (self.prevval < 0): #loss
                self.sumloss += (self.prevval * -1) #unngative here
            else: # gain
                self.sumgain += self.prevval

            self.avgain = self.sumgain / 14
            self.avloss = self.sumloss / 14

            if self.avloss == 0:
                self.rsiret = 100 - (100/(1+(self.avgain/1)))
            else:
                self.rsiret = 100 - (100/(1+(self.avgain/self.avloss)))

            self.daynum += 1
        elif self.daynum > 14:
            self.prevval = value - self.prevval
            if (self.prevval < 0): #loss
                self.avgain = ((self.avgain * 13) + 0 )/14
                self.avloss = ((self.avloss * 13) + (self.prevval * -1))/14
            else: # gain
                self.avgain = ((self.avgain * 13) + self.prevval)/14
                self.avloss = ((self.avloss * 13) + 0)/14
            if self.avloss == 0:
                self.rsiret = 100 - (100/(1+(self.avgain/1)))
            else:
                self.rsiret = 100 - (100/(1+(self.avgain/self.avloss)))
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
            self.algo_rsi(mesg_json["value"])
            self.sendMessage(str(self.rsiret).encode(), isBinary)

           

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