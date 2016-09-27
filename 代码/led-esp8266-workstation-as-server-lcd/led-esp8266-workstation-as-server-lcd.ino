#include <SoftwareSerial.h>
#include <PCD8544.h>

#define DEBUG true

#define PIN_CE   8 //Pin 3 on LCD
#define PIN_RST 7 //Pin 4 on LCD
#define PIN_DC    6 //Pin 5 on LCD
#define PIN_DIN  5 //Pin 6 on LCD
#define PIN_CLK  4 //Pin 7 on LCD
#define PIN_BL 12 //background LED

SoftwareSerial esp8266(2, 3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
// This means that you need to connect the TX line from the esp to the Arduino's pin 2
// and the RX line from the esp to the Arduino's pin 3

// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;

static PCD8544 lcd(4, 5, 6, 7, 8);

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different

  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  pinMode(12, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  analogReference(INTERNAL);

  digitalWrite(12, HIGH);

  String res = "";

  sendData("AT+RST\r\n", 5000, DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n", 2000, DEBUG); // configure as station
  sendData("AT+CWLAP\r\n", 10000, DEBUG); // get AP list
  sendData("AT+CWJAP=\"HiWiFi_Free\",\"openusing\"\r\n", 15000, DEBUG); //connect to AP
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,800\r\n", 1000, DEBUG); // turn on server on port 80

  res = sendData("AT+CIFSR\r\n", 5000, DEBUG); // get ip address
  displayIPAddr(res);

  delay(1000);
  digitalWrite(12, LOW);
}

void displayLCD(String command, String s)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (s.indexOf("OK") != -1)
    lcd.print(command + ": OK");
  else
    lcd.print(command + ": Failure");
}

void displayIPAddr(String s) {
  int p1 = s.indexOf("+CIFSR:STAIP,\"") + 14;
  char ip[16];
  int idx = 0;
  while (s[p1 + idx] != '"') {
    ip[idx] = s[p1 + idx];
    idx++;
  }
  ip[idx] = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ip);
}

void loop()
{
  if (esp8266.available()) // check if the esp is sending a message
  {
    if (esp8266.find("+IPD,"))
    {
      delay(1000); // wait for the serial buffer to fill up (read all the serial data)
      // get the connection id so that we can then disconnect
      int connectionId = esp8266.read() - 48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48

      String response = "";
      while (esp8266.available())
      {
        // The esp has data so display its output to the serial window
        char c = esp8266.read(); // read the next character.
        response += c;
      }

      //Serial.println(response);

      if (response.indexOf("#LED:ON#") != -1)
        digitalWrite(13, HIGH);
      else if (response.indexOf("#LED:OFF#") != -1)
        digitalWrite(13, LOW);
      else
        Serial.println("Bad command");

      // make close command
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendData(closeCommand, 1000, DEBUG); // close connection
    }
  }
  delay(1000);
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

  esp8266.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
    lcd.clear();
    lcd.setCursor(0, 0);
    if (response.indexOf("OK") != -1)
      lcd.print(command + ": OK");
    else
      lcd.print(command + ": Failure");
  }

  return response;
}
