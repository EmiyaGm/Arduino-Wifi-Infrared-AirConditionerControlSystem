#include <IRremote.h>

#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "GM"
#define PASSWORD    "123456789abc"
#define HOST_NAME   "10.18.55.240"
#define HOST_PORT   (1234)

SoftwareSerial mySerial(10, 11); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);
IRsend irsend;
void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");
    
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
      
    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP:");
        Serial.println( wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
    
    Serial.print("setup end\r\n");
}
 
void loop(void)
{
    uint8_t buffer[128] = {0};
    
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
        //char *hello = "Hello, this is client!";
       // wifi.send((const uint8_t*)hello, strlen(hello));
        while(true){
      String str="";
    unsigned int irSignal[140];
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len-1; i++) {
            if((char)buffer[i]!=','){
              Serial.print((char)buffer[i]); 
              str=str+((char)buffer[i]);     
            }else{
              irSignal[i]=str.toInt();
              str="";
            }
            
        }
        Serial.print("]\r\n");
    }
    int khz = 38;
    irsend.sendRaw(irSignal, sizeof(irSignal), khz);
    }
    } else {
        Serial.print("create tcp err\r\n");
    }
    
    delay(5000);
}

