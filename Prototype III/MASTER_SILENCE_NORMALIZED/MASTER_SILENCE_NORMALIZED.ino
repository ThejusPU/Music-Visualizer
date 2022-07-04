/**
 * nrmalized the silence.
 * the mean values in silence are:
 * 138.08   111.89    85.79   56.64   28.38   28.98
 * the standard deviation in silence are:
 * 9.99     7.69      6.11    4.65    2.88    16.37   
 */
#include <Wire.h>
#include "FastLED.h"

#define SLAVE_ADDR 9
#define ANSWERSIZE 6

#define NUM_LEDS 150
#define WALL_LEDS 43
#define NUM_COLORS 90
#define DATA_PIN 8

CRGB leds[NUM_LEDS];
CRGB undr[NUM_COLORS];

int mode;
int count;

double mean[ANSWERSIZE];
double stdDev[ANSWERSIZE];

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  mode = 0;
  count = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  int oct_Values[ANSWERSIZE];

  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  int i = 0;
  while (Wire.available()) {
    oct_Values[i] = (int) Wire.read();
    i++;
    if (i >= ANSWERSIZE) break;
  }

  switch (mode) {
    case 0:
      for (int x = 0; x < ANSWERSIZE; x++) {
        mean[x] = mean[x] + oct_Values[x];
      }
      count++;
      if (count == 1000) {
        Serial.println("Mean Values");
        for (int x = 0; x < ANSWERSIZE; x++) {
          mean[x] = mean[x] / 1000;
          Serial.print(mean[x]);
          Serial.print("\t");
        }
        Serial.println();
        mode++;
        count = 0;
      }
      break;
    case 1:
      for (int x = 0; x < ANSWERSIZE; x++) {
        stdDev[x] = stdDev[x] + pow(mean[x] - oct_Values[x], 2);
      }
      count++;
      if (count == 1000) {
        Serial.println("StdDev Values");
        for (int x = 0; x < ANSWERSIZE; x++) {
          stdDev[x] = pow(stdDev[x] / 1000, 0.5);
          Serial.print(stdDev[x]);
          Serial.print("\t");
        }
        Serial.println();
        mode++;
        count = 0;
      }
      break;
    default:
      
      Serial.print((oct_Values[0] - mean[0]) / stdDev[0]);
      Serial.print("\t");
      Serial.print((oct_Values[1] - mean[1]) / stdDev[1]);
      Serial.print("\t");
      Serial.print((oct_Values[2] - mean[2]) / stdDev[2]);
      Serial.print("\t");
      Serial.print((oct_Values[3] - mean[3]) / stdDev[3]);
      Serial.print("\t");
      Serial.print((oct_Values[4] - mean[4]) / stdDev[4]);
      Serial.print("\t");
      Serial.println((oct_Values[5] - mean[5]) / stdDev[5]);
      break;
  }
}
