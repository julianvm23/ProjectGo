#define latchPin 9
#define clockPin 10
#define dataPin 8
#define analogPin A0
#define boardSize 9
int sensorData[boardSize][boardSize];

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(analogPin,    INPUT);
  Serial.begin(9600);
}

void loop() {
  for(int rows = 0; rows<boardSize; rows++){
    for(int colls = 0; colls<boardSize; colls++){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin,clockPin,LSBFIRST, (boardSize*rows+colls));
      digitalWrite(latchPin, HIGH);
      delay(10);
      sensorData[rows][colls] = analogRead(analogPin);
    }
  }
  for(int rows = 0; rows<boardSize; rows++){
    for(int colls = 0; colls<boardSize; colls++){
      if(rows == 1 &&( colls == 7 || colls == 8)){
        Serial.print(sensorData[rows][colls]);
      }else if(rows == 2 && (colls == 0 || colls == 1)){
        Serial.print(sensorData[rows][colls]);
      }else{
        Serial.print("X");
      }
      Serial.print("\t");
    }
    Serial.print("\n");
    Serial.print("\n");
  }
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n");
  delay(100);
} 
