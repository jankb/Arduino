#include <LiquidCrystal_I2C.h>



const int TRIG_PIN = 6;
const int ECHO_PIN = 7;
const int BUTTON_PIN = 5;

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
 

  startMillis = millis();
}
const int NUM_SAMPLES = 50;
unsigned long measurements [NUM_SAMPLES];



int sample = 0;
unsigned int average = 0;
unsigned int zeroing_distance = 0;
bool use_zeroing = false;

void loop() {
  long duration = 0;
  unsigned long cm = 0;
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH)
  {
    zeroing_distance = average;
    use_zeroing = !use_zeroing; // Toggle zeroing
  }
 

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
    //  Serial.print(measurements[i]);
    //  Serial.print(",");
    sum = sum + measurements[i];;

  }
  average = sum / NUM_SAMPLES;
  //  Serial.print(" Average : ");
  //  Serial.print(sum/NUM_SAMPLES);
  //   Serial.println("");

  //  Serial.print("cm ");
  //  Serial.print(cm);
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
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
  }

  //  Serial.println();

  delay(10);
}
