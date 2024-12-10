import sys
import os
import time
from datetime import datetime

import requests
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    topics = [("garage/garagedeur/init", 1), ("garage/garagedeur/health", 1)]
    client.subscribe(topics)

def on_message(client, userdata, msg):
    topic = msg.topic
    if topic == "garage/garagedeur/health":
        global last_health_message
        last_health_message = datetime.now()
    elif topic == "garage/garagedeur/init":
        global last_init_message
        last_init_message = datetime.now()
    payload = msg.payload.decode("utf-8")
    health_message = f"Last health message received at {last_health_message.strftime('%d-%m-%y %H:%M:%S')} from {payload}. "
    init_message = f"Last init message received at {last_init_message.strftime('%d-%m-%y %H:%M:%S')}. " if last_init_message is not None else "Last init message was never received. "
    forced_reset_message = f"Last forced reset was at {last_forced_reset.strftime('%d-%m-%y %H:%M:%S')}. " if last_forced_reset is not None else "Last forced reset was never sent. "
    print(f"{health_message}{init_message}{forced_reset_message}")

def reset_switch(ha_token):
    # send a shutdown signal to home assistant
    headers = {
        "Authorization": f"Bearer {ha_token}",
        "Content-Type": "application/json"
    }

    # turn plug off
    data = {
        "entity_id": "automation.kerstverlichting_uit_ochtend"
    }
    # response = requests.post("https://ha.nijboer.frl/api/services/automation/trigger", headers=headers, json=data)
    # if response.status_code != 200:
    #     print(f"Error turning off power plug: {response}")

    # turn plug back on
    data = {
        "entity_id": "automation.kerstverlichting_aan_ochtend"
    }
    time.sleep(1)
    # response = requests.post("https://ha.nijboer.frl/api/services/automation/trigger", headers=headers, json=data)
    # if response.status_code != 200:
    #     print(f"Error turning off power plug: {response}")

    print("waiting for ESP to restart")
    time.sleep(10)
    print("resuming normal operation")

def get_required_env_variable(envvar):
    """Get the required environment variable."""
    if os.environ.get(envvar) is None:
        print(f"{envvar} is not set")
        sys.exit(1)
    return os.environ.get(envvar)

# get required environment variables
mqtt_password = get_required_env_variable("MQTT_PASSWORD")
homeassistant_token = get_required_env_variable("HA_TOKEN")

# initialize date variables
last_health_message = datetime.now()
last_init_message = None
last_forced_reset = None

# Initialize MQTT client
mqttclient = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttclient.on_connect = on_connect
mqttclient.on_message = on_message

# Set username and password
mqttclient.username_pw_set("zigbee", mqtt_password)

mqttclient.connect("192.168.179.99", 1883, 60)

mqttclient.loop_start()

while True:
    time.sleep(1)
    # if the last_health_time is more than 1 minute ago, print a warning
    if (datetime.now() - last_health_message).total_seconds() > 60:
        print("WARNING: No health message received for more than 1 minute")
        last_forced_reset = datetime.now()
        reset_switch(homeassistant_token)
