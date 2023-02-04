#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <string.h>
using namespace std;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

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

void setup() {
  Serial.begin(9600);
  Serial.println("ready");

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)

}

int pix=0;
int lpix=0;
int angle=0;

//String outerst = "O:";
//String innerst = "I:";
  
void loop() {
  Serial.println("Enter number");

  while (Serial.available() == 0) {
  }
  
  angle = Serial.parseInt();
  Serial.println(angle);

  int outer = pointOuter(angle);
  int inner = pointInner(outer);

  Serial.print("O:");
  Serial.println(outer);
  Serial.print("I:");
  Serial.println(inner);

  if(pix>0){
    lpix = pix;
  }
  
  strip.setPixelColor(outer, strip.Color(251, 203, 59));
  strip.setPixelColor(inner, strip.Color(251, 203, 59));
  delay(100);
  
  strip.show();
  strip.setPixelColor(outer, strip.Color(0,0,0));
  strip.setPixelColor(inner, strip.Color(0,0,0));
  
  
}

// outer 32
// middl 25 
// inner 16

int pointOuter(int heading){
  int outer=(int)ceil(heading/11.25);
  return outer;
}

int pointInner(int outer){
  int inner=(outer/2)+57;
  return inner;
}

//int pointMiddle(int heading){
//  int middle=;
//  return middle;
//}
