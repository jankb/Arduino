#include <dummy.h>
#include <NeoPixelBus.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

const uint16_t PixelCount = 30;
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
#define colorSaturation 128
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);


#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever



#include <ESP8266WiFi.h>

/************************* WiFi Access Point *********************************/

//#define WLAN_SSID       ""
//#define WLAN_PASS       ""
#include <wificred.h>



// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(115200);
  delay(10);

  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  Serial.println(F("NeoPixelBus/HTTP Server"));

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();


  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());





  strip.Begin();
  strip.Show(); // Initialize all pixels to 'off'

}


uint8_t red_level   = 0;
uint8_t green_level = 0;
uint8_t blue_level  = 0;


bool isStripOn = true;

void colorWipe(RgbColor c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    strip.SetPixelColor(i, c);
    strip.Show();
    Serial.print('s');
    delay(wait);
  }
  Serial.println('*');
}

void loop() {


  /*
        Serial.print(F("RGB Got: "));
        Serial.println((char*)rgbLevel.lastread);
        uint32_t inputv = atoi((char *)rgbLevel.lastread);
        red_level = (inputv >> 16);
        green_level = (inputv >> 8);
        blue_level = (inputv);
  */



}
