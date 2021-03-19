#include <EspNow.h>
String mac_addr = "3C:61:05:30:8E:40";
Peer myEsp;
json data;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  myEsp.init(mac_addr);              //register peer with mac_addr
  myEsp.setOnRecieve(your_function); //recevies data from other peer
  
  myEsp.setOnRecieve(error,"error");
  myEsp.setOnRecieve(success,"success");
  myEsp.setOnRecieve(idk,"idk");
}

void loop()
{
  data.clear();
  data.addUnit("Name", "Happy");
  data.addUnit("Age", "20");
  data.addUnit("CPI", "10");
  data.addUnit("type", "success");
  myEsp.send(data);
  delay(200);
}

void your_function(json msg)
{
  //Message is the data recieved from Peer it contains data insidee it.
  Serial.println("Data is : " + msg.getString());
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