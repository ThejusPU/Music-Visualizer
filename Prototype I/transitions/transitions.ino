#include "FastLED.h"

#define NUM_LEDS 150
#define DATA_PIN 6

const float pi = 3.1415;

CRGB leds[NUM_LEDS];
CRGB temp[150];

int start = -1;

struct color
{
  int r;
  int g;
  int b;
};

typedef struct color Color;

void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(A0, INPUT);
  pinMode(A0, INPUT);

  int z = 1;
  while(z <= 3)
  {
    for(int i = 0; i < NUM_LEDS; i++)
    {
      if(z==1){
      leds[i] = CRGB(255,0,0);}
      else if(z==2){
      leds[i] = CRGB(0,255,0);}
      else{
      leds[i] = CRGB(0,0,255);}
    }
    FastLED.show();
    delay(500);
    z++;
  }

  for(int i = 0; i < 150; i++)
  {
    if(i < 50)
    {
      double x = i;
      double ratio = x / 49.0;
      int green = 255 * ratio;
      leds[i] = CRGB(green,0,255-green);
      temp[i] = CRGB(green,0,255-green);
    }
    else if(i < 100)
    {
      double x = i - 50;
      double ratio = pow(sin(x*pi/100), 2);
      int green = 255 * ratio;
      leds[i] = CRGB(255-green,green,0);
      temp[i] = CRGB(255-green,green,0);
    }
    else if(i < 150)
    {
      double x = i - 100;
      double ratio = x / 49.0;
      int green = 255 * ratio;
      leds[i] = CRGB(0, 255-green, green);
      temp[i] = CRGB(0, 255-green, green);
    }
  }
  FastLED.show();
  delay(50);
}

void loop()
{
  int first = findLoc();
  for(int i = 0; i < NUM_LEDS/2; i++)
  {
    leds[75 + i] = temp[(i + first) % 150];
    leds[74 - i] = temp[(i + first) % 150];
  }
  
  FastLED.show();
  delay(50);
}

int findLoc()
{
  start++;
  if(start == 150) {
    start = -1;
  }
  return start+1;
}
