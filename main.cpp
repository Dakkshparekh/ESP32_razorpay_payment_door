#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP32Servo.h>

#define WLAN_SSID "YOUR_WIFI_SSID"
#define WLAN_PASS "YOUR_WIFI_PASSWORD"

#define AIO_SERVER     "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME   "YOUR_ADAFRUIT_USERNAME"
#define AIO_KEY        "YOUR_ADAFRUIT_KEY"
#define FeedName       "amount"

Servo myservo;
int pos = 0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe amount = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/" FeedName);

void setup() {
  Serial.begin(115200);
  myservo.attach(4);
  Serial.println(F("RazorPay Payment Operated Door Prototype"));
  Serial.print("Connecting to WiFi: ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&amount);
}

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe* subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &amount) {
      Serial.println("Payment received! Operating door...");
      for (pos = 0; pos <= 90; pos += 5) {
        myservo.write(pos);
        delay(15);
      }
      delay(5000);
      for (pos = 90; pos >= 0; pos -= 5) {
        myservo.write(pos);
        delay(15);
      }
      delay(5000);
    }
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      Serial.println("Failed to connect to MQTT. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("MQTT Connected!");
}
