// Definerer pin-nummer for servoer og kontrollpinnene
const int servoControll = 14;  
const int servoLeft1 = 32;     
const int servoLeft2 = 33;     
const int servoRight1 = 25;    
const int servoRight2 = 26;    
const int controllPin = 4;     

// Variabler for servo-status
int servoGo = 0;       
int servoReady = 1;    

// Funksjon for å aktivere og bevege servoene
void dig() {
  if (servoReady) {  // Sjekker om servoene er klare
    Serial.println("DIG!DIG!DIG!");  
    servoReady = 0;  

    // Beveger servoene til ønsket posisjon
    ServoLeft1.write(100);  
    ServoLeft2.write(100);  
    ServoRight1.write(80);  
    ServoRight2.write(80);  
    delay(1000);  // Holder posisjonen i 1 sekund

    // Flytter servoene tilbake til startposisjon
    ServoLeft1.write(150);  
    ServoLeft2.write(150);  
    ServoRight1.write(30);  
    ServoRight2.write(30);  
    servoReady = 1;  
  }
}

// Setup-funksjon for initialisering av pinner og servoer
void setup() {
  Serial.begin(115200);  
  ServoLeft1.attach(servoLeft1);  
  ServoLeft2.attach(servoLeft2);  
  ServoRight1.attach(servoRight1);  
  ServoRight2.attach(servoRight2);  

  // Setter servoene til startposisjon
  ServoLeft1.write(150);  
  ServoLeft2.write(150);  
  ServoRight1.write(30);  
  ServoRight2.write(30);  
}

// Loop-funksjon som kontinuerlig leser kontrollpinnene og utfører handlinger
void loop() {
  servoGo = digitalRead(servoControll);  // Leser kontrollpinnen for servomotor
  updateLed();  // Oppdaterer LED-status
  if (servoGo) {  // Kjører dig-funksjonen hvis servoGo er aktivert
    dig();
  }
}

// Funksjon for å oppdatere LED-lampen basert på status
void updateLed() {
  if (ledOn) {  // Hvis LED-en skal være på
    digitalWrite(ledPin, HIGH);  
    Serial.println("Led is on");  
  }
  else {  // Hvis LED-en skal være av
    digitalWrite(ledPin, LOW);   
    Serial.println("Led is off");  
  }
}
