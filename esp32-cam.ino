#include <WebServer.h> // Bibliotek for å håndtere webserver-funksjoner på ESP32.
#include <WiFi.h>      // Bibliotek for å administrere WiFi-tilkoblinger på ESP32.
#include <esp32cam.h>  // Bibliotek for å kontrollere kamera-modulen på ESP32.

// Definer WiFi-legitimasjon. 
const char* WIFI_SSID = "Administrasjon (3)";
const char* WIFI_PASS = "jenny12345";

// Oppretter en webserver på port 80 (standard HTTP-port).
WebServer server(80);

// Pin-definisjoner
const int LED_PIN = 33;   // GPIO-pin for å kontrollere en LED.
const int SERVO_PIN = 14; // GPIO-pin for å kontrollere en servo.

// Definer forskjellige kameraoppløsninger.
static auto loRes = esp32cam::Resolution::find(320, 240); // Lav oppløsning (320x240)
static auto midRes = esp32cam::Resolution::find(350, 530); // Middels oppløsning (350x530)
static auto hiRes = esp32cam::Resolution::find(800, 600); // Høy oppløsning (800x600)

// Funksjon for å fange et bilde fra kameraet og gi ut på en nettside
void serveJpg()
{
  // Fang et bilde fra kameraet.
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    // Hvis bildet ikke kan fanges, skriv ut en feilmelding
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  // Hvis fangst av bilde er vellykket, skriv ut oppløsning og størrelse på bildet
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  // Sett innholdslengde og send bildet som JPEG til klienten
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client); // Skriv bildet til webklienten.
}

// Handler-funksjon for å sette kameraet til lav oppløsning og sende et bilde.
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    // Hvis det er feil ved å sette lav oppløsning, skriv ut en feilmelding.
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg(); // Server bildet.
}

// Handler-funksjon for å sette kameraet til høy oppløsning og sende et bilde.
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    // Hvis det er feil ved å sette høy oppløsning, skriv ut en feilmelding.
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg(); // Server bildet.
}

// Handler-funksjon for å sette kameraet til middels oppløsning og sende et bilde.
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    // Hvis det er feil ved å sette middels oppløsning, skriv ut en feilmelding.
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg(); // Server bildet.
}

// Handler for å slå på LED-en.
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH); // Sett LED-pinnen til høy (slå på LED-en).
  server.send(200, "text/plain", "LED is ON"); // Send bekreftelse til klienten.
}

// Handler for å slå av LED-en.
void handleLedOff() {
  digitalWrite(LED_PIN, LOW); // Sett LED-pinnen til lav (slå av LED-en).
  server.send(200, "text/plain", "LED is OFF"); // Send bekreftelse til klienten.
}

// Handler for å aktivere servo-motoren.
void handleDig() {
  digitalWrite(SERVO_PIN, HIGH); // Slå på servo-motoren.
  server.send(200, "text/plain", "DIG is GO"); // Send bekreftelse til klienten.
  delay(100); // Vent i 100 millisekunder.
  digitalWrite(SERVO_PIN, LOW); // Slå av servo-motoren.
  delay(1000); // Vent i 1 sekund.
  server.send(200, "text/plain", "DIG is DONE"); // Send bekreftelse til klienten.
}

// Funksjon for å sette opp ESP32 og webserveren.
void setup() {
  Serial.begin(115200); // Start seriell kommunikasjon for debugging.
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;  // Konfigurasjon for kameraet.
    cfg.setPins(pins::AiThinker); // Sett pins for AiThinker-kamera.
    cfg.setResolution(hiRes);    // Sett oppløsning til høy oppløsning.
    cfg.setBufferCount(2);       // Sett antall buffere til 2.
    cfg.setJpeg(80);             // Sett JPEG-komprimering til 80%.

    bool ok = Camera.begin(cfg); // Initialiser kameraet med den angitte konfigurasjonen.
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL"); // Skriv ut om kameraet ble initialisert riktig.
  }

  WiFi.persistent(false);  // Deaktiver lagring av WiFi-innstillinger for fremtidige oppkoblinger.
  WiFi.mode(WIFI_STA);     // Sett ESP32 til stasjonmodus (WiFi-klient).
  WiFi.begin(WIFI_SSID, WIFI_PASS); // Koble til WiFi med SSID og passord.
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Vent til ESP32 er koblet til WiFi.
  }

  // Når tilkoblingen er vellykket, skriv ut IP-adressen til ESP32.
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  // Skriv ut URL-er for å hente bilder og kontrollere enheter.
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
  Serial.println("  /led-on");
  Serial.println("  /led-off");
  Serial.println("  /dig");

  // Sett pinnene for LED og servo som utganger.
  pinMode(LED_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  // Definer webserverens URL-ruter og tilhørende handler-funksjoner.
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
  server.on("/led-on", handleLedOn);
  server.on("/led-off", handleLedOff);
  server.on("/dig", handleDig);

  server.begin(); // Start webserveren.
}

// Hovedloop-funksjon for å håndtere klientforespørsler.
void loop() {
  server.handleClient(); // Håndter innkommende HTTP-forespørsler.
}
