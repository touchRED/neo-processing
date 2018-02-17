#include <Adafruit_NeoPixel.h>
#include <math.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(90, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

byte colorCube[90][3];

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  double x;
  double y;
  double z;

  byte red;
  byte green;
  byte blue;

  for(uint8_t t = 0; t < 90; t++){
    
    //parameterization for x. were going to get 85 points, so we multiply t by 2pi/85 to cover the entire knot 
    
    x=.41*cos((t*2*M_PI)/90)-.18*sin((t*2*M_PI)/90)-.83*cos(2*(t*2*M_PI)/90)-.83*sin(2*(t*2*M_PI)/90)-.11*cos(3*(t*2*M_PI)/90)+.27*sin(3*(t*2*M_PI)/90);
//    x=x*1000.0;
//    x=x/1000.0;
    x*=2;
    x=x+3;//add 3 to shift right 3, make calues positive
    
    //repeating for y
    y=.36*cos((t*2*M_PI)/90)+.27*sin((t*2*M_PI)/90)-1.13*cos(2*(t*2*M_PI)/90)+.30*sin(2*(t*2*M_PI)/90)+.11*cos(3*(t*2*M_PI)/90)-.27*sin(3*(t*2*M_PI)/90);
//    y=y*1000;
//    y=y/1000.0;
    y*=2;
    y=y+3;//forward 3
    
    //repeating for z
    z=.45*sin((t*2*M_PI)/90)-.30*cos(2*(t*2*M_PI)/90)+1.13*sin(2*(t*2*M_PI)/90)-.11*cos(3*(t*2*M_PI)/90)+.27*sin(3*(t*2*M_PI)/90);
//    z=z*1000;
//    z=z/1000.0;
    z*=2;
    z=z+3;//up 3
    
    red=constrain(byte(floor(x*51)), 0, 255);
    green=constrain(byte(floor(y*51)), 0, 255);
    blue=constrain(byte(floor(z*51)), 0, 255);

    byte colors[3] = {red, green, blue};
    colorCube[t][0] = colors[0];
    colorCube[t][1] = colors[1];
    colorCube[t][2] = colors[2];
  }

  Serial.begin(9600);
  strip.begin();

  for(uint8_t i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(colorCube[i][0], colorCube[i][1], colorCube[i][2]));  
  }
  
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

//  if(Serial.available() >= 3){
//    
//    //Use Serial.readBytes() to read in 3 RGB values at a time
//    byte colors[3];
//    int numBytes = Serial.readBytes(colors, 3);
//
//    byte red = colors[0];
//    byte green = colors[1];
//    byte blue = colors[2];
//    
//    if(red >= 0 && blue >= 0 && green >= 0){
//      colorChange(strip.Color(red, green, blue));
//    }
//  }

  if(Serial.available()){
    byte pos = Serial.read();
    singlePosition(strip.Color(colorCube[pos][0], colorCube[pos][1], colorCube[pos][2]), pos);  
  }
  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Fill the dots all at once
void colorChange(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}


// Send a single pixel around the strip
void singlePixel(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(i > 0){
      strip.setPixelColor(i - 1, 0);
    }else{
      strip.setPixelColor(strip.numPixels() - 1, 0);
    }
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Light up a single pixel at a specified position
void singlePosition(uint32_t c, uint8_t pos) {
  if(pos < strip.numPixels()){
    for(uint16_t i = 0; i < strip.numPixels(); i++){
      if(i == pos){
        strip.setPixelColor(i, c);
      }else{
        strip.setPixelColor(i, 0);
      }  
    }
    strip.show();  
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
