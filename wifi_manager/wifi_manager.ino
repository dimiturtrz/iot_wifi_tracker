#include <WiFiManager.h>
#define SYSTEM_LED 2 //Define blinking LED pin

const char* logServer = "api.pushingbox.com";

String deviceId;
String objectName;


WiFiManager wifiManager;
WiFiManagerParameter deviceIdParameter("pushing box scenario device id", "enter device id", "", 40);
WiFiManagerParameter objectNameParameter("object name", "enter object name", "object", 40);

void setupWifiManager() {
  
  // In order to have the oportunity to set the devid (could be commented out)
  wifiManager.resetSettings();
  
  // to be certain where to look
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  wifiManager.addParameter(&deviceIdParameter);
  wifiManager.addParameter(&objectNameParameter);

  wifiManager.autoConnect("ESP8622TrackerAP", "securePassword");

  deviceId = deviceIdParameter.getValue();
  objectName = objectNameParameter.getValue();
}

void reconnectWifiManager() {
  Serial.println("reconnecting");
  wifiManager.autoConnect("ESP8622TrackerAP", "securePassword");

  deviceId = deviceIdParameter.getValue();
  objectName = objectNameParameter.getValue();
}

void manageConnection() {
  pinMode(SYSTEM_LED, (WiFi.status() == WL_CONNECTED) ? HIGH : LOW);
  
  if(WiFi.status() != WL_CONNECTED) {
    reconnectWifiManager();  
  }  
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
  manageConnection();
  
  Serial.println(objectName);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);

  
  if(!sentNotification && rssi < -80 ) {
    Serial.println("sending");
    sendNotification();
    sentNotification = true;
  }
  
  delay(10000);
}
