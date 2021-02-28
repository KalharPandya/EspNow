#include <Arduino.h>
#include <esp_now.h>
#include "Message/Message.h"
#define MAX_PEERS 20
int channel = 4, encrypt = 0;
Message recievedData;
int peerIndex = 0;
void onReceive(const uint8_t *mac, const uint8_t *data, int len);
class Peer
{
public:
	Message *msg;
	esp_now_peer_info_t *peer;
	String peerId="motor1";
	void (*dataRecieve)(Message);
	Mac *peerAddress = new Mac();
	void init(String mac_addr)
	{
		this->peerAddress->setAddress(mac_addr);
		msg = new Message();
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
			// Serial.println("ESPNow Init Success");
		}
		else
		{
			// Serial.println("ESPNow Init Failed");
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
		peer->channel = channel;
		peer->encrypt = encrypt;
		// Register the peer
		if (esp_now_add_peer(peer) == ESP_OK)
		{
			// Serial.println("Peer added");
		}
	}
	void setOnRecieve(void (*f)(Message))
	{
		this->dataRecieve = f;
	}
	void send(String data, int com_type)
	{
		msg->setMessage(data, com_type);
		msg->create();
		msg->display();
		esp_now_send(peerAddress->getAddress(), (uint8_t *)msg, sizeof(*msg));
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

void onReceive(const uint8_t *src, const uint8_t *data, int len)
{
	macHelper.copyConstantUint(src);
	memcpy(&recievedData, data, sizeof(recievedData));
	findPeer(macHelper.getStrAddress()).dataRecieve(recievedData);
}