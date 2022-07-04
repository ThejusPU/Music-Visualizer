#include <Wire.h>
#include "FastLED.h"

#define SLAVE_ADDR 9
#define ANSWERSIZE 6

#define NUM_LEDS 150
#define WALL_LEDS 42
#define NUM_COLORS 90
#define DATA_PIN 8

CRGB leds[NUM_LEDS];
CRGB undr[NUM_COLORS];


double mean[ANSWERSIZE] = {127.87, 111.89, 85.79, 56.64, 28.38, 28.98};
double stdDev[ANSWERSIZE] = {9.99, 7.69, 6.11, 4.65, 2.88, 16.37};

//{7.87, 9.86, 14.38, 22.22, 38.79, 5.15}
double maximum[ANSWERSIZE] = {7.87, 9.86, 14.38, 22.22, 38.79, 5.15};
int individualBlock;
double increment;
int offset;
int now;
double redSF = 0.7;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  int phase = -1;
  double temp;
  for (int x = 0; x < NUM_COLORS; x++) {
    if (x % 15 == 0) phase++;
    temp = ((double)(x - (phase * 15))) / 15;
    Serial.println(temp);
    switch (phase) {
      case 0:
      undr[x] = CRGB(255 * redSF, temp * 255, 0);
      break;
      case 1:
      undr[x] = CRGB((255 - 255*temp) * redSF, 255, 0);
      break;
      case 2:
      undr[x] = CRGB(0, 255, 255*temp);
      break;
      case 3:
      undr[x] = CRGB(0, 255 - 255*temp, 255);
      break;
      case 4:
      undr[x] = CRGB((255*temp) * redSF, 0, 255);
      break;
      case 5:
      undr[x] = CRGB(255 * redSF, 0, 255 - 255*temp);
      break;
    }
//    if (x == 0) {
//      undr[x] = CRGB(255, 255, 255);
//    }
  }

  increment = ((double) 2 * NUM_COLORS) / ((double) 3 * NUM_LEDS - WALL_LEDS - 1);
  individualBlock = WALL_LEDS / ANSWERSIZE;
  offset = 0;
  now = millis();
  

//  leds[WALL_LEDS] = CRGB(255, 255, 255);
  FastLED.show();
}

void setBar(double values[]) {
//  for(int x = 0; x < WALL_LEDS; x++) {
//    leds[x] = CRGB(255, 255, 255);
//  }
  int index;
  for(int x = WALL_LEDS + 1; x < NUM_LEDS ; x++) {
    index = (int)(((double)x - WALL_LEDS - 1) * increment) + offset;
    if (index >= NUM_COLORS) index -= NUM_COLORS;
    leds[x] = undr[index];
  }
  if (millis() - now > 500) {
    offset++;
    now = millis();
  }
  if (offset == NUM_COLORS) offset = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  double oct_Values[ANSWERSIZE];                     //contains octaves normalized by silent data

  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  int i = 0;
  while (Wire.available()) {
    oct_Values[i] = (int) Wire.read();
    oct_Values[i] = ((oct_Values[i] - mean[i]) / stdDev[i]) - 1;
    //Serial.print(oct_Values[i]);
    if (oct_Values[i] < 0) oct_Values[i] = 0;
    oct_Values[i] = oct_Values[i] / maximum[i];
    if (oct_Values[i] > 1) oct_Values[i] = 1;
    //Serial.print("\t");
    i++;
    if (i >= ANSWERSIZE) break;
  }
  //Serial.print(analogRead(0));
  //Serial.println();

  setBar(oct_Values);
  FastLED.show();
}
