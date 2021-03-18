#include <EspNow.h>
String mac_addr = "3C:61:05:31:27:5C";
Peer myEsp;
void setup() {
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  myEsp.init(mac_addr);
  myEsp.setOnRecieve(defaultHandler);
  myEsp.setOnRecieve(error,"error");
  myEsp.setOnRecieve(success,"success");
  myEsp.setOnRecieve(idk,"idk");
}

void loop() {
}


void success(json msg) {
  Serial.println("Success is : " + msg.getString());
  Serial.println(ESP.getFreeHeap());
}
void error(json msg) {
  Serial.println("Error is : " + msg.getString());
  Serial.println(ESP.getFreeHeap());
}
void defaultHandler(json msg) {
  Serial.println("Default is : " + msg.getString());
  Serial.println(ESP.getFreeHeap());
}
void idk(json msg) {
  Serial.println("IDK!!!!!!!!!!!! is : " + msg.getString());
}