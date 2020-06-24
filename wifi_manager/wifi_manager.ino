#include <WiFiManager.h>

const char* logServer = "api.pushingbox.com";

String deviceId;
String objectName;

void setupWifiManager() {
  
  WiFiManager wifiManager;
  
  // In order to have the oportunity to set the devid (could be commented out)
  wifiManager.resetSettings();
  
  // to be certain where to look
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  WiFiManagerParameter deviceIdParameter("pushing box scenario device id", "enter device id", "", 40);
  wifiManager.addParameter(&deviceIdParameter);
  WiFiManagerParameter objectNameParameter("object name", "enter object name", "object", 40);
  wifiManager.addParameter(&objectNameParameter);

  wifiManager.autoConnect("ESP8622TrackerAP", "securePassword");

  deviceId = deviceIdParameter.getValue();
  objectName = objectNameParameter.getValue();
}

void sendNotification(){
  
  Serial.println();
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));

  while(!client.connect(logServer, 80)) {
    Serial.println("- problem connecting");
  }

  Serial.println("- succesfully connected");

  String getString = "GET /pushingbox?devid=" + 
                     String(deviceId) + 
                     "&object_name=" + 
                     String(objectName) + 
                     " HTTP/1.1";
  
  Serial.println(getString);
  Serial.println("- sending data...");

  client.println(getString);
  client.println(String("Host: ") + logServer);
  client.println("Connection: close \n");

  Serial.println("[Response:]");
  while (client.connected() || client.available())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }
  
  client.stop();
  Serial.println("- stopping the client");
}

void setup() {
  Serial.begin(115200);
  setupWifiManager();
}


boolean sentNotification = false;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(objectName);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  if(!sentNotification /*&& rssi < -80*/ ) {
    Serial.println("sending");
    sendNotification();
    sentNotification = true;
  }
  delay(10000);
}
