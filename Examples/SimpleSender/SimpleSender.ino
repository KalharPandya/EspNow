#include <EspNow.h>
String mac_addr = "2C:F4:32:15:61:9E";
int command_type = 1;
Peer myEsp;
json data;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  myEsp.init(mac_addr);              //register peer with mac_addr
  myEsp.setOnRecieve(your_function); //recevies data from other peer
}

void loop()
{
  data.clear();
  data.addUnit("Name", "Happy");
  data.addUnit("Age", "20");
  data.addUnit("CPI", "10");
  myEsp.send(data);
  delay(2000);
}

void your_function(json msg)
{
  //Message is the data recieved from Peer it contains data insidee it.
  Serial.println("Data is : " + msg.getString());
}
