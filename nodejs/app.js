'use strict';
var express = require('express');
var app = express();
const mqtt = require('mqtt');
const url = require('url')
const client = mqtt.connect('mqtt://192.168.0.200');
var _count = 0;
var motor_speed = 32768;

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
    client.publish('openone/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    res.send("");
});

app.get('/decrease_pitch', function(req, res){
    console.log("decreasing pitch");
    client.publish('openone/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    res.send("");
});

app.get('/increase_roll', function(req, res){
    console.log("increasing roll");
    client.publish('openone/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    res.send("");
});
app.get('/decrease_roll', function(req, res){
    console.log("decreasing roll");
    client.publish('openone/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    res.send("");
});


app.get('/set_speed', function(req, res){
    
    console.log("set speed to " + req.query.speed);
    motor_speed = 65535 * req.query.speed / 100;
    res.send("");
});


app.listen(80);

