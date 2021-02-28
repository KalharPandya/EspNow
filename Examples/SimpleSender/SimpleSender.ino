

#include <EspNow.h>
String mac_addr = "2C:F4:32:15:61:9E";
String data="Hello World!!!";
int command_type=1;
Peer myEsp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myEsp.init(mac_addr);//register peer with mac_addr
  myEsp.setOnRecieve(your_function);//recevies data from other peer
}

void loop() {
myEsp.send(data,command_type);//sends data to peer with mac_addr
delay(2000);
}

void your_function(Message msg) {
  //Message is the data recieved from Peer it contains data insidee it.
  String data = msg.data;
  int command_type = msg.command_type;
  int random_key = msg.randomKey;
  Serial.println("Data is : " + data + "\nCommand Type is : " + String(command_type) + "\nrandomKey is : " + String(random_key));
}
