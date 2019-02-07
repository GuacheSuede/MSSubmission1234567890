var WebSocketServer = require('websocket').server;
var WebSocketClient = require('websocket').client;
var WebSocketFrame  = require('websocket').frame;
var WebSocketRouter = require('websocket').router;
var W3CWebSocket = require('websocket').w3cwebsocket;
const fetch = require('node-fetch');
var WebSocketServer = require('websocket').server;
var http = require('http');
global.btoa = function (str) {return new Buffer(str).toString('base64');};

const express = require('express')
const app = express()
const port = 3000

var port_start = 29170

app.get('/', (req, res) => {
    req.setTimeout(0) // no timeout
    // will nto end sefrver as msot liekly will run forever
    ++port_start;
    res.send(port_start.toString());
    var url = "http://127.0.0.1:8080/db/prices?pagesize=1&sort_by=datetime&filter={\"ticker\": \"";
    var url_end = "\"}";
   var code_string = `
            


var prices = [];
var price_page = 0;
function price(ticker){
    var r_obj = {};

    fetch(
        url + ticker + url_end + "&page="+price_page, {
            method: 'get',
            headers:{
                'Authorization': 'Basic '+btoa('admin:changeit'), 
                'Content-Type': 'application/json'
            }
        }).then(response => {
            return response.json();
            }).then(data => obj[ticker] = data["_embedded"][0]["price"])

    return r_obj
        
}
 
var server = http.createServer(function(request, response) {response.writeHead(404);response.end();});
server.listen(port_start, function() {});

wsServer = new WebSocketServer({httpServer: server,autoAcceptConnections: false});
               
wsServer.on('request', function(request) {
    
    
    var connection = request.accept('echo-protocol', request.origin);
    console.log((new Date()) + ' Connection accepted.');
    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            ++price_page;
            connection.send(price('AAPL'))
            console.log('Received Message: ' + message.utf8Data);
            connection.sendUTF(message.utf8Data);
        }
        
    });
    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});
        `
        eval(code_string)

        // /uise time stamps to ensure will not convflcit
})

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
