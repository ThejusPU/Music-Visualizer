/*
  
*/
#include "FastLED.h"

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#define NUM_LEDS 150
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
CRGB undr[12 * 6];


#include <FHT.h> // include the library

void setup() {
  Serial.begin(115200); // use the serial port

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  int z = 1;
  while (z <= 3) {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (z == 1) {
        leds[i] = CRGB(255, 0, 0);
      }
      else if (z == 2) {
        leds[i] = CRGB(0, 255, 0);
      }
      else {
        leds[i] = CRGB(0, 0, 255);
      }
    }
    FastLED.show();
    delay(500);
    z++;
  }

  int phase = 0;
  int temp;
  for (int x = 0; x < 12 * 6; x++) {
    if (x % 12) phase++;
    temp = (x - (phase * 12)) / 12;
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
  }


  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while (1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while (!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht

    
    updateLEDS();
    sei();
  }
}

void updateLEDS() {

  double octaves[6];
  double LED[6];
  findOctaves(octaves);

  int val;
  int octSize = NUM_LEDS / 6;
  float avg;
  for(int x = 0; x < 6; x++) {
    val = (int)(octaves[x] * octSize / (-32 * x + 192));
    if (val > octSize) val = octSize;
    float a = 0.25 * val;
    if (a > 1) a = 1;
    if (a < 0.05) a = 0.2;
    LED[x] = a;
    avg += LED[x];
  }

  for (int x = 0; x < 6; x++) {
    LED[x] /= avg;
  }

  avg /= 6;

  int octCount = 0;
  int prev = 0;
  int total = avg * (NUM_LEDS / 2);
  int color;
  for (int x = 0; x < (NUM_LEDS / 2); x++) {
    if (x < total) {
      color = (12 * octCount) + ((x - prev) / LED[octCount] * total);
      leds[(NUM_LEDS / 2 ) + x] = undr[color];
      leds[(NUM_LEDS / 2 ) - 1 - x] = undr[color];
    }
    else {
      leds[x] = CRGB(0, 0, 0);
    }
    if (x == prev + total * LED[octCount]) {
      octCount++;
      prev += total * LED[octCount];
    }
  }

  FastLED.show();
}

void findOctaves(double * octaves) {
  int count = 0;
  int octCount = 0;
  for (int x = 2; x <= 129; x++) {
    if (x == 5 || x == 9 || x == 17 || x == 33 || x  == 65) {
      octaves[octCount] = (octaves[octCount] / count);
      octCount++;
      count = 0;
    }
    octaves[octCount] += fht_log_out[x];
    count++;
  }
  octaves[octCount] /= count;
}
