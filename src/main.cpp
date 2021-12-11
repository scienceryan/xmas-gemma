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
int left_bound = 1 - SIP_SIZE;               // sets the left bound for the source position
int right_bound = 96;                        //sets the right bound for the source position
uint16_t position_range = right_bound - left_bound;

uint8_t pixel_position[] = {0, 32, 64, 96}; // led virtual position between 0...255

uint8_t pixel_intensity(uint8_t pixel_index, int sip_origin)
{ // pixel index 0..NUMPIXELS-1
  int sip_index;

  sip_index = (int)pixel_position[pixel_index] - sip_origin;

  if (sip_index < 0)
    return 0; // negative origins default to 0 intensity
  if (sip_index >= SIP_SIZE)
    return 0; // origins beyond SIP bounds default to 0 intensity
  return sip[sip_index];
}

void setup()
{
  // These lines are specifically to try overclocking @ 16Mhz,
  // not officially supported at 3.3V, but has been known to work
#ifdef __SERIALDEBUG__
  delay(5000);
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.print("SIP_SIZE=");
  Serial.println(SIP_SIZE);
  Serial.print("left_bound=");
  Serial.println(left_bound);
  Serial.print("right_bound=");
  Serial.println(right_bound);
  Serial.print("position_range=");
  Serial.println(position_range);
#endif

  /*
F_CPU == 16000000;
clock_prescale_set(clock_div_1);
*/
  // END of overclocking code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}

void loop()
{
  static uint16_t step = 0;
  static uint16_t midpoint_step = position_range / 2;
  int source_origin = 0;

  const uint32_t hue_green = 21845; //green hue
  const uint32_t hue_red = 0; //red hue
  static uint32_t huenow = hue_green;

  if (step < midpoint_step)
    source_origin = left_bound + step * 2; // first move origin to the right with each step
  else if (step < position_range+1)
    source_origin = right_bound - (step - midpoint_step) * 2; // when the origin hits 256 move it to the left

#ifdef __SERIALDEBUG__
  Serial.print(step);
  Serial.print(":");
  Serial.print(source_origin);
  Serial.print(" ");
#endif

  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.ColorHSV(huenow, 255, pixel_intensity(i, source_origin)));
#ifdef __SERIALDEBUG__
    Serial.print(i);
    Serial.print(",");
    Serial.print(pixel_intensity(i, source_origin));
    Serial.print(" ");
#endif
  }
#ifdef __SERIALDEBUG__
  Serial.println();
#endif
  pixels.show();
  step++;
  if (step > position_range)
  {
    step = 0;
    if (huenow == hue_green) huenow = hue_red;
    else huenow = hue_green;
  }

#ifdef __DELAYVAL__
  delay(__DELAYVAL__);
#endif

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
