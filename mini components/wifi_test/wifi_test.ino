#include <ESP8266WiFi.h>

//SSID of your network
char ssid[] = "HUAWEI P20 lite";
//password of your WPA Network
char pass[] = "4396ba54ae2f";

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Before connecting");
  WiFi.begin(ssid, pass);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  WiFi.softAP("testNetwork", "1234");
}

void loop () {
  /*long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  delay(1000);*/
}
