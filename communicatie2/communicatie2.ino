#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define latchPin 13
#define clockPin 15
#define dataPin 12
#define analogPin A0
#define boardSize 9
#define buttonPin 5     // the number of the pushbutton pin
int sensorData[sq(boardSize)];
int tempLicht[sq(boardSize)];
byte stones[sq(boardSize)+1];
byte speler = 0;

/* Vul hier je ssid en password van de WiFi in */
const char* ssid = "Ziggo0DD7F";
const char* password = "uxoKPpNsaaNc";

/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "81.207.147.229";

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/* topics */

#define GO_TOPIC     "floor1/go" /* true=on, false=off */

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  if(payload[sq(boardSize)] == 0){
    speler = 0;
    Serial.println("Jij bent aan de beurt");
    for(int y=0; y<boardSize; y++){
      for(int x=0; x<boardSize; x++ ){
        Serial.print(payload[x*y]);
        Serial.print("\t");
      }
      Serial.print("\n");
    }
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP8266Client";
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
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the pushbutton pin as an input:

  
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
  
  for(int rows = 0; rows<sq(boardSize); rows++){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin,clockPin,LSBFIRST, rows);
    digitalWrite(latchPin, HIGH);
    delay(10);
    tempLicht[rows] = analogRead(analogPin);
  }
}
void loop() {
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming 
  subscribed topic-process-invoke receivedCallback */
  client.loop();
  if (digitalRead(buttonPin) == LOW && speler == 0) {
    for(int rows = 0; rows<sq(boardSize); rows++){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin,clockPin,LSBFIRST, rows);
      digitalWrite(latchPin, HIGH);
      delay(10);
      sensorData[rows] = analogRead(analogPin);
      if(float(sensorData[rows])<(float(tempLicht[rows])/100*95)){
        stones[rows] = 255;
      }
      else{
        stones[rows] = 0;
      }
    }
    speler = 255;
    stones[sq(boardSize)] = speler;
    client.publish("floor1/go", stones,(sq(boardSize)+1)); 
    delay(1000); 
  } 
}
