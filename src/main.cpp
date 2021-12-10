#include <Arduino.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 0 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1 // Time (in milliseconds) to pause between pixels

#define SIP_INDEX_MAX 32  //number of points in the source intensity profile
uint8_t sip[SIP_INDEX_MAX]; // array to store the source intensity profile

uint8_t pixel_position[] = {51,102,153,204}; // led virtual position between 0...255

void init_sip() {
  uint8_t i, j;
  float ratio, jsquared;

  j = SIP_INDEX_MAX / 2;
  jsquared = j*j;
  
  for (i=0; i<j; i++) {
    ratio = i * i / jsquared;
    sip[i] = 255 * ratio;
  }

  for (i=j; i<SIP_INDEX_MAX; i++) {
    ratio = (i-SIP_INDEX_MAX-1) * (i-SIP_INDEX_MAX-1) / jsquared;
    sip[i] = 255 * ratio;
  }
}

uint8_t pixel_intensity(uint8_t pixel_index, int sip_origin){ // pixel index 0..NUMPIXELS-1
  int sip_index;

  sip_index = (int)pixel_position[pixel_index] - sip_origin;

  if (sip_index<0) return 0; // negative origins default to 0 intensity
  if (sip_index >= SIP_INDEX_MAX) return 0; // origins beyond SIP bounds default to 0 intensity
  return sip[sip_index];
}

void setup()
{
  // These lines are specifically to try overclocking @ 16Mhz,
  // not officially supported at 3.3V, but has been known to work

/*
F_CPU == 16000000;
clock_prescale_set(clock_div_1);
*/
  // END of overclocking code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'

  init_sip();
}

void loop()
{
  static uint16_t step=0;
  int source_origin = 0;

  uint32_t huenow;
  const uint32_t hue_green = 21845;

  if (step<256) source_origin = step;  // first move origin to the right with each step
  else if (step<511) source_origin = 510-step; // when the origin hits 256 move it to the left
  else {  // wrap step back to 0 after 511 steps
    source_origin = 0;
    step = 0;
  }  

for (uint8_t i = 0; i<NUMPIXELS; i++){
  pixels.setPixelColor(i,pixels.ColorHSV(hue_green,255,pixel_intensity(i,source_origin)));
}
/*
  huenow=0;
  while (huenow < hue_green)
  {
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.

    pixels.fill(pixels.gamma32(pixels.ColorHSV(huenow)));
    pixels.show(); // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
    huenow += 4;
  }


  while (huenow > 0)
  {
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.

    pixels.fill(pixels.gamma32(pixels.ColorHSV(huenow)));
    pixels.show(); // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
    huenow -= 4;
  }
*/
}

