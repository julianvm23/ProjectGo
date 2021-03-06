#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* change it with your ssid-password */
const char* ssid = "Huisje67";
const char* password = "DeGuDaBo248857149!";

/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "81.207.147.229";
float temperature = 0;

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/*LED GPIO pin*/
const int led = 4;
const int led2 = 26;

/* topics */

#define LED_TOPIC     "floor1/led" /* true=on, false=off */
#define TEMP_TOPIC    "floor1/temp"


long lastMsg = 0;
char msg[20];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print(payload[i]);
  }
  Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == 't') {
    digitalWrite(led, HIGH);
     
  } 
  if ((char)payload[0] == 'r'){
    /* we got '0' -> on */
    digitalWrite(led, LOW);
  }
  if ((char)payload[0] == 'p') {
    digitalWrite(led2, HIGH); 
  } 
  else if ((char)payload[0] == 'q'){
    /* we got '0' -> on */
    digitalWrite(led2, LOW);
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
      client.subscribe(LED_TOPIC);
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
  /* set led as output to control led on-off */
  pinMode(led, OUTPUT);
   pinMode(led2, OUTPUT);

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
} 
