// including libarys
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const byte innteruptpin = 5;
const unsigned  long const_int_debounceMS = 200;
unsigned long long_lastpress = 0;

//mqtt vars
const char* Topic = "doorbell/state";
IPAddress host(192, 168, 1, 11);
const int port = 1883;
const char* username = "doorbell";
const char* password = "";
const char* devid = ""; //This could just be a UUID, MAC Address or a name
const char* str_msg = "ON";

bool pressed = false;


WiFiClient wifiClient;


void callback(char* topic, byte* payload, unsigned int length) {
  // I am Button, i do not know about reciving messages
}

PubSubClient mqtt_client(host, port, callback, wifiClient);

extern void doorbell();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin("", "");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
  //Connect to MQTT Server
//  if (mqtt_client.connect(devid, username, password) == false) {
//    while (true) {
//      delay(1000);
//      digitalWrite(LED_BUILTIN, HIGH);
//      delay(500);
//      digitalWrite(LED_BUILTIN, LOW);
//    }
//  }

  // do some pin stuff
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(innteruptpin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(innteruptpin), doorbell, FALLING);

  digitalWrite(LED_BUILTIN, HIGH);
  //  WiFi.forceSleepBegin();
}

void doorbell() {
  pressed = true;
}



void loop() {
  if(millis() == 0){
    long_lastpress = 0;
  }
  // put your main code here, to run repeatedly:
  if(pressed == true && (millis() - long_lastpress) > const_int_debounceMS){
    mqtt_client.connect(devid, username, password);
    mqtt_client.publish(Topic, str_msg, false);
    mqtt_client.disconnect();
    pressed = false;
    long_lastpress = millis();
  }
}
