// Load Wi-Fi library
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Ping.h>

// Wifi network credentials
const char* ssid = "yourwifissid";
const char* password = "yourwifipassword";

// set the pin
const int RELAYPIN = 2;

// MQTT Broker
const char *mqtt_broker = "192.168.x.x"; // IP of the mqtt broker
const char *topic = "garage/garagedeur/switch";
const char *mqtt_username = "zigbee";
const char *mqtt_password = "yourmqttpassword";
const int mqtt_port = 1883;
int looptimes = 1;

IPAddress gateway(-1, -1, -1, -1); // IP adress to ping for checking the connection
int pingfailures = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  // Initialize the output pin
  pinMode(RELAYPIN, OUTPUT);
  // Set outputs to LOW
  digitalWrite(RELAYPIN, LOW);

  //setup wifi network
  Serial.println("setting up wifi...");
  setup_wifi();
  Serial.println("finished setting up wifi");

  Serial.println("setting up mqtt connection...");
  connect_mqtt();
  Serial.println("finished setting up mqtt connection");

  Serial.printf("Subscribing to topic %s\n", topic);
  client.subscribe(topic);
}

void connect_mqtt() {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(mqtt_subscriber_callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    client.publish("garage/garagedeur/init", "ON");
}

void mqtt_subscriber_callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();

    Serial.println("Sending HIGH signal to relay");
    digitalWrite(RELAYPIN, HIGH);
    delay(400);
    digitalWrite(RELAYPIN, LOW);
    Serial.println("Sending LOW signal to relay");
    Serial.println("-----------------------");
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network

  // Configures static IP address
  // network configuration
  // IPAddress local_IP(192, 168, 179, 20);
  //

  // IPAddress subnet(255, 255, 255, 0);
  // IPAddress primaryDNS(192, 168, 179, 254);
  // IPAddress secondaryDNS(8, 8, 8, 8);

  // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (looptimes % 500000 == 0) {
    Serial.println("publishing health message to mqtt");
    String strLocalIp = WiFi.localIP().toString();
  const char* cstrIp = strLocalIp.c_str();
    client.publish("garage/garagedeur/health", cstrIp);
    looptimes = 0;
  }
  if (looptimes % 50000 == 0) {
    bool success = Ping.ping(gateway, 1);
    if (success) {
      Serial.println("ping");
      pingfailures = 0;
    }
    else {
      Serial.println("Error! No connection to gateway");
      pingfailures++;

      if (pingfailures == 10) {
        ESP.restart();
      }
    }
  }

  client.loop();
  looptimes++;
}
