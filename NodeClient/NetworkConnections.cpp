#include "NetworkConnections.h"
struct NetworkDetails{
		const char * PREFIX = NETWORK_PREFIX;
		uint8_t PREFIX_LENGTH = strlen(NETWORK_PREFIX);
		const char * PASSWORD = NETWORK_PASSWORD;
		bool connected = false;
		bool busy = false;
		bool hasClientIP = false;
		bool hasReceivedLastPacket = false;
		String clientIP = "";
		unsigned long t_busy = 0;
		const unsigned long BUSY_PERIOD = 1000;
		unsigned long lastScannedMillis = 0;
		const uint16_t SCAN_PERIOD = 5000;
		
	} network;
	

int16_t GetMaxRSSI(uint16_t n) {
      int16_t maxRSSI;
      uint16_t returnIndex = -1;
      bool firstNW = true;
	  unsigned long t1 = millis();
      for (int i = 0; i < n; ++i)
      {
        Serial.println(WiFi.SSID(i));
        bool hasENC = WiFi.encryptionType(i) != ENC_TYPE_NONE;
		if(!hasENC) continue;
		bool hasPrefix  = strncmp(network.PREFIX, WiFi.SSID(i).c_str(), network.PREFIX_LENGTH) == 0;
		if(!hasPrefix) continue;
		int16_t nextRSSI = WiFi.RSSI(i);
        if (firstNW) {
          maxRSSI = nextRSSI;
          returnIndex = i;
          firstNW = false;
        } else if (nextRSSI > maxRSSI) {
          maxRSSI = nextRSSI;
          returnIndex = i;
        }
      }
	  Serial.println(millis() -t1);
      return returnIndex;
    }

void connectToSNode(uint16_t n) {
      int16_t index = GetMaxRSSI(n);
      if (index == -1) {
        Serial.println("No server found");
      } else {
        WiFi.begin(WiFi.SSID(index), network.PASSWORD);
        network.busy = true;
      }

      WiFi.scanDelete();
    }

void scanNWAsync() {
      if (!network.connected && !network.busy) {
        if (millis() - network.lastScannedMillis > network.SCAN_PERIOD || network.lastScannedMillis == 0)
        {
          network.lastScannedMillis = millis();
          WiFi.scanNetworks(true);
          Serial.print("\nScan start ... ");

        }


        int16_t n = WiFi.scanComplete();
        if (n > 0) {
          Serial.println(n);
          connectToSNode(n);
        }
      }
	  
      if ((millis() - network.t_busy > network.BUSY_PERIOD) && network.busy) {
        network.t_busy = millis();
		wl_status_t status = WiFi.status();
        switch(status){
		case WL_CONNECTED:
          network.busy = false;
          network.connected = true;
		  break;
		}
	  }
}



void setConnected(bool t){
	network.connected = t;
}

void setBusy(bool t){
	network.busy = t;
}
bool getConnected(){
	return network.connected;
}
bool getBusy(){
	return network.busy;
}
void setHasClientIP(bool t){
	network.hasClientIP = t;
}
bool getHasClientIP(){
	return network.hasClientIP;
}
void setLastPacketRecv(bool t){
	network.hasReceivedLastPacket = t;
}
bool getLastPacketRecv(){
	return network.hasReceivedLastPacket;
}
void setClientIP(String IP){
	network.clientIP = IP;
}
String getClientIP(){
	return network.clientIP;
}