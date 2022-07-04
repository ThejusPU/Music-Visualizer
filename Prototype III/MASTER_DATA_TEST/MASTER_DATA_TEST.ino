/**
 * Code for the Master device which takes frequency information from Slave device
 * and prints it out ensuring the I2C communication is working.
 * 
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


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {
  // put your main code here, to run repeatedly:

  int oct_Values[ANSWERSIZE];

  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  int count = 0;
  while (Wire.available()) {
    oct_Values[count] = (int) Wire.read();
    count++;
    if (count >= ANSWERSIZE) break;
  }

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
}
