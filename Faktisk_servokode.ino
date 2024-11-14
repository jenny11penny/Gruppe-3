// Definerer pin-nummer for servoer og kontrollpinner
const int servoControll = 14;  
const int servoLeft1 = 32;     
const int servoLeft2 = 33;     
const int servoRight1 = 25;    
const int servoRight2 = 26;    
const int controllPin = 4;     // Kontrollpinne for LED

// Variabler for servo-status
int servoGo = 0;       // Bestemmer om servomotorene skal aktivere
int servoReady = 1;    // Sjekker om servomotorene er klare til å starte

// Funksjon for å bevege servoene 
void dig() {
  if (servoReady) {  // Sjekker om servoene er klare
    Serial.println("DIG!DIG!DIG!");  // Skriver ut melding til Serial Monitor
    servoReady = 0;  // Setter servoReady til 0 for å unngå ny aktivering

    // Aktiverer venstre servoer
    digitalWrite(servoLeft1, HIGH);
    digitalWrite(servoLeft2, HIGH);
    delay(5000);  // Holder venstre servoer aktive i 5 sekunder
    digitalWrite(servoLeft1, LOW);
    digitalWrite(servoLeft2, LOW);
    delay(1000);  // Venter 1 sekund

    // Aktiverer høyre servoer
    digitalWrite(servoRight1, HIGH);
    digitalWrite(servoRight2, HIGH);
    delay(5000);  // Holder høyre servoer aktive i 5 sekunder
    digitalWrite(servoRight1, LOW);
    digitalWrite(servoRight2, LOW);
    delay(1000);  // Venter 1 sekund

    servoReady = 1;  // Setter servoReady tilbake til 1 for å tillate neste aktivering
  }
}

// Funksjon for å oppdatere LED-status
void updateLed() {
  if (ledOn) {  // Hvis LED-en skal være på
    digitalWrite(ledPin, HIGH);  // Tennes LED-lampen
    Serial.println("Led is on");  // Skriver ut melding til Serial Monitor
  }
  else {  // Hvis LED-en skal være av
    digitalWrite(ledPin, LOW);   // Slukkes LED-lampen
    Serial.println("Led is off");  // Skriver ut melding til Serial Monitor
  }
}

// Setup-funksjon for initialisering
void setup() {
  Serial.begin(115200);  // Starter seriell kommunikasjon
  pinMode(ledPin, OUTPUT);  // Setter LED-pinnen som utgang
  pinMode(controllPin, INPUT_PULLDOWN);  // Setter kontrollpinnen som inngang med pull-down motstand
  pinMode(servoControll, INPUT_PULLDOWN);  // Setter servo kontrollpinnen som inngang med pull-down
  pinMode(servoLeft1, OUTPUT);  // Setter venstre servo 1 som utgang
  pinMode(servoLeft2, OUTPUT);  // Setter venstre servo 2 som utgang
  pinMode(servoRight1, OUTPUT);  // Setter høyre servo 1 som utgang
  pinMode(servoRight2, OUTPUT);  // Setter høyre servo 2 som utgang
}

// Loop-funksjon som kontinuerlig kjører
void loop() {
  servoGo = digitalRead(servoControll);  // Leser servo kontrollpinnen for å starte graving
  ledOn = digitalRead(controllPin);  // Leser kontrollpinnen for LED-status
  updateLed();  // Oppdaterer LED-lampen basert på status
  if (servoGo) {  // Hvis kontrollsignalet for servo er aktivert, kjør dig-funksjonen
    dig();
  }
}
