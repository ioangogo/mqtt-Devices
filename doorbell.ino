// including libarys
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const byte innteruptpin = 5;
const int const_int_debounceMS = 200;
int int_lastpress = 0;

//mqtt vars
const char* Topic = "doorbell/state";
IPAddress host(0, 0, 0, 0);
const int port = 1883;
const char* username = "doorbell";
const char* password = "";
const char* devid = ""; //This could just be a UUID, MAC Address or a name
const char* str_msg = "ON";


WiFiClient wifiClient;


void callback(char* topic, byte* payload, unsigned int length) {
  // I am Button, i do not know about reciving messages
}

PubSubClient mqtt_client(host, port, callback, wifiClient);

extern void doorbell();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin("ssid", "pass");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
  //Connect to MQTT Server
  if(mqtt_client.connect(devid, username, password) == false){
  	while (true){
		delay(1000);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(2000);
		digitalWrite(LED_BUILTIN, LOW);
  	}
  }

  // do some pin stuff
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(innteruptpin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(innteruptpin), doorbell, RISING);

  digitalWrite(LED_BUILTIN, HIGH);
//  WiFi.forceSleepBegin();
}

void doorbell(){
  if((millis() - int_lastpress) > const_int_debounceMS){
	    mqtt_client.publish(Topic, str_msg, false);
  }
  int_lastpress = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

} 
