#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
 
const char* WIFI_SSID = "Administrasjon (3)";
const char* WIFI_PASS = "jenny12345";
 
WebServer server(80);

const int LED_PIN = 33;
const int SERVO_PIN = 14; 
 
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);
void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));
 
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}
 
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}
 
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/plain", "LED is ON");
}

void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "LED is OFF");
}

void handleDig() {
  digitalWrite(SERVO_PIN, HIGH);
  server.send(200, "text/plain", "DIG is GO");
  delay(100);
  digitalWrite(SERVO_PIN, LOW);
  delay(1000);
  server.send(200, "text/plain", "DIG is DONE");
}
 
void  setup(){
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
  Serial.println("  /led-on");
  Serial.println("  /led-off");
  Serial.println("  /dig");

  pinMode(LED_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
 
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
  server.on("/led-on", handleLedOn);
  server.on("/led-off", handleLedOff);
  server.on("/dig", handleDig);
 
  server.begin();
}
 
void loop()
{
  server.handleClient();
}