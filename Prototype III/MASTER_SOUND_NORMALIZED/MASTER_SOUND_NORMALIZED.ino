/**
 * These are the values from the normalized silence values
 * the mean values in silence are:
 * 127.87   111.89    85.79   56.64   28.38   28.98
 * the standard deviation in silence are:
 * 9.99     7.69      6.11    4.65    2.88    16.37   
 * 
 * We are going to use these values to normalize the z-score of
 * loud music.
 * 
 * Resulting mean values are
 * 6.40     8.56      12.54   19.06   31.89   4.42
 * 6.85     8.73      12.73   19.30   32.02   3.40
 * 7.56     9.30      13.11   18.63   29.44   3.51
 * and stdDev Values are 
 * 1.29     1.43      2.16    4.16    6.77    1.73
 * 1.17     1.39      1.83    2.56    4.82    0.91
 * 1.31     1.56      2.27    3.74    7.77    1.37
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


double mean[ANSWERSIZE];
double stdDev[ANSWERSIZE];

double meanSound[ANSWERSIZE];
double stdDevSound[ANSWERSIZE];

int mode;
int count;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  mean[0] = 127.87;
  mean[1] = 111.89;
  mean[2] = 85.79;
  mean[3] = 56.64;
  mean[4] = 28.38;
  mean[5] = 28.98;

  stdDev[0] = 9.99;
  stdDev[1] = 7.69;
  stdDev[2] = 6.11;
  stdDev[3] = 4.65;
  stdDev[4] = 2.88;
  stdDev[5] = 16.37;

  mode = 0;
  count = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  double oct_Values[ANSWERSIZE];                     //contains octaves normalized by silent data

  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  int i = 0;
  while (Wire.available()) {
    oct_Values[i] = (int) Wire.read();
    oct_Values[i] = (oct_Values[i] - mean[i]) / stdDev[i];
    i++;
    if (i >= ANSWERSIZE) break;
  }
 
  switch (mode) {
    case 0:
      for (int x = 0; x < ANSWERSIZE; x++) {
        meanSound[x] = meanSound[x] + oct_Values[x];
      }
      count++;
      Serial.println(count);
      if (count == 500) {
        Serial.println("Mean Values");
        for (int x = 0; x < ANSWERSIZE; x++) {
          meanSound[x] = meanSound[x] / 500;
          Serial.print(meanSound[x]);
          Serial.print("\t");
        }
        Serial.println();
        mode++;
        count = 0;
      }
      break;
    case 1:
      for (int x = 0; x < ANSWERSIZE; x++) {
        stdDevSound[x] = stdDevSound[x] + pow(meanSound[x] - oct_Values[x], 2);
      }
      count++;
      Serial.println(count);
      if (count == 500) {
        Serial.println("StdDev Values");
        for (int x = 0; x < ANSWERSIZE; x++) {
          stdDevSound[x] = pow(stdDevSound[x] / 500, 0.5);
          Serial.print(stdDevSound[x]);
          Serial.print("\t");
        }
        Serial.println();
        mode++;
        count = 0;
      }
      break;
    default:
      
      Serial.print(oct_Values[0]);
      Serial.print("\t");
      Serial.print(oct_Values[1]);
      Serial.print("\t");
      Serial.print(oct_Values[2]);
      Serial.print("\t");
      Serial.print(oct_Values[3]);
      Serial.print("\t");
      Serial.print(oct_Values[4]);
      Serial.print("\t");
      Serial.println(oct_Values[5]);
      break;
  }
}
