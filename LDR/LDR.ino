#define LDR A0
#define LED 3

void setup() {
  pinMode(LDR,INPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldr = analogRead(LDR);
  Serial.println(ldr);
  if(ldr <= 7){
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(200);
}
