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
}

void loop()
{
  uint32_t huenow;
  const uint32_t hue_green = 21845;
  pixels.clear(); // Set all pixel colors to 'off'

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

}
