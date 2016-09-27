// esp8266_test.ino
//
// Plot LM35 data on thingspeak.com using an Arduino and an ESP8266 WiFi 
// module.
//
// Author: Mahesh Venkitachalam
// Website: electronut.in

#include <SoftwareSerial.h>
#include <stdlib.h>

#define DEBUG true

// LED 
int ledPin = 13;
// LM35 analog input
int lm35Pin = 0;

// replace with your channel's thingspeak API key
String apiKey = "T2RJXWQAVXG4ZV39";

// connect 2 to TX of Serial USB
// connect 3 to RX of serial USB
SoftwareSerial ser(2, 3); // RX, TX

// this runs once
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);    

  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  ser.begin(9600);
  
  // reset ESP8266
  sendData("AT+RST\r\n",5000,DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n",2000,DEBUG); // configure as station
  sendData("AT+CWLAP\r\n",10000,DEBUG); // get AP list
  sendData("AT+CWJAP=\"HiWiFi_Free\",\"openusing\"\r\n",15000,DEBUG); //connect to AP
  sendData("AT+CIPMUX=0\r\n",2000,DEBUG); //single connection mode
  sendData("AT+CIFSR\r\n",5000,DEBUG); // get ip address
}

/*
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    ser.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(ser.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = ser.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

// the loop 
void loop() {
  
  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);

  // read the value from LM35.
  // read 10 values for averaging.
  int val = 0;
  for(int i = 0; i < 10; i++) {
      val += analogRead(lm35Pin);   
      delay(500);
  }

  // convert to temp:
  // temp value is in 0-1023 range
  // LM35 outputs 10mV/degree C. ie, 1 Volt => 100 degrees C
  // So Temp = (avg_val/1023)*5 Volts * 100 degrees/Volt
  float temp = val*50.0f/1023.0f;

  // convert to string
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);
  
  Serial.println(strTemp);
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"192.168.4.2\",8080\r\n";//change
  sendData(cmd,5000,DEBUG);
  
  // prepare GET string
  String getStr = "ESP8266webtest/TempServlet/?field1="+String(strTemp)+"\r\n\r\n";

  //send data length
  cmd = "AT+CIPSEND=" + String(getStr.length()) + "\r\n";
  ser.print(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  sendData("AT+CIPCLOSE\r\n",3000,DEBUG);
    
  // thingspeak needs 15 sec delay between updates
  delay(5000);  
}
