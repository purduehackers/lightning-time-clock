#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <string.h>
using namespace std;
#include <DS3231.h>

DS3231 rtc(SDA, SCL);

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 73
// 32 + 25 + 16
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// setup() function -- runs once at startup --------------------------------

typedef struct lightning_time
{
  uint8_t sparks;
  uint8_t zaps;
  uint8_t bolts;
} LightningTime;

LightningTime convertToLightning(Time current)
{
  float millisPerSpark = 21093.75;

  Serial.println(current.hour);
  Serial.println(current.min);
  Serial.println(current.sec);
  uint32_t hour = current.hour;
  uint32_t min = current.min;
  uint32_t sec = current.sec;
  uint32_t hour_term = hour * 1000u * 60u * 60u;
  uint32_t min_term = min * 1000u * 60u;
  uint32_t sec_term = 1000u * sec;
  Serial.print("cast hour* 1000 * 60 * 60 "); Serial.println(hour * 1000u * 60u * 60u);
  Serial.print("cast min* 1000 * 60 "); Serial.println(min * 1000u * 60u);
  Serial.print("cast sec* 1000 "); Serial.println(sec * 1000u);
  Serial.print("millis"); Serial.println((1000u * 60u * 60u * hour) + (1000u * 60u * min) + (1000u * sec));
  Serial.print("hour_term"); Serial.println(hour_term);
  Serial.print("min_term"); Serial.println(min_term);
  Serial.print("sec_term"); Serial.println(sec_term);
  Serial.print("hour_term + min_term"); Serial.println(hour_term + min_term);
  Serial.print("hour_term + sec_term"); Serial.println(hour_term + sec_term);
  Serial.print("min_term + sec_term"); Serial.println(min_term + sec_term);
  Serial.print("hour_term + min_term + sec_term"); Serial.println(hour_term + min_term + sec_term);
  Serial.println("--");
  uint32_t millis = hour_term + min_term + sec_term;

  Serial.print("\nMillis per Spark: ");
  Serial.print(millisPerSpark);
  Serial.print("\nMillis: ");
  Serial.print(millis);
  Serial.print("\n");

  uint32_t totalSparks = (millis / millisPerSpark);
  uint32_t totalZaps = (totalSparks / 16);
  uint32_t totalBolts = (totalZaps / 16);

  uint8_t sparks = totalSparks % 16;
  uint8_t zaps = totalZaps % 16;
  uint8_t bolts = totalBolts % 16;

  return {sparks, zaps, bolts};
}

void setup()
{
  // rtc.setTime(3, 20, 33);
  // rtc.setDate(4, 2, 2023);

  Serial.begin(115200);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)

  rtc.begin();
}

int pix = 0;
int lpix = 0;
int angle = 0;

void loop()
{
  Time current = rtc.getTime();

  LightningTime lightning = convertToLightning(current);
  Serial.println("--");
  Serial.println(lightning.bolts);
  Serial.println(lightning.zaps);
  Serial.println(lightning.sparks);

  // angle = current.sec*60;

  // int outer = pointOuter(angle);
  // int inner = pointInner(outer);

  // Serial.print("O:");
  // Serial.println(outer);
  // Serial.print("I:");
  // Serial.println(inner);

  if (pix > 0)
  {
    lpix = pix;
  }

  strip.setPixelColor(lightning.bolts, strip.Color(255, 0, 0));
  strip.setPixelColor(lightning.zaps + 32, strip.Color(0, 255, 0));
  strip.setPixelColor(lightning.sparks + 56, strip.Color(0, 0, 255));
  strip.show();

  delay(1000);
  strip.setPixelColor(lightning.bolts, strip.Color(0, 0, 0));
  strip.setPixelColor(lightning.zaps + 32, strip.Color(0, 0, 0));
  strip.setPixelColor(lightning.sparks + 56, strip.Color(0, 0, 0));
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
