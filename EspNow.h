#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "MacAddress/MacAddress.h"
#include <Arduino_JSON.h>
#include <esp_now.h>
#define MAX_PEERS 20
int channel = 10, encrypt = 0;
int peerIndex = 0;
#define MAX_HANDLERS 15
Mac macHelper;
void onReceive(const uint8_t *mac, const uint8_t *data, int len);
String recievedData;
void defaultPrintHandlerESPNow(JSONVar msg)
{
	Serial.println("Data found");
	Serial.print(msg["data"]);
	if(msg["newLine"])	Serial.println();
}
void defaultPrintHandlerESPNow1(JSONVar msg)
{
	Serial.println("Data found 11111111111111");
	Serial.print(msg["data"]);
	if(msg["newLine"])	Serial.println();
}
class Peer
{
public:
	JSONVar handleType;
	esp_now_peer_info_t *peer;
	int handlerIndex = 0;
	void (*dataRecieve[15])(JSONVar);
	Mac *peerAddress = new Mac();
	void init(String id)
	{
		this->peerAddress->parseName(id);
		if (peerIndex == 0)
			InitESPNow();
		addThisPeer();
		createPeer();
		esp_now_register_recv_cb(onReceive);
		// setOnRecieve(defaultPrintHandlerESPNow1,"print");
		setOnRecieve(defaultPrintHandlerESPNow,"print");

	}
	void InitESPNow()
	{
		WiFi.mode(WIFI_STA);
		WiFi.disconnect();
		if (esp_now_init() == ESP_OK)
		{
			Serial.println("ESPNow Init Success");
		}
		else
		{
			Serial.println("ESPNow Init Failed");
			// Retry InitESPNow, add a counte and then restart?
			// InitESPNow();
			// or Simply Restart
			ESP.restart();
		}
	}
	void addThisPeer();

	void createPeer()
	{
		peer = new esp_now_peer_info_t();
		memcpy(peer->peer_addr, peerAddress->getAddress(), 6);
		// Register the peer
		if (esp_now_add_peer(peer) == ESP_OK)
		{
			// Serial.println("Peer added");
		}
	}
	void setOnRecieve(void (*f)(JSONVar), String type = "default")
	{
		handleType[type] =handlerIndex;
		this->dataRecieve[handlerIndex++] = f;
	}
	void send(JSONVar data)
	{
		if(data["type"]== null)	data["type"] = "default";
		String dataString = JSON.stringify(data);
		sendString(dataString);
	}
	void sendString(String dataString)
	{
		const char *dataConst = dataString.c_str();
		int dataSize = dataString.length() + 1;
		char dataArray[dataSize];
		memcpy(dataArray, dataConst	, dataSize);
		esp_now_send(peerAddress->getAddress(), (uint8_t *)dataArray, dataSize);
	}
	void println(String dataString)
	{
		print(dataString, true);
	}
	void print(String dataString, bool newLine = false)
	{
		JSONVar data;
		data["data"]=dataString;
		data["type"]="print";
		data["newLine"] = newLine;
		send(data);
	}
};

Peer *allPeers[MAX_PEERS];
void Peer::addThisPeer()
{
	allPeers[peerIndex++] = this;
}

Peer findPeer(String targetAddress)
{
	for (int i = 0; i < peerIndex; i++)
	{
		if (targetAddress == allPeers[i]->peerAddress->getStrAddress())
		{
			return *allPeers[i];
		}
	}
}
JSONVar recievedJson;
JSONVar stringData;
Peer dataFrom;
void onReceive(const uint8_t *src, const uint8_t *data, int len)
{
	recievedJson = JSONVar();
	macHelper.copyConstantUint(src);
	recievedData = "";
	String type;
	
	for (int i = 0; i < len; i++)
	{
		recievedData += char(data[i]);
	}
	
	recievedJson = JSON.parse(recievedData);
	
	if(JSON.typeof(recievedJson) == "undefined"){
		recievedJson = JSONVar();
		recievedJson["type"] =  "String";
		recievedJson["value"] = recievedData;
		type = "String";
	}
	type = recievedJson["type"];
	dataFrom = findPeer(macHelper.getStrAddress());
	int typeIndex = dataFrom.handleType.hasOwnProperty(type)?dataFrom.handleType[type]:dataFrom.handleType["default"];

	dataFrom.dataRecieve[typeIndex](recievedJson);
}

void setId(String id)
{
	macHelper.parseName(id);
	esp_base_mac_addr_set(macHelper.getAddress());
}