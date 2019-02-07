
from autobahn.twisted.websocket import WebSocketServerProtocol, \
    WebSocketServerFactory
import time
import json

class PriceWS(WebSocketServerProtocol):

    def onConnect(self, request):
        print("Client connecting: {0}".format(request.peer))

    def onOpen(self):
        self.prices = []
        self.start = "False"

        print("WebSocket connection open.") 
        
        # with open ("aapl.o.csv", "r") as f:
        #     for line in f:
        #         self.prices.append(line)
        #         time.sleep(0.5) # times 2 faster than actual request rate..

    def onMessage(self, payload, isBinary):
        mesg = payload.decode('utf8')
        mesg_json = json.loads(mesg)
        
        if(mesg_json["command"] == "START"):
            f=open(mesg_json["asset"]+".csv")
            # proof of work hence we did not implement a fast db lookup
            self.lines=f.readlines()
            self.lineno = 0
            self.sendMessage("ACKED".encode(), isBinary)
        elif(mesg_json["command"] == "NEXT"):
            self.lineno+=1
            self.sendMessage(self.lines[self.lineno].encode(), isBinary)
        elif(mesg_json["command"] == "RESET"):
            self.lineno=0
            self.sendMessage("RESETED".encode(), isBinary)
            




        # echo back message verbatim
      

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