'use strict';
var express = require('express');
var app = express();

//const app = require('express')();
//const http = require('http').createServer(app);
const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://192.168.0.200');
var _count = 0;

function forward(){
    client.publish('openone/pitch', '{"speed": 65535, "mode":"CW"}')
    console.log('sending forward')
    _count++;
    if(_count < 5){
        setTimeout(reverse, 2000);
    } else {
        stop();
    }
}

function reverse(){
    client.publish('openone/pitch', '{"speed": 65535, "mode":"CCW"}')
    console.log('sending reverse')
    setTimeout(forward, 2000);
}


function stop(){
    client.publish('openone/pitch', '{"speed": 0, "mode":"COAST"}')
    console.log('sending pitch stop')    
}


client.on('connect', () => {
   // Inform controllers that garage is connected
   console.log('connected to mqtt server');
   
})

console.log('Server-side code running');

app.use(express.static(`${__dirname}/public`));

app.get('/time', function(req, res){
    res.send((new Date()).toLocaleTimeString());
});

app.get('/stop', function(req, res){
    console.log("stopping motors");
    client.publish('openone/pitch', '{"speed": 0, "mode":"COAST"}');
    client.publish('openone/roll', '{"speed": 0, "mode":"COAST"}');
    res.send("");
});

app.get('/increase_pitch', function(req, res){
    console.log("increasing pitch");
    client.publish('openone/pitch', '{"speed": 65535, "mode":"CW"}');
    res.send("");
});

app.get('/decrease_pitch', function(req, res){
    console.log("decreasing pitch");
    client.publish('openone/pitch', '{"speed": 65535, "mode":"CCW"}');
    res.send("");
});

app.get('/increase_roll', function(req, res){
    console.log("increasing roll");
    client.publish('openone/roll', '{"speed": 65535, "mode":"CW"}');
    res.send("");
});
app.get('/decrease_roll', function(req, res){
    console.log("decreasing roll");
    client.publish('openone/roll', '{"speed": 65535, "mode":"CCW"}');
    res.send("");
});


app.listen(80);

