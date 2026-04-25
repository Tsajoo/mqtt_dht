#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "YOUR_SSID";
const char* pass = "YOUR_PASSWORD";

// Add your MQTT credentials
const char* mqtt_user = "your_mqtt_username";
const char* mqtt_password = "your_mqtt_password";

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  client.setServer("mqtt-dashboard.com", 8884);
  client.setCredentials(mqtt_user, mqtt_password);   // <-- ADD THIS LINE
  client.connect("lele");
}

void loop() {
  if (!client.connected()) {
    // Reconnect with credentials
    client.setCredentials(mqtt_user, mqtt_password); // also needed here
    client.connect("ESP32_MQTT");
  }
  client.loop();

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) {
    char tempStr[8], humStr[8];
    dtostrf(t, 1, 2, tempStr);
    dtostrf(h, 1, 2, humStr);
    client.publish("ESP32/T", tempStr);
    client.publish("ESP32/H", humStr);
  }
  delay(1000);
}