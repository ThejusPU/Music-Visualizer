#include <Wire.h>
#include "FastLED.h"

#define SLAVE_ADDR 9
#define ANSWERSIZE 6

#define NUM_LEDS 150
#define NUM_COLORS 90
#define DATA_PIN 8
#define COLOR_SPEED 3
#define STAR_SPEED 2

CRGB leds[NUM_LEDS];
uint8_t reds[NUM_COLORS];
uint8_t blues[NUM_COLORS];
uint8_t greens[NUM_COLORS];
uint16_t stars[NUM_LEDS];
uint16_t maxStar[NUM_LEDS];


// Look at MASTER_SOUND_NORMALIZED for info
float mean[ANSWERSIZE] = {127.87, 111.89, 85.79, 56.64, 28.38, 28.98};
float stdDev[ANSWERSIZE] = {9.99, 7.69, 6.11, 4.65, 2.88, 16.37};
/*
  class Star {

  private:
    short posLED;
    short totalTime;
  //    int cI;   //colorIndex
    short timeAccu;

  public:
    Star() {
      this->posLED = -1;
      this->totalTime = (int) random(500, 1000);
      this->timeAccu = 0;
    }

    void decrementTime(long change) {
      this->timeAccu = this->timeAccu + change;
    }

    int getPos() {
      return this->posLED;
    }

    int resetPos() {
      this->posLED = -1;
    }

    int setPos(int pos) {
      this->posLED = pos;
      this->totalTime = (int) random(500, 1000);
      this->timeAccu = 0;
    }

    CRGB getColor() {
      return undr[cI];
    }

    float getPercent() {
      return abs(this->timeAccu - (this->totalTime / 2)) / (this->totalTime / 2);
    }

    int getTimeLeft() {
      return this->totalTime - this->timeAccu;
    }
  };
*/

float oct_Values[ANSWERSIZE];                     //contains octaves normalized by silent data

float currM[ANSWERSIZE];
float currSD[ANSWERSIZE];
int sampleSize;
int color;
int colorReg;
int starReg;
int side;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  for (int x = 0; x < ANSWERSIZE; x++) {
    currM[x] = 0;
    currSD[x] = 0;
  }
  sampleSize = 0;

  int phase = -1;
  double temp;
  double redSF = 0.7;
  for (int x = 0; x < NUM_COLORS; x++) {
    if (x % 15 == 0) phase++;
    temp = ((float)(x - (phase * 15))) / 15;
    switch (phase) {
      case 0:
        reds[x] = 255 * redSF;
        blues[x] = temp * 255;
        greens[x] = 0;
        break;
      case 1:
        reds[x] = (255 - 255 * temp) * redSF;
        blues[x] = 255;
        greens[x] = 0;
        break;
      case 2:
        reds[x] = 0;
        blues[x] = 255;
        greens[x] = 255 * temp;
        break;
      case 3:
        reds[x] = 0;
        blues[x] = 255 - 255 * temp;
        greens[x] = 255;
        break;
      case 4:
        reds[x] = (255 * temp) * redSF;
        blues[x] = 0;
        greens[x] = 255;
        break;
      case 5:
        reds[x] = 255 * redSF;
        blues[x] = 0;
        greens[x] = 255 - 255 * temp;
        break;
    }
  }
  //FastLED.show();

  for (int x = 0; x < NUM_LEDS; x++) {
    stars[x] = 0;
    maxStar[x] = 0;
  }

  color = 0;
  colorReg = 0;
  starReg = 0;
  side = 1;
}

void starsOnStrip() {
  int pos;
  float percent;
  int colorPos;
  for (int x = 0; x < NUM_LEDS; x++) {
    percent = 0.03;
    colorPos = color + x;
    if (x >= (NUM_LEDS / 2)) colorPos = color + (NUM_LEDS - x);
    if (colorPos >= NUM_COLORS) colorPos -= NUM_COLORS;
    if (stars[x] != 0) percent = ((float)stars[x]) / (3 * (float)maxStar[x]);
    leds[x] = CRGB(reds[colorPos] * percent, blues[colorPos] * percent, greens[colorPos] * percent);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  int i = 0;
  while (Wire.available()) {
    oct_Values[i] = (int) Wire.read();
    i++;
    if (i >= ANSWERSIZE) break;
  }

  //  int percentSum = 0;
  for (i = 0; i < ANSWERSIZE; i++) {
    oct_Values[i] = ((oct_Values[i] - mean[i]) / stdDev[i]);
    if (oct_Values[i] < 1) oct_Values[i] = 0;
    if (sampleSize < 500) sampleSize++;
    currM[i] = ((sampleSize - 1) * currM[i] + oct_Values[i]) / sampleSize;
    currSD[i] = sqrt(((sampleSize - 1) * pow(currSD[i], 2) + pow(oct_Values[i] - currM[i], 2)) / (sampleSize));
    oct_Values[i] = oct_Values[i] / (currM[i] + (2 * currSD[i]));
    if (oct_Values[i] > 1) oct_Values[i] = 1;
    //    percentSum += oct_Values[i];
  }

  /*  for (int x = 1; x < ANSWERSIZE; x++) {
      Serial.print(oct_Values[x]);
      Serial.print("\t");
    }
    Serial.println();

    for (int x = 1; x < ANSWERSIZE; x++) {
      Serial.print("\t");
      Serial.print(currM[x]);
    }
    Serial.println();

    for (int x = 1; x < ANSWERSIZE; x++) {
      Serial.print("\t");
      Serial.print(currSD[x]);
    }*/
  if (sampleSize == 500) {

    int randPaul = random(100);
    int sum = 0;
    int x = 1;
    while (x < ANSWERSIZE) {
      Serial.print(oct_Values[x]);
      Serial.print("\t");
      sum += oct_Values[x] * 100 / 5;
      if (sum >= randPaul) break;
      x++;
    }
    Serial.print("\n");

    if (x != ANSWERSIZE) {
      randPaul = (NUM_LEDS / 2) + ((random(NUM_LEDS / 10) + ((x - 1) * 15)) * side);
      side *= -1;
      boolean success = false;
      do {
        if (stars[randPaul] == 0) {
          stars[randPaul] = random(25, 50);
          maxStar[randPaul] = stars[randPaul];
          success = true;
        }
        randPaul = random(NUM_LEDS);
      } while (success == false);
    }
    
  }
  starsOnStrip();
//  leds[NUM_LEDS / 2] = CRGB(255, 255, 255);
  FastLED.show();

  colorReg++;
  starReg++;
  if (starReg == STAR_SPEED) {
    for (int x = 0; x < NUM_LEDS; x++) {
      if (stars[x] != 0) stars[x] -= 1;
    }
    starReg = 0;
  }
  if (colorReg == COLOR_SPEED) {
    color++;
    colorReg = 0;
  }
  if (color == NUM_COLORS) color = 0;
}
