#include <WiFiManager.h>
#include <SPI.h>

String testString;

void setup() {

  Serial.begin(115200);
  Serial.println("started");
  
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  wifiManager.resetSettings();
  
  // set custom ip for portal
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  WiFiManagerParameter custom_string("test string", "enter test string", "", 40);
  wifiManager.addParameter(&custom_string);

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("AutoConnectAP", "securePassword");

  testString = custom_string.getValue();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(testString);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  
  
  delay(10000);
}
