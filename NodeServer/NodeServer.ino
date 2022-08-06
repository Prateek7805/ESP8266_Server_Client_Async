#include<ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);
#define BLINK_PERIOD 250
long lastBlinkMillis;
boolean ledState = 0;
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("NODEServer","NodeServer");
  delay(100);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500); 
  }
  Serial.println("connection");
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    ledState = request->getParam("state")->value().toInt();
    Serial.println(ledState);
    request->send(200, "text/plain", "OK");
  });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
wdt_reset();
digitalWrite(LED_BUILTIN, ledState);
    
}
