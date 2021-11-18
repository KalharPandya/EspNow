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

void success(JSONVar msg) {
  Serial.println("Success is : " + JSON.stringify(msg));
  Serial.println(ESP.getFreeHeap());
}
void error(JSONVar msg) {
  Serial.println("Error is : " + JSON.stringify(msg));
  Serial.println(ESP.getFreeHeap());
}
void defaultHandler(JSONVar msg) {
  Serial.println("Default is : " + JSON.stringify(msg));
  Serial.println(ESP.getFreeHeap());
}
void idk(JSONVar msg) {
  Serial.println("IDK!!!!!!!!!!!! is : " + JSON.stringify(msg));
}