#include <SoftwareSerial.h>
#include <stdlib.h>

#define DEBUG true


int lm35Pin = 0; // LM35 analog input


SoftwareSerial ser(2, 3); // RX, TX
void setup() {

Serial.begin(9600); // enable debug serial
ser.begin(9600); // enable software serial
// reset ESP8266
sendData("AT+RST\r\n",5000,DEBUG); // reset module
sendData("AT+CWMODE=1\r\n",2000,DEBUG); // configure as station
sendData("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n",15000,DEBUG);
sendData("AT+CIPMUX=0\r\n",2000,DEBUG); //single connection mode

}
String sendData(String command, const int timeout, boolean debug)
{
String response = "";
ser.print(command); // send the read character to the esp8266
long int time = millis();
while( (time+timeout) > millis())
{
while(ser.available())
{
char c = ser.read(); // read the next character.
response+=c;
}
} if(debug)
{
Serial.print(response);
} return response;
}
void loop() {
// read the value from LM35.read 10 values for averaging.
int val = 0;
for(int i = 0; i < 10; i++) {
val += analogRead(lm35Pin);
delay(500);
} // 转换成温度数据
float temp = val*50.0f/1023.0f;
// convert to string
char buf[16];
String strTemp = dtostrf(temp, 4, 1, buf);
Serial.println(strTemp);
// 和Web服务器建立连接
String cmd = "AT+CIPSTART=\"TCP\",\"10.18.55.240\",8080\r\n";
sendData(cmd,5000,DEBUG);
// 准备 GET string
String getStr = "GET /update/?field1="+String(strTemp)+"\r\n\r\n";
//发送数据长度
cmd = "AT+CIPSEND=" + String(getStr.length()) + "\r\n";
ser.print(cmd);
if(ser.find('>')){
//发送数据
ser.print(getStr);
} 
delay(15000);
}
