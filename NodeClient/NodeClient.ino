#include<LittleFS.h>
#include "NetworkConnections.h"
 

const char * SERVER_HOST_IP = "192.168.4.1";
const uint16_t TCP_PORT = 80;


#define BLINK_PERIOD 500
long lastBlinkMillis;
boolean ledState;
uint8_t buttonPin = 0;

clientDetails clientInfo;
/*STA Events*/
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler, connectedEventHandler;
/*End STA Events*/

/* event callbacks */
static void handleClientData(void* arg, AsyncClient* client, void *data, size_t len) {
  //Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
  //Serial.write((uint8_t*)data, len);
  //Serial.println(clientInfo.aClient == NULL);
  deleteClient(client);
  //Serial.println(clientInfo.aClient == NULL);
}

static void onClientConnect(void* arg, AsyncClient* client) {
  Serial.printf("\n client has been connected to %s on port %d \n", SERVER_HOST_IP, TCP_PORT);
  if (client->space() > 32 && client->canSend()) {
    String req = "GET /" + clientInfo.uri + "?" + clientInfo.paramString + " HTTP/1.0\r\nHost: " + WiFi.localIP().toString() + "\r\n\r\n";
    Serial.println(req);
    //send the request
    client->write(req.c_str());
  }
}

static void deleteClient(AsyncClient * client) {
  clientInfo.aClient = NULL;
  delete client;
}

/**/
void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP & event)
  {
    Serial.print("Station connected, IP: ");
    Serial.println(WiFi.localIP());
    setHasClientIP(true);
    setClientIP(WiFi.localIP().toString());
  });

  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected & event)
  {
    Serial.println("Station disconnected");
    setBusy(false);
    setConnected(false);
    setHasClientIP(false);
    deleteClient(clientInfo.aClient);
  });

  connectedEventHandler = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected & event) {
    Serial.println("Station connected");
    setBusy(false);
    setConnected(true);
    setHasClientIP(false);
  });
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}
void sendDataToServer(clientDetails * ci) {
  if (getHasClientIP()) {
    Serial.println("here");
    if (ci->aClient) {
      ci->aClient->close(true);
      ci->aClient->connect(ci->HOST.c_str(), ci->PORT);
      return;
    }
    ci->aClient = new AsyncClient;
    ci->aClient->onData(ci->handleData, ci->aClient);
    ci->aClient->onConnect(ci->handleConnect, ci->aClient);
    ci->aClient->connect(ci->HOST.c_str(), ci->PORT);
  }
}




void loop()
{

  // blink LED
  if (millis() - lastBlinkMillis > BLINK_PERIOD)
  {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    lastBlinkMillis = millis();
    if(!digitalRead(buttonPin)){
      //Serial.println("YES");
      clientInfo.uri = "update";
      clientInfo.paramString = "state=" + String(!ledState);
      sendDataToServer(&clientInfo);
    }else{
      clientInfo.uri = "update";
      clientInfo.paramString = "state=" + String(ledState);
      sendDataToServer(&clientInfo);
    }
    
  }
  scanNWAsync();

}
