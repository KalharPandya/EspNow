

#include <EspNow.h>
String mac_addr = "2C:F4:32:15:61:9E";
Peer myEsp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myEsp.init(mac_addr);
  myEsp.setOnRecieve(your_function);//pass a function which will be called when data is recieved, 
}

void loop() {
}


void your_function(json msg) {
  //Message is the data recieved from Peer it contains data insidee it.
  Serial.println("Data is : " + msg.getString());
}
