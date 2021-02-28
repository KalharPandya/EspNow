#include<Message.h>
Message msg;
  String mac_addr= "2C:F4:32:15:61:9E";
  int c=1;
  String s="data";

void setup() {
  Serial.begin(115200);
  msg.setMessage(s,c);
  msg.create();
  msg.display();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
}
