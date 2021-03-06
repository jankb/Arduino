
#include "DHT.h"
#define DHTPIN D3        // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;

//#define WLAN_SSID       ""
//#define WLAN_PASS       ""
#include <wificred.h>

#include <Wire.h>
#include <BMP180I2C.h>
#define I2C_ADDRESS 0x77
//create an BMP180 object using the I2C interface
BMP180I2C bmp180(I2C_ADDRESS);
bool bmp180_found = false;

//#define DEBUG 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Serial.print("\n\nSketch:   ");   Serial.println(__FILE__);
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


  Wire.begin();

  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");

  }
  else
  {
    bmp180_found = true;
  }

  if (bmp180_found)
  {
    bmp180.resetToDefaults();
    bmp180.setSamplingMode(BMP180MI::MODE_UHR);
  }
}



void loop() {
  float tmp = 0.0;
  float pressure = 0.0;
  if (bmp180_found)
  {
    fromBMP(pressure, tmp);
    delay(100);
    fromBMP(pressure, tmp);
    /*
      Serial.print("\nBMP180 Temperature: ");
      Serial.println(tmp);
      Serial.print("\nBMP180 Pressure: ");
      Serial.println(pressure);
    */
  }

  float t = dht.readTemperature();
  if (isnan(t) == false)
  {
    Serial.print("Temperature : ");
    Serial.println(t);

  }
  String temperature = "temp=" + String(tmp);
  String prs =  "&pressure=" + String(pressure);



  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  String url("http://www.polvott.net/iot/met.cgi?" + temperature + prs);
  Serial.printf("[HTTP] url : %s\n", url.c_str());
  if (http.begin(client, url ))
  {
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
#ifdef DEBUG
    int httpCode = HTTP_CODE_NOT_FOUND; 
#else
    int httpCode = http.GET();
#endif

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
#ifdef DEBUG
  delay(1000);
#else
  ESP.deepSleep(10 * 60e6);
  delay(10000);
#endif

}

void fromBMP(float &pressure, float &temperature)
{
  if (!bmp180.measureTemperature())
  {
    Serial.println("could not start temperature measurement, is a measurement already running?");
    return;
  }
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  temperature = bmp180.getTemperature();
  if (!bmp180.measurePressure())
  {
    Serial.println("could not start perssure measurement, is a measurement already running?");
    return;
  }

  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  pressure = bmp180.getPressure() * 0.01;
}
