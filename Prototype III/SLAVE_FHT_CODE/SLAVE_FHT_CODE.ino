/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/
#define LOG_OUT 0 // use the log output function
#define LIN_OUT 0
#define LIN_OUT8 0
#define OCTAVE 1
#define FHT_N 256
#define SCALE 1
#define OCT_NORM 1
#include <FHT.h>

#define SLAVE_ADDR 9
#include <Wire.h>

void setup() {

  Wire.begin(SLAVE_ADDR);

  Wire.onRequest(requestEvent);
  
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void requestEvent() {

  byte response[6];

  for (byte x = 0; x < 6; x++) {
    if (fht_oct_out[x + 2] > 254) response[x] = 254;
    else response[x] = fht_oct_out[x + 2];
  }

  Wire.write(response, sizeof(response));

}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
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
    fht_mag_octave(); // take the output of the fht
    sei();
    
  }
}
