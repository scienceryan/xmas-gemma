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
#define NUMPIXELS 4

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// array to store the source intensity profile
uint8_t sip[] =
    {0,
     0,
     1,
     2,
     4,
     6,
     9,
     12,
     16,
     20,
     25,
     30,
     36,
     42,
     49,
     56,
     64,
     72,
     81,
     90,
     100,
     110,
     121,
     132,
     143,
     156,
     168,
     182,
     195,
     209,
     224,
     239,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     255,
     239,
     224,
     209,
     195,
     182,
     168,
     156,
     143,
     132,
     121,
     110,
     100,
     90,
     81,
     72,
     64,
     56,
     49,
     42,
     36,
     30,
     25,
     20,
     16,
     12,
     9,
     6,
     4,
     2,
     1,
     0};

int SIP_SIZE = sizeof(sip) / sizeof(sip[0]); //number of points in the source intensity profile
int led_step[NUMPIXELS];
uint16_t led_hue[NUMPIXELS]; // hue of each pix
#define HUE_GREEN 21845
#define HUE_RED 0

uint8_t rand8bit()
{
  // LFSR using Galois agorithm for 8-bit  x^8+x^6+x^5+x^4+x^0
  uint8_t lsb;
  uint8_t start_state = 0x1f; /* Any nonzero start state will work. */
  static uint8_t lfsr = start_state;

  lsb = lfsr & 1u; /* Get LSB (i.e., the output bit). */
  lfsr >>= 1;      /* Shift register */
  if (lsb)         /* If the output bit is 1, */
    lfsr ^= 0xB8;  /*  apply toggle mask. */

  return lfsr;
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
#ifdef __SERIALDEBUG__
  delay(5000);
#endif
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'

  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    led_step[i] = -1; // -1 indicates not activated
    led_hue[i] = HUE_GREEN;
  }
}

void loop()
{
  uint8_t diceroll;

  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    if (led_step[i] < 0)
    {                        // if not activated, roll the dice to activate
      diceroll = rand8bit(); // 0..255

      if (diceroll < 5)
      {
        led_step[i] = 0;
        if (diceroll & 1u)
          led_hue[i] = HUE_GREEN;
        else
          led_hue[i] = HUE_RED;
      }
    }
    if (led_step[i] >= 0)
      pixels.setPixelColor(i, pixels.ColorHSV(led_hue[i], 255, sip[led_step[i]]));
  }

#ifdef __SERIALDEBUG__
  Serial.print(led_step[0]);
  Serial.print(" ");
  Serial.print(led_step[1]);
  Serial.print(" ");
  Serial.print(led_step[2]);
  Serial.print(" ");
  Serial.println(led_step[3]);
#endif

  pixels.show();

  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    if (led_step[i] >= 0)  // only step activated LEDs
    {
      ++led_step[i];
      if (led_step[i] > SIP_SIZE - 1) // if cycled through the whole profile
        led_step[i] = -1;
    }
  }

#ifdef __DELAYVAL__
  delay(__DELAYVAL__);
#endif
}
