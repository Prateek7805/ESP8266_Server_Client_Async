#ifndef _NETWORK_CONNECTIONS
	#define _NETWORK_CONNECTIONS
	#define NETWORK_PREFIX "NODEServer"
	#define NETWORK_PASSWORD "NodeServer"
	#include<Arduino.h>
	#include<ESP8266WiFi.h>
	#include <ESPAsyncTCP.h>
	#include <ESPAsyncWebServer.h>
	
	struct NetworkDetails;
	typedef void (*_handleData)(void*, AsyncClient*, void *, uint32_t);
	typedef void (*_deleteClient)( AsyncClient *);
	typedef void (* _handleConnect)(void*, AsyncClient*);
	typedef void (* _handleError) (void*, AsyncClient*, int);
	typedef struct cd{
		AsyncClient * aClient = NULL;
		String HOST = "192.168.4.1";
		uint16_t PORT = 80;
		String uri = "update";
		String paramString = "";
		_handleData handleData;
		_deleteClient deleteClient;
		_handleConnect handleConnect;
		_handleError handleError = NULL;
	} clientDetails;
	
	
	int16_t GetMaxRSSI(uint16_t n);
	void connectToSNode(uint16_t n);
	void scanNWAsync(void);
	
	
	void setConnected(bool t);
	void setBusy(bool t);
	bool getConnected();
	bool getBusy();
	void setHasClientIP(bool t);
	bool getHasClientIP();
	void setClientIP(String IP);
	String getClientIP();
	
#endif