#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "vaillant.h"
#include "Parameterliste.h"

// Update these with values suitable for your network.

//const char* ssid = "VodafoneSharingDock_CC4810";
const char* ssid = "FRITZ!Box 6490 Cable";
const char* password = "Hanirulesthisnetwork";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial1.println();
  Serial1.print("Connecting to ");
  Serial1.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
  }

  randomSeed(micros());

  Serial1.println("");
  Serial1.println("WiFi connected");
  Serial1.println("IP address: ");
  Serial1.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial1.print("Message arrived [");
  Serial1.print(topic);
  Serial1.print("] ");
  for (int i = 0; i < length; i++) {
    Serial1.print((char)payload[i]);
  }
  Serial1.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial1.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial1.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial1.print("failed, rc=");
      Serial1.print(client.state());
      Serial1.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);               // Vaillant Serial Port
  Serial1.begin(115200);            // Debug Serial Port
  Serial1.println('\0');
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial1.println("Vaillant ESP8266");
}

const int paramNrDefault = -1;
const int paramNrMax = 255;
int paramNr = paramNrDefault;

void loop() {

  //If the Client has lost connection --> reconnect
  if (!client.connected()) {
    reconnect();
  }

  //check MQTT for new messages
  client.loop();

  //read Parameter
  byte resArrayLen = 50;
  byte array[resArrayLen];

  int len;
  if(paramNr==-1)
  {
    // Read error log
    len = readRequest(1, paramNr, array, resArrayLen);

    if(len>0){
      client.publish("haniham/ErrorLog", (char*) array);
    }

  }
  else
  {
    len = readParam(paramNr, array, resArrayLen);

    //Len >0 ==> valid result
    if(len>0){
      //make MQTT topic
      char topic [30]="haniham/Raw/Param";
      topic[17] = '0' + paramNr/100;
      topic[18] = '0' + (paramNr/10)%10;
      topic[19] = '0' + paramNr%10;
      topic[20] = '\0';
      //Publish MQTT message
      client.publish(topic, (char*) array);

      //Start Parsing
      const Parameterelement* parameterelement = getParameterelement(paramNr);
    }
  }





  paramNr++;
  if(paramNr>paramNrMax)
    paramNr=paramNrDefault;

  delay(100);
}
