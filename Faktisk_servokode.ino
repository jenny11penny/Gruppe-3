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

void updateLed() {
  if (ledOn) {
    digitalWrite(ledPin,HIGH);
    Serial.println("Led is on");
  }
  else {
    digitalWrite(ledPin,LOW);
    Serial.println("Led is off");
  }}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  pinMode(controllPin, INPUT_PULLDOWN);
  pinMode(servoControll, INPUT_PULLDOWN);
  pinMode(servoLeft1, OUTPUT);
  pinMode(servoLeft2, OUTPUT);
  pinMode(servoRight1, OUTPUT);
  pinMode(servoRight2, OUTPUT);


}
void loop() {
  servoGo = digitalRead(servoControll);
  ledOn = digitalRead(controllPin);
  updateLed();
  if (servoGo) {
    dig();
  }
}
