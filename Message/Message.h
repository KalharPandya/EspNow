#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include "../MacAddress/MacAddress.h"
Mac macHelper;
class Message
{
    public:

      String data;
      int command_type;
      uint8_t randomKey;
      void setMessage(String data,int command_type = 0){
        this->data = data;
        this->command_type = command_type;
      }
      void create(){
        this->randomKey = random(1,255);
        
      }
       void display()
      {
        Serial.print("data:");
        Serial.println(data);
        Serial.print("command_type:");
        Serial.println(command_type);
        Serial.print("randomKey:");
        Serial.println(randomKey);
             
        //Serial.println("Size of this object = "+ String(sizeof(*this)));
      }
};
