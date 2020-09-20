import paho.mqtt.client as mqtt
import time
import datetime


#DeviceID = "00D2DC93" # original stelpro 3V3
#DeviceID = "00D2DC93" # test maison
#DeviceID = "00D30A3A" # stelpro 12V
connected = False

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    global connected
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    #client.subscribe(DeviceID + "_from_server/#") #
    client.subscribe("opensolar/#")
    connected = True

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(" ************** ON MESSAGE ********************")
    s = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + " topic:" + msg.topic + " payload:" + msg.payload
    fname = "log_" + DeviceID + ".txt"
    with open(fname, "a") as f:
        f.write(s + "\n")
    print(s)

def on_disconnect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    print(userdata)
    
client = mqtt.Client("latitude_01344")
#client.username_pw_set("", password="")
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.0.200", 1883, 15)
client.loop_start()
while not connected:
    time.sleep(0.5)
i = 0
while(True):
    payload = "technolaf " + str(i)
    topic = "opensolar_todevice"
    print("publishing payload:" + payload + " topic:" + topic)
    client.publish(topic,payload, 0)
    i += 1
    time.sleep(10.0)
client.loop_stop()    

