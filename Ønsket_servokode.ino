const int servoControll = 14;
const int servoLeft1 = 32;
const int servoLeft2 = 33;
const int servoRight1 = 25;
const int servoRight2 = 26;
const int ledPin = 17;
const int controllPin = 4;

//bools
int servoGo = 0;
int servoReady = 1;
void dig() {
  if (servoReady) {
    //dig
    Serial.println("DIG!DIG!DIG!");
    servoReady = 0;
      ServoLeft1.write(100);
      ServoLeft2.write(100);
      ServoRight1.write(80);
      servoRight2.write(80);
      delay(1000);       // Hold posisjonen i 1 sekund

    //Tilbake til start
      ServoLeft1.write(150);
      ServoLeft2.write(150);
      ServoRight1.write(30);
      servoRight2.write(30);
    servoReady = 1;
  }
}

void setup() {
  Serial.begin(115200);
  ServoLeft1.attach(servoLeft1);
  ServoLeft2.attach(servoLeft2;
  ServoRight1.attach(servoRight1);
  ServoRight2.attach(servoRight2);

  // Startposisjon for servoene
  ServoLeft1.write(150);
  ServoLeft2.write(150);
  ServoRight1.write(30);
  ServoRight2.write(30);


}
void loop() {
  servoGo = digitalRead(servoControll);
  updateLed();
  if (servoGo) {
    dig();
  }
}
