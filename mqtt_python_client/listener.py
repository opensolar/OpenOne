import paho.mqtt.client as mqtt
import time
import datetime


#DeviceID = "00D2DC93" # original stelpro 3V3
#DeviceID = "00D2DC93" # test maison
#DeviceID = "00D30A3A" # stelpro 12V

BROKER = "192.168.0.200"
#BROKER = "test.mosquitto.org"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " +str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    #client.subscribe(DeviceID + "_from_device/#") #
    
    client.subscribe("opensolar") 


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    s = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + " topic:" + msg.topic + " payload:" + msg.payload.decode()
    fname = "log_opensolar.txt"
    with open(fname, "a") as f:
        f.write(s + "\n")
    print(s)

client = mqtt.Client("laptop_test")

client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, 1883, 15)
#client.subscribe("opensolar")

client.loop_start()
while(True):
    time.sleep(10.0)
#client.loop_stop()    

