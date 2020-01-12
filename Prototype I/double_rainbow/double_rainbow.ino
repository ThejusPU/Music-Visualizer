#include "FastLED.h"

#define NUM_LEDS 150
#define DATA_PIN 6
#define PITCH_PIN 0
#define BRIGHT_PIN 4

const float pi = 3.1415;

CRGB leds[NUM_LEDS];
CRGB temp[NUM_LEDS];

int maximum = 0;
int minimum = 50;
int countmax = 0;
int countmin = 0;
double reduction = .92;

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

  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(i < NUM_LEDS/4)
    {
      int green = 255 * setLeds(i);
      leds[i] = CRGB(green,0,255-green);
      temp[i] = CRGB(green,0,255-green);
    }
    else if(i < NUM_LEDS/2)
    {
      int green = 255 * setLeds((int)(i-(NUM_LEDS/4)));
      leds[i] = CRGB(255-green,green,0);
      temp[i] = CRGB(255-green,green,0);
    }
    else if(i < 3*NUM_LEDS/4)
    {
      int green = 255 * setLeds((int)(i-(NUM_LEDS/2)));
      leds[i] = CRGB(green,255-green,0);
      temp[i] = CRGB(green,255-green,0);
    }
    else
    {
      int blue = 255 * setLeds((int)(i-(3*NUM_LEDS/4)));
      leds[i] = CRGB(255-blue,0,blue);
      temp[i] = CRGB(255-blue,0,blue);
    }
  }
  FastLED.show();
  delay(500);
}

void loop()
{
  int pitch = analogRead(PITCH_PIN);
  double bright = convBrightness((int)analogRead(BRIGHT_PIN));
  if(pitch < minimum)
  {
    minimum=pitch;
  }
  else if(countmin == 20)
  {
    minimum = minimum + 40;
    countmin = 0;
  }
  if(pitch > maximum)
  {
    maximum=pitch;
  }  
  else if(countmax == 20)
  {
    maximum = maximum - 40 ;
    countmax = 0;
  }
  countmax++;
  countmin++;

  Serial.println(pitch);

  for(int i = 0; i < NUM_LEDS; i++)
  {
    CRGB light = leds[i];
    leds[i] = CRGB(light.r*reduction, light.g*reduction, light.b*reduction);
  }

  int loc = findLoc(pitch);
  int mid = NUM_LEDS / 2;
  for(int i = -30*bright; i <= 30*bright; i++)
  {
    if(mid + loc + i < NUM_LEDS && mid + loc + i >= 0)
    {
      CRGB light = temp[mid+loc+i];
      double fact = abs(i)/(30*bright);
      if(CRGB(light.r*fact, light.g*fact, light.b*fact).getLuma() > leds[mid+loc+i].getLuma())
      {
        leds[mid+loc+i] = CRGB(light.r*fact, light.g*fact, light.b*fact);
      }
    }
    if(mid - loc + i < NUM_LEDS && mid - loc + i >= 0)
    {
      CRGB light = temp[mid-loc+i];
      double fact = abs(i)/(30*bright);
      if(CRGB(light.r*fact, light.g*fact, light.b*fact).getLuma() > leds[mid-loc+i].getLuma())
      {
        leds[mid-loc+i] = CRGB(light.r*fact, light.g*fact, light.b*fact);
      }
    }
  }
  
  FastLED.show();
  delay(50
  
  );
}

double convBrightness(int b)
{
  double c = b / 614.0000;
  if( c < 0.05 )
  {
    c = 0;
  }
  if( c >= 0.05 && c < 1)
  {
    c = pow(c,.5);
  }
  else if(c > 1)
  {
    c = 1.00;
  }
  return c;
}

int findLoc(int pitch)
{
  double p = pitch;
  return (p-minimum)*NUM_LEDS/(2*(maximum-minimum));
}

double setLeds(int i)
{
  double x = i;
  return 4*x/NUM_LEDS;
}






