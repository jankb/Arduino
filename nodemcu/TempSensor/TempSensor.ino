
#include "DHT.h"
#define DHTPIN 4        // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;

//#define WLAN_SSID       ""
//#define WLAN_PASS       ""
#include <wificred.h>


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
  Serial.println();

  dht.begin();
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


}


// the loop function runs over and over again forever
void loop() {
  // wait for a second

  float t = dht.readTemperature();
  if (isnan(t) == false)
  {
    Serial.print("Temperature : ");
    Serial.println(t);

  }
  String temperature = "temp=" + String(t);

  

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  String url("http://www.polvott.net/iot/met.cgi?" + temperature + "&pressure=0");
  Serial.printf("[HTTP] url : %s\n", url.c_str());
  if (http.begin(client, "http://www.polvott.net/iot/met.cgi?" + temperature + "&pressure=0" ))
  {


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode =  http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }


  Serial.println("Going to sleep...");
  ESP.deepSleep(5*60e6);
  delay(1000);

}
