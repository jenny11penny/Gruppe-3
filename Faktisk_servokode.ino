const int servoControll = 14;
const int servoLeft1 = 32;
const int servoLeft2 = 33;
const int servoRight1 = 25;
const int servoRight2 = 26;
const int controllPin = 4;

//bools

int servoGo = 0;
int servoReady = 1;
void dig() {
  if (servoReady) {
    //dig
    Serial.println("DIG!DIG!DIG!");
    servoReady = 0;
    digitalWrite(servoLeft1, HIGH);
    digitalWrite(servoLeft2, HIGH);
    delay(5000);
    digitalWrite(servoLeft1, LOW);
    digitalWrite(servoLeft2, LOW);
    delay(1000);
    digitalWrite(servoRight1, HIGH);
    digitalWrite(servoRight2, HIGH);
    delay(5000);
    digitalWrite(servoRight1, LOW);
    digitalWrite(servoRight2, LOW);
    delay(1000);
    servoReady = 1; 
  }
}
//updateled

void setup() {
  Serial.begin(115200);
  ServoLeft1.attach(servoLeft1);
  ServoLeft2.attach(servoLeft2;
  ServoRight1.attach(servoRight1);
  ServoRight2.attach(servoRight2);

  // Startposisjon for servoene
  ServoLeft1.write(150);
  ServoLeft2.write(150);
  ServoRight1(30);
  ServoRight2.write(30);


}
void loop() {
  servoGo = digitalRead(servoControll);
  updateLed();
  if (servoGo) {
    dig();
  }
}