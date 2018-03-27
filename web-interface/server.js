const express = require('express');
const app = express();
var protobuf = require("protobufjs");
const path = require('path');
var dgram = require('dgram');
var socket = dgram.createSocket('udp4');

var broadcastAddress = '192.168.1.255';
var broadcastPort = 50001;

var message = '';

app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname, '/index.html'));
});


socket.bind(broadcastPort, '0.0.0.0', ()  => {
    socket.setBroadcast(true);
});

socket.on('listening', () => {
  message = 'Listening now.';
  console.log('Listening');
});

socket.on('message', function(msg, rinfo) {
    console.log('Received %d bytes from %s:%d\n',
    msg.length, rinfo.address, rinfo.port);
    console.dir(msg); 

    var j = JSON.stringify(msg);    
    message = msg;
});

app.post('/message', (msg) => {
  socket.send(msg, broadcastPort, broadcastAddress, (err) => {
    socket.close();
  });
});



app.get('/message', function(req, res){
      if(err) console.log('Error in messages');
      console.log(message);
      res.send(message);
});

app.use(express.static(path.join(__dirname, '')));

app.listen(3000, () => { 
  console.log('App running on port 3000') 
});



