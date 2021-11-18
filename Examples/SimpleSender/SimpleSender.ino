#include <EspNow.h>
Peer myEsp;
JSONVar myData;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setId("myESP2");//MAX 6 chars This is its own id
  myEsp.init("myESP1");            //register peer with peer_id
  myEsp.setOnRecieve(your_function); //recevies myData from other peer
}

void loop()
{
  myData["Name"] =  "Kalhar";
  myData["Age"] =  "22";
  myData["CPI"] =  "10";
  myData["type"] =  "success";
  myEsp.send(myData);
  delay(2000);
}

void your_function(JSONVar msg)
{
  //Message is the myData recieved from Peer it contains myData insidee it.
  Serial.println("myData is : " + JSON.stringify(msg));
}