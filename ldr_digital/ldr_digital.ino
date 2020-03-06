// configureer hier de LDRsensor pin, deze sluiten we hier
// aan op de analoge poort 0 van de arduino: A0
int LDRsensor = 2;
 
void setup() {                
  Serial.begin(9600); // zet seriele poort aan op 9600 baud.
}
 
// deze loop wordt continu herhaald
void loop() {
  // print LDR waarde via seriele monitor
  Serial.print("LDR waarde: ");
  Serial.print(digitalRead(LDRsensor));
  Serial.println("");
  delay(100); // wacht 100 ms
}
