//ESP8266 Arduino Tutorial - Push notification messages example


#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "v2E04B3E716236D2"; //replace with deviceid from pushingbox account
const char* logServer = "api.pushingbox.com";

const char* ssid = "Terzievi"; //replace with your WiFi credentials
const char* password = "tetradka"; //replace with your WiFi password


boolean sentNotification = false;

void setup() {
  Serial.begin(115200);
  Serial.println("startup");
}

void sendNotification(String message){

  Serial.println("- connecting to Home Router SID: " + String(ssid));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");

    String getString = "GET /pushingbox?devid=" + 
                       String(deviceId) + 
                       "&object_name=" + 
                       String(message) + 
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
  }
  
  client.stop();
  Serial.println("- stopping the client");
}

void loop() {
  Serial.println("begining");
  if(!sentNotification) {
    Serial.println("sending");
    sendNotification("ball");
    sentNotification = true;
  }
  delay(500000000);
}
