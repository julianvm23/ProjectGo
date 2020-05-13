#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define latchPin 9
#define clockPin 10
#define dataPin 8
#define analogPin A0
#define boardSize 9

const int buttonPin = 2;     // the number of the pushbutton pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

/* change it with your ssid-password */
const char* ssid = "AIVD 2.0";
const char* password = "3171JD18#";

/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "81.207.147.229";

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/* topics */

#define GO_TOPIC     "floor1/go" /* true=on, false=off */

long lastMsg = 0;
char msg[20];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
   //Serial.println(message);
     for(int y=0; y<9; y++){
    for(int x=0; x<9; x++ ){
      Serial.print(payload[x*y]);
      Serial.print("\t");
    }
    Serial.print("\n");
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(GO_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(analogPin, INPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  Serial.begin(115200);
  // We start by connecting to a WiFi network
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

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
}
void loop() {
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming 
  subscribed topic-process-invoke receivedCallback */
  client.loop();
  long now = millis();

  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    lastMsg = now;
    byte sensorData[sq(boardSize)];
    for(int rows = 0; rows<sq(boardSize); rows++){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin,clockPin,LSBFIRST, rows);
      digitalWrite(latchPin, HIGH);
      delay(10);
      sensorData[rows] = analogRead(analogPin);
    }
    client.publish("floor1/go", sensorData,81);  
  } 
}
