#include <LiquidCrystal_I2C.h>



const int TRIG_PIN = 6;
const int ECHO_PIN = 7;
const int BUTTON_PIN = 3;

LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;
int buttonState = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Initializing...");

  Serial.begin(9600);
  while (!Serial);

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), doZeroing, RISING );


  startMillis = millis();
}
const int NUM_SAMPLES = 50;
unsigned long measurements [NUM_SAMPLES];



int sample = 0;
volatile unsigned int average = 0;
volatile unsigned int zeroing_distance = 0;
volatile bool use_zeroing = false;
volatile bool graphics_dirty = false;

void loop() {
  long duration = 0;
  unsigned long cm = 0;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);


  duration = pulseIn(ECHO_PIN, HIGH);
  cm = duration / 29 / 2;

  if (sample < NUM_SAMPLES)
  {
    measurements[sample++] = cm;
  }
  else
  {
    sample = 0;
  }

  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    sum = sum + measurements[i];
  }
  average = sum / NUM_SAMPLES;
 
  currentMillis = millis();
  if ((currentMillis - startMillis >= period) ||
       graphics_dirty == true)
  {
    lcd.clear();

    lcd.setCursor(2, 0);
    lcd.print("cm:");
    lcd.setCursor(2, 1);

    if (use_zeroing)
    {
      int z_dist = zeroing_distance - cm;
      lcd.print((z_dist < 0) ? 0 : z_dist);
      lcd.setCursor(0, 0);
      lcd.print("z");
    }
    else
    {
      lcd.print(cm);
    }


    lcd.setCursor(8, 0);
    lcd.print("Average:");
    lcd.setCursor(10, 1);
    lcd.print(average);

    startMillis = currentMillis;
    graphics_dirty = false;
  }

  delay(10);
}

void doZeroing()
{
  use_zeroing = !use_zeroing;
  zeroing_distance = average;
  graphics_dirty = true;
}
