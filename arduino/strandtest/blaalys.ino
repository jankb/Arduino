#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
char c=' ';

enum Part_T
{
  ALL,
  LEFT,
  RIGHT
};
void setAllToColor(uint32_t color, Part_T part = ALL);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

uint16_t onTime = 60;
uint16_t offTime = 100;
bool isRunning = true;



void loop() {
  if (isRunning)
  {
//  delay(250);
  setAllToColor(strip.Color(0,0,255), LEFT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), LEFT);
  delay(offTime);
  setAllToColor(strip.Color(0,0,255), LEFT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), LEFT);
  delay(offTime);
   setAllToColor(strip.Color(0,0,255), LEFT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), LEFT);
  delay(offTime);

  // delay(250);
  setAllToColor(strip.Color(0,0,255), RIGHT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), RIGHT);
  delay(offTime);
  
  setAllToColor(strip.Color(0,0,255), RIGHT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), RIGHT);
  delay(offTime);
  
  setAllToColor(strip.Color(0,0,255), RIGHT);
  delay(onTime);
  setAllToColor(strip.Color(0,0,0), RIGHT);
  delay(offTime);
  }

  if (Serial.available())
  {
   while(Serial.available() > 0)
   {
    onTime = Serial.parseInt();
    offTime = Serial.parseInt();
   }
   Serial.print("On time  : ");
   Serial.println(onTime);

   Serial.print("Off time : ");
   Serial.println(offTime);
    /*
     c = Serial.read();
    if (c == 'S')
    {
      Serial.write("Stop");
      isRunning = false;
    }
    if (c == 's')
    {
      Serial.write("start");
      isRunning = true;
    }
    Serial.write(c);
    */
  }

 /*
  m_readBlue = analogRead(A0) / 4;
  m_readGreen = analogRead(A1) / 4;
  m_readRed = analogRead(A2) / 4;
  uint32_t red = strip.Color(15,10,0);
  uint32_t color = strip.Color(m_readRed,m_readGreen,m_readBlue);
Serial.println(color);
 pos =  color / 557064;
Serial.println(pos);

   strip.setPixelColor(lastPos,strip.Color(0,m_readGreen,m_readBlue));
   strip.setPixelColor(pos,color);
   strip.show();
   lastPos = pos;
 
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    Serial.println(m_readRed);
    
    if (i < (strip.numPixels() / 2))
    {
    strip.setPixelColor(i,red);
    }
    else
    {
    strip.setPixelColor(i,color);
    }
    strip.show();
   
    
  }
  */
  
  /*
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  */
//  rainbowCycle(20);
 // theaterChaseRainbow(50);
  
}

void setAllToColor(uint32_t color, enum Part_T part)
 {
  uint16_t endLed = strip.numPixels();
  uint16_t startLed = 0;
  if (part == LEFT)
  {
    endLed = strip.numPixels() / 2;
  }
  if (part == RIGHT)
  {
    startLed = strip.numPixels() / 2;
  }
  
   for(uint16_t i=startLed; i<endLed; i++) 
   {
    strip.setPixelColor(i,color);
   }
    strip.show();
 }
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
