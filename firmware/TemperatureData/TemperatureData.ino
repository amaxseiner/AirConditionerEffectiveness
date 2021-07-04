// Alex Maxseiner

#include <stdio.h>
#include "wifiOLEDlib.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

#include <WiFiUdp.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 myOled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <SimpleDHT.h>
int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22);

float temperature = 0;
float humidity = 0;
int errDHT22 = SimpleDHTErrSuccess;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long wifiEpoch = 0;

char lcdBuffer[80];
String oledline[9];
char data[80];
int status = WL_IDLE_STATUS;

WiFiUDP Udp;

char packetBuffer[256];

IPAddress ip(192, 168, 1, 156);

unsigned int localPort = 5005;

boolean sendData = false;


void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  oledline[1] = "Temperature Data";
  int jj; for (jj=2; jj<=8; jj++){ oledline[jj]=""; }

  // Make sure to use the correct I2C address. Address 0x3C for 128x64
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!myOled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }

  // check for the WiFi module:
  oledline[2] = "Wifi status..."; displayTextOLED(oledline);
  oledline[3] = "";
  oledline[4] = "";
  oledline[5] = "";
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // attempt to connect to Wifi network:  
  oledline[2] = "Connect to Wifi..."; displayTextOLED(oledline);
  status = WiFi.begin(ssid, pass); delay(5000);
  startMillis = millis();
  while (status != WL_CONNECTED) {    
    Serial.print("Attempting to connect to WiFi: ");
    status = WiFi.begin(ssid, pass);    
    delay(250);
  }
  startMillis = millis();  //initial start time
  Udp.begin(localPort);
  printWifiStatus();
}

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    IPAddress remoteIp = Udp.remoteIP();
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    sendData = true;
  }

  currentMillis = millis();

  
  //*************************************//
  //*** Start OLED Display **************//
  //*************************************//
  if( (currentMillis - startMillis >= 10000) && sendData==true){ //Display every second
     
      wifiEpoch = WiFi.getTime();
      // display data on OLED
      sprintf(lcdBuffer, "%lddBm %d.%d.%d.%d", WiFi.RSSI(), WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
      oledline[2] = String(lcdBuffer);
      convDDHHMMSS(currentMillis/1000, lcdBuffer); 
      oledline[7] = String(lcdBuffer);
      convCurrentTimeET(wifiEpoch, lcdBuffer); 
      oledline[8] = String(lcdBuffer);
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

      getMacWifiShield(lcdBuffer); 
      oledline[6] = "MAC " + String(lcdBuffer);

      errDHT22 = dht22.read2(&temperature, &humidity, NULL);
      if (errDHT22 != 0) {
        temperature = (-1 - 32)/1.8;
        humidity = -1;
      }
      sprintf(lcdBuffer, "Temp:     %.2f C", (float)temperature); 
      oledline[4] = lcdBuffer;
      sprintf(lcdBuffer, "Humidity: %.2f RH%%", (float)humidity);
      oledline[5] = lcdBuffer;
      displayTextOLED(oledline);
      sprintf(data, "Data %.2f %.2f", (float)temperature,(float)humidity);
      Serial.println(data);
      // send data to server
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(data);
      Udp.endPacket();
      

  }
  //*************************************//
  //***  End OLED Display ***************//
  //*************************************//
  
}


void displayTextOLED(String oledline[]) {
  int jj;
  myOled.clearDisplay();
  myOled.setTextSize(1);
  myOled.setTextColor(SSD1306_WHITE);
  myOled.setCursor(0, 0);
  for (jj=1; jj<=8; jj++) { 
    myOled.println(oledline[jj]);
  }
  myOled.display();  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
