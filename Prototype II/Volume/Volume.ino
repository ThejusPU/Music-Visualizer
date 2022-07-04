
#include "FastLED.h"

const int sampleWindow = 10; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define NUM_LEDS 150
#define NUM_COLORS 90
#define DATA_PIN 8

CRGB leds[NUM_LEDS];
CRGB undr[NUM_COLORS];
double low;
int coloroffset;
int numCount;
 
void setup() 
{
   Serial.begin(9600);

   low = 100;
   coloroffset = 0;
   numCount = 0;
   FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  int phase = -1;
  int temp;
  for (int x = 0; x < NUM_COLORS; x++) {
    if (x % 15 == 0) phase++;
    temp = (x - (phase * 15)) / 15;
    switch (phase) {
      case 0:
      undr[x] = CRGB(255, temp * 255, 0);
      break;
      case 1:
      undr[x] = CRGB(255 - 255*temp, 255, 0);
      break;
      case 2:
      undr[x] = CRGB(0, 255, 255*temp);
      break;
      case 3:
      undr[x] = CRGB(0, 255 - 255*temp, 255);
      break;
      case 4:
      undr[x] = CRGB(255*temp, 0, 255);
      break;
      case 5:
      undr[x] = CRGB(255, temp * 255, 255);
      break;
    }
    if (x == 0) {
      undr[x] = CRGB(255, 255, 255);
    }
  }
}

void setLEDS(CRGB color) {
  for(int x = 0; x < NUM_LEDS; x++) {
    leds[x] = color;
  }
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = ((peakToPeak) * 5.0) / 1024;  // convert to volts
   Serial.println(volts);
   volts /= 4;
   if (volts > 1) volts = 1;
   if (volts < 0.1) volts = 0.1;
   

   int volume = (NUM_LEDS / 2) * volts;
   int color;
  for (int x = 0; x < (NUM_LEDS / 2); x++) {
    if (x <= volume) {
      color = ((double)x / ((double)volume)) * NUM_COLORS;
      color += coloroffset;
      if (color >= NUM_COLORS) {
        color -= NUM_COLORS;
      }
      if (x == 0) color = 0;
      leds[(NUM_LEDS / 2) + x] = undr[color];
      leds[(NUM_LEDS / 2) - x - 1] = undr[color];
    }
    else {
      leds[(NUM_LEDS / 2) + x] = CRGB(0, 0, 0);
      leds[(NUM_LEDS / 2) - x - 1] = CRGB(0, 0, 0);
    }
  }
    coloroffset++;
    if (coloroffset >= NUM_COLORS) {
      coloroffset = 0;
    }


  FastLED.show();
}
