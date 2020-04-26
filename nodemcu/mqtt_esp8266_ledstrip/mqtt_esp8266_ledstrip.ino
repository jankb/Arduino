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
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

//#define WLAN_SSID       ""
//#define WLAN_PASS       ""
#include <wificred.h>

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.1.138"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/v3/onoff");

Adafruit_MQTT_Subscribe redLevel   = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/v3/redLevel");
Adafruit_MQTT_Subscribe greenLevel = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/v3/greenLevel");
Adafruit_MQTT_Subscribe blueLevel  = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/v3/blueLevel");

Adafruit_MQTT_Subscribe rgbLevel  = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/v3/rgb");



void MQTT_connect();



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  Serial.println(F("NeoPixelBus/MQTT demo"));

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

  
  mqtt.subscribe(&onoffbutton);
  mqtt.subscribe(&redLevel);
  mqtt.subscribe(&greenLevel);
  mqtt.subscribe(&blueLevel);
  mqtt.subscribe(&rgbLevel);


  strip.Begin();
  strip.Show(); // Initialize all pixels to 'off'
  
}


uint8_t red_level   = 0;
uint8_t green_level = 0;
uint8_t blue_level  = 0;


bool isStripOn = true;

void colorWipe(RgbColor c, uint8_t wait) {
  for(uint16_t i=0; i<strip.PixelCount(); i++) {
    strip.SetPixelColor(i, c);
    strip.Show();
    Serial.print('s');
    delay(wait);
  }
  Serial.println('*');
}

void loop() {

  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &onoffbutton) 
    {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      if (strcmp((char *)onoffbutton.lastread, "ON") == 0)
       {
         digitalWrite(LED_BUILTIN, LOW); 
         isStripOn = true;
       }
       else
       {
         digitalWrite(LED_BUILTIN, HIGH); 
         colorWipe(RgbColor(0, 0, 0), 1);
         isStripOn = false;
       }
    }
    if (subscription == &redLevel) 
    {
       Serial.print(F("RED Got: "));
       red_level = atoi((char *)redLevel.lastread);
      Serial.println(red_level);
      
    }
    if (subscription == &greenLevel) 
    {
       Serial.print(F("GREEN Got: "));
       green_level = atoi((char *)greenLevel.lastread);
      Serial.println(green_level);
      
    }
    if (subscription == &blueLevel) 
    {
       Serial.print(F("BLUE Got: "));
       blue_level = atoi((char *)blueLevel.lastread);
      Serial.println(blue_level);
      
    }
     if (subscription == &rgbLevel) 
    {
       Serial.print(F("RGB Got: "));
      Serial.println((char*)rgbLevel.lastread);
      uint32_t inputv = atoi((char *)rgbLevel.lastread);
      red_level = (inputv >> 16);
      green_level = (inputv >> 8);
      blue_level = (inputv);
      
      
    }
    if (isStripOn)
    {
      colorWipe(RgbColor(red_level, green_level, blue_level), 1);
    }
  }
  
  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
