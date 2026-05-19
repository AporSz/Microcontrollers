/*
  Modified from Random Nerd Tutorials
  ESP8266 NodeMCU MQTT - Publish BME280 Sensor Readings (Arduino IDE)
*/

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> // Changed to BME280 library

#define WIFI_SSID "Mateinfo"
#define WIFI_PASSWORD "computer"

// Mosquitto MQTT Broker (Replace with your local or cloud broker IP)
#define MQTT_HOST IPAddress(192, 168, 39, 55)
// #define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// BME280 MQTT Topics
#define MQTT_PUB_TEMP "esp/bme280/temperature"
#define MQTT_PUB_HUM  "esp/bme280/humidity"
#define MQTT_PUB_PRES "esp/bme280/pressure"
// (Gas topic removed because BME280 doesn't have a gas sensor)

Adafruit_BME280 bme; // I2C

// Variables to hold sensor readings
float temperature;
float humidity;
float pressure;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings (10 seconds)

void getBME280Readings() {
  // BME280 reads sequentially without the asynchronous begin/end of the BME680
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0;
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print(" packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialize the BME280 sensor (Default I2C address is usually 0x76 or 0x77)
  // if (!bme.begin(0x76)) { 
  //   Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
  //   while (1);
  // }

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  
  // If your broker requires authentication (username and password), set them below
  // mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  
  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Every X number of seconds (interval = 10 seconds) it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Get new readings from the BME280
    getBME280Readings();
    
    Serial.println();
    Serial.printf("Temperature = %.2f ºC \n", temperature);
    Serial.printf("Humidity = %.2f %% \n", humidity);
    Serial.printf("Pressure = %.2f hPa \n", pressure);

    // Publish an MQTT message on topic esp/bme280/temperature
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temperature).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i \n", MQTT_PUB_TEMP, packetIdPub1);

    // Publish an MQTT message on topic esp/bme280/humidity
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(humidity).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i \n", MQTT_PUB_HUM, packetIdPub2);

    // Publish an MQTT message on topic esp/bme280/pressure
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_PRES, 1, true, "MESSAGE");
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i \n", MQTT_PUB_PRES, packetIdPub3);
  }
}