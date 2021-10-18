#include <EspNow.h>
Peer myEsp;
void setup() {
  Serial.begin(115200);
  setId("myESP1");//MAX 6 chars This is its own id
  myEsp.init("myESP2");  //  register peer with peer_id
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