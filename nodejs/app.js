'use strict';
var express = require('express');
var app = express();
const mqtt = require('mqtt');
const url = require('url')
const client = mqtt.connect('mqtt://technolaf.com');
var _count = 0;
var motor_speed = 32768;
var deviceID = 0
var pitch_reversed = false
var roll_reversed = false

client.on('connect', () => {
   // Inform controllers that garage is connected
   console.log('connected to mqtt server');
   
})

console.log('Server-side code running');

app.use(express.static(`${__dirname}/public`));


app.get('/time', function(req, res){
    res.send("DeviceID:" + deviceID + "<br>" + (new Date()).toLocaleTimeString());
});

app.get('/stop', function(req, res){
    console.log("stopping motors");
    client.publish('openone/' + deviceID + '/pitch', '{"speed": 0, "mode":"COAST"}');
    client.publish('openone/' + deviceID + '/roll', '{"speed": 0, "mode":"COAST"}');
    res.send("");
});

app.get('/increase_pitch', function(req, res){
    console.log("increasing pitch");
    if(pitch_reversed){
        client.publish('openone/' + deviceID + '/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    } else {
        client.publish('openone/' + deviceID + '/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    }
    res.send("");
});

app.get('/decrease_pitch', function(req, res){
    console.log("decreasing pitch");
    if(pitch_reversed){
        client.publish('openone/' + deviceID + '/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    } else {
        client.publish('openone/' + deviceID + '/pitch', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    }
    res.send("");
});

app.get('/increase_roll', function(req, res){
    console.log("increasing roll");
    if(roll_reversed){
        client.publish('openone/' + deviceID + '/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    } else {
        client.publish('openone/' + deviceID + '/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    }
    res.send("");
});
app.get('/decrease_roll', function(req, res){
    console.log("decreasing roll");
    if(roll_reversed){
        client.publish('openone/' + deviceID + '/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CW"}');
    } else {
        client.publish('openone/' + deviceID + '/roll', '{"speed": ' + motor_speed.toString() + ', "mode":"CCW"}');
    }
    res.send("");
});


app.get('/set_speed', function(req, res){
    console.log("set speed to " + req.query.speed);
    motor_speed = 65535 * req.query.speed / 100;
    res.send("");
});


app.get('/device_id', function(req, res) {
    deviceID = req.query.id;
    console.log("device ID:" + deviceID);
    res.sendFile('public/openone.html', {root: __dirname })
});


app.get('/reverse_pitch', function(req, res) {
    console.log("pitch reversed");
    pitch_reversed = !pitch_reversed;
});
app.get('/reverse_roll', function(req, res) {
    console.log("roll reversed");
    roll_reversed = !roll_reversed;
});





app.listen(80);

