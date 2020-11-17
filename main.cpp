#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "Esp32MQTTClient.h"
#include "PubSubClient.h"

const char* ssid = "selma-razvoj";
const char* pass = "selmasubotica";
const char* brokerUser = "tilben97biker@gmail.com";
const char* brokerPass = "0dc9d70b";
const char* broker = "mqtt.dioty.co";
const char* outTopic = "/tilben97biker@gmail.com/out";
const char* inTopic = "/tilben97biker@gmail.com/in";

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
long cnt = 0;
char message[50];
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message Received.... Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for(int i = 0; i<length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if(payload[0] == 't' && payload[1] == 'r')
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n Connected to the network");
  Serial.print("Network IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(broker, 1883);
  client.setCallback(callback);

    Serial.print("Connecting to");
    Serial.print(broker);
}

void loop() {
  while(!client.connected())
  {
    Serial.print(".");
    if(client.connect("tralala", brokerUser, brokerPass))
    {
      Serial.print("\n Connected");
      client.subscribe(inTopic);
    }
  }
  client.loop();

  currentTime = millis();
  if(currentTime - lastTime > 2000)
  {
    cnt++;
    sniprintf(message, 75, "Count: %ld", cnt);
    client.publish(outTopic, message);
    lastTime = millis();

  }
  
}

