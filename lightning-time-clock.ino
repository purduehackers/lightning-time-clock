#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <string.h>
using namespace std;
#include <DS3231.h>

DS3231 rtc(SDA, SCL);

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN 6

#define LED_COUNT 97
// 32 + 25 + 16 + 24

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

typedef struct lightning_time
{
  uint8_t charges;
  uint8_t sparks;
  uint8_t zaps;
  uint8_t bolts;
} LightningTime;

LightningTime convertToLightning(Time current)
{
  // float millisPerSpark = 21093.75;
  float millisPerCharge =  1318.359375;

  Serial.println(current.hour);
  Serial.println(current.min);
  Serial.println(current.sec);
  uint32_t hour = current.hour;
  uint32_t min = current.min;
  uint32_t sec = current.sec;
  uint32_t hour_term = hour * 1000u * 60u * 60u;
  uint32_t min_term = min * 1000u * 60u;
  uint32_t sec_term = 1000u * sec;
  uint32_t millis = hour_term + min_term + sec_term;
  uint32_t totalCharges = (millis / millisPerCharge);
  uint32_t totalSparks = (totalCharges / 16);
  uint32_t totalZaps = (totalSparks / 16);
  uint32_t totalBolts = (totalZaps / 16);

  uint8_t charges = totalCharges % 16;
  uint8_t sparks = totalSparks % 16;
  uint8_t zaps = totalZaps % 16;
  uint8_t bolts = totalBolts % 16;

  return {charges, sparks, zaps, bolts};
}

void setup()
{

  Serial.begin(115200);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)

  rtc.begin();
  // rtc.setTime(3, 35, 30);
  // rtc.setDate(18, 2, 2023);
}

int pix = 0;
int lpix = 0;
int angle = 0;

// all variables for mapping lightning units to rgb
int input_start = 0;    // The lowest number of the range input.
int input_end = 15;    // The largest number of the range input.
int output_start = 0; // The lowest number of the range output.
int output_end = 255;  // The largest number of the range output.
int outputr= 0;
int outputg= 0;
int outputb= 0;

void loop()
{
  Time current = rtc.getTime();

  LightningTime lightning = convertToLightning(current);
  Serial.println("--");
  Serial.println(lightning.bolts);
  Serial.println(lightning.zaps);
  Serial.println(lightning.sparks);

  if (pix > 0)
  {
    lpix = pix;
  }

  int outputr = output_start + ((output_end - output_start) / (input_end - input_start)) * (lightning.bolts - input_start);
  int outputg = output_start + ((output_end - output_start) / (input_end - input_start)) * (lightning.zaps - input_start);
  int outputb = output_start + ((output_end - output_start) / (input_end - input_start)) * (lightning.sparks - input_start);

  if (lightning.bolts == 0) {
      for (int i = 0; i < 32; i++) {
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
  }
  for (int i = 0; i < (lightning.bolts * 2) + 2; i++) {
    strip.setPixelColor(i, strip.Color(outputr, 161, 0));
  }
  if (lightning.zaps == 0) {
    for (int i = 32; i < 57; i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
  }
  for (float i = 32.0; i < (lightning.zaps * 1.5) + 33; i++) {
    if (24 % lightning.zaps != 0) {
      strip.setPixelColor((int) i, strip.Color(50, outputg, 214));
      strip.setPixelColor(ceil((lightning.zaps * 1.5) + 32), strip.Color(12, ceil(outputg / 4), 54));
    } else {
      strip.setPixelColor(i, strip.Color(50, outputg, 214));
    }
  }
  if (lightning.sparks == 0) {
    for (int i = 56; i < 73; i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
  }
  for (int i = 56; i < lightning.sparks + 57; i++) {
    strip.setPixelColor(i, strip.Color(246, 133, outputb));
  }
  if (lightning.charges == 0) {
    for (float i = 72; i < 96; i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
  }
  for (int i = 72; i < lightning.charges + 73; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  // strip.setPixelColor(lightning.bolts * 2, strip.Color(outputr, 161, 0));
  // strip.setPixelColor(lightning.zaps + 32, strip.Color(50, outputg, 214));
  // strip.setPixelColor(lightning.sparks + 56, strip.Color(246, 133, outputb));
  strip.show();

  // delay(1000);
  // strip.setPixelColor(lightning.bolts, strip.Color(0, 0, 0));
  // strip.setPixelColor(lightning.zaps + 32, strip.Color(0, 0, 0));
  // strip.setPixelColor(lightning.sparks + 56, strip.Color(0, 0, 0));
}

// outer 32
// middl 25
// inner 16

int pointOuter(int heading)
{
  int outer = (int)ceil(heading / 11.25);
  return outer;
}

int pointInner(int outer)
{
  int inner = (outer / 2) + 57;
  return inner;
}

// int pointMiddle(int heading){
//   int middle=;
//   return middle;
// }
