#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "MacAddress/MacAddress.h"
#include <ArduinoJSON.h>
#include <esp_now.h>
#define MAX_PEERS 20
int channel = 10, encrypt = 0;
int peerIndex = 0;
#define MAX_HANDLERS 15
Mac macHelper;
void onReceive(const uint8_t *mac, const uint8_t *data, int len);
String recievedData;
class Peer
{
public:
	json handleType;
	esp_now_peer_info_t *peer;
	int handlerIndex = 0;
	void (*dataRecieve[15])(json);
	Mac *peerAddress = new Mac();
	void init(String mac_addr)
	{
		this->peerAddress->setAddress(mac_addr);
		if (peerIndex == 0)
			InitESPNow();
		addThisPeer();
		createPeer();
		esp_now_register_recv_cb(onReceive);
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
	void setOnRecieve(void (*f)(json), String type = "")
	{
		handleType.addUnit(type,handlerIndex);
		this->dataRecieve[handlerIndex++] = f;
	}
	void send(json data)
	{
		String dataString = data.getString();
		// dataString = "Hello";
		const char *dataConst = dataString.c_str();
		int dataSize = dataString.length() + 1;
		char dataArray[dataSize];
		memcpy(dataArray, dataConst, dataSize);
		esp_now_send(peerAddress->getAddress(), (uint8_t *)dataArray, dataSize);
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
json recievedJson;
Peer dataFrom;
void onReceive(const uint8_t *src, const uint8_t *data, int len)
{
	recievedJson.clear();
	macHelper.copyConstantUint(src);
	recievedData = "";
	for (int i = 0; i < len; i++)
	{
		recievedData += char(data[i]);
	}
	recievedJson = parseJSON(recievedData);
	String type = recievedJson.getValue("type");
	dataFrom = findPeer(macHelper.getStrAddress());
	int typeIndex = dataFrom.handleType.getNumberValue(type);
	dataFrom.dataRecieve[typeIndex](recievedJson);
}