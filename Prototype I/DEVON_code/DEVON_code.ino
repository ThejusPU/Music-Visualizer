/**
 * LED Music Visualizer
 * by Devon Crawford
 * using the FastLED library: http://fastled.io/
 * April 22, 2018
 * Watch the video: https://youtu.be/lU1GVVU9gLU
 */
#include "FastLED.h"

#define NUM_LEDS 150        // How many leds in your strip?
#define updateLEDS 8        // How many do you want to update every millisecond?
#define COLOR_SHIFT 180000  // Time for colours to shift to a new spectrum (in ms)
CRGB leds[NUM_LEDS];        // Define the array of leds
CRGB temp[NUM_LEDS];        // Define array for preset lights

// Define the digital I/O PINS..
#define DATA_PIN 6          // led data transfer
#define PITCH_PIN 0         // pitch input from frequency to voltage converter
#define BRIGHT_PIN 4        // brightness input from amplified audio signal

// Don't touch these, internal color variation variables
unsigned long setTime = COLOR_SHIFT;
int mulC = 2;

int maximum = 0;
int minimum = 50;
int countmax = 0;
int countmin = 0;

// Define color structure for rgb
struct color {
  int r;
  int g;
  int b;
};
typedef struct color Color;

void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);
    pinMode(A0, INPUT);
    pinMode(A4, INPUT);
    
    for(int i = 0; i < NUM_LEDS; i++)
    {
    if(i < NUM_LEDS/2)
    {
      int green = 255 * setLeds(i);
      leds[i] = CRGB(green,0,255-green);
      temp[i] = CRGB(green,0,255-green);
    }
    else
    {
      int green = 255 * setLeds((int)(i-(NUM_LEDS/2)));
      leds[i] = CRGB(255-green,green,0);
      temp[i] = CRGB(255-green,green,0);
    }
  }
  FastLED.show();
    FastLED.show();
}

void loop() {

  // Shift all LEDs to the right by updateLEDS number each time
  for(int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - updateLEDS];
  }

  // Get the pitch and brightness to compute the new color
  int newPitch = (analogRead(PITCH_PIN));
  int bright = analogRead(BRIGHT_PIN);
  CRGB nc = pitchConv(newPitch, bright);

  // Set the left most updateLEDs with the new color
  for(int i = 0; i < updateLEDS; i++) {
    leds[i] = CRGB(nc.r, nc.g, nc.b);
  }
  FastLED.show();

  //printColor(nc);
  delay(1);
}

/**
 * Converts the analog brightness reading into a percentage
 * 100% brightness is 614.. about 3 volts based on frequency to voltage converter circuit
 * The resulting percentage can simply be multiplied on the rgb values when setting our colors,
 * for example black is (0,0,0) so when volume is off we get 0v and all colors are black (leds are off)
 */
double convBrightness(int b) {
  double c = b / 614.0000;
  if( c < 0.05 ) {
    c = 0;
  }
  if( c >= 0.05 && c < 0.2) {
    c = c * 1.5;
  }
  else if(c > 1) {
    c = 1.00;
  }
  return c;
}

/**
 * Creates a new color from pitch and brightness readings
 * int p         analogRead(pitch) representing the voltage between 0 and 5 volts
 * double b      analogRead(brightness) representing volume of music for LED brightness
 * returns Color structure with rgb values, which appear synced to the music
 */
CRGB pitchConv(int p, int b) {
  Color c;
  double bright = convBrightness(b);

  int loc = findLoc(p);
  return temp[loc];
}

int findLoc(int pitch)
{
  if(pitch < minimum)
  {
    minimum=pitch;
  }
  else if(countmin == 20)
  {
    minimum = minimum + 20;
    countmin = 0;
  }
  if(pitch > maximum)
  {
    maximum=pitch;
  }  
  else if(countmax == 20)
  {
    maximum = maximum - 20 ;
    countmax = 0;
  }
  countmax++;
  countmin++;
  double p = pitch;
  return (p-minimum)*NUM_LEDS/(maximum-minimum);
}

double setLeds(int i)
{
  double x = i;
  return 2*x/NUM_LEDS;
}

void setColor(Color *c, int r, int g, int b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

// Prints color structure data
void printColor(Color c) {
  Serial.print("( ");
  Serial.print(c.r);
  Serial.print(", ");
  Serial.print(c.g);
  Serial.print(", ");
  Serial.print(c.b);
  Serial.println(" )");
}
