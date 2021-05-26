/* Written by Rupak Poddar */

#include <FastLED.h>               // https://github.com/FastLED/FastLED
#define NUMLEDS 144                // Number of LEDs
#define DATA_PIN 3                   // Connect your addressable LED strip to this pin.
#define SENSITIVITY 300           // Ranges from 0 to 1023
#define MAX_BRIGHTNESS 200  // Ranges from 0 to 255
#define ENVELOPE_PIN A0         // Connect sound detector to this pin
#define SATURATION 150          // Ranges from 0 to 255
#define MINVAL 60
#define HUE_INIT 10
#define HUE_CHANGE 1

CRGB leds[NUMLEDS];
byte brightness[NUMLEDS];
byte hue[NUMLEDS];
int analogVal;
int DELAY;

void setup() { 
  Serial.begin(9600);
  pinMode(ENVELOPE_PIN, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMLEDS);

  for(int i = 0; i <= NUMLEDS; i++){
    brightness[i] = 0;
    hue[i] = 0;
  }

  //Turn off all the LEDs
  for(int i=0; i <= NUMLEDS; i++)  
  {
  leds[i] = CRGB::Black;
  }

  //Update the LED strip
  FastLED.show(); 
}

void loop() {
  analogVal = analogRead(ENVELOPE_PIN);

  if(analogVal > SENSITIVITY)
  analogVal = SENSITIVITY;

  if(analogVal < MINVAL)
  analogVal = 0;
   
/*----------------------------------------------------------
  - 5 styles for sound reactive led strip are given below in different lines.
  - Uncomment the function which you want to try and comment the others.
  - "LinearFlowing" is uncommented by default.          
-----------------------------------------------------------*/

   LinearFlowing();
  // LinearReactive();
  // BrightnessReactive();
  // CentreProgressive();
  // EdgeProgressive();
  
  FastLED.show();
}

void LinearFlowing(){
  byte val = map(analogVal, 0, SENSITIVITY+1, 0, MAX_BRIGHTNESS);
  DELAY = map(analogVal, 0, SENSITIVITY+1, 20, 1);
  
  for(int i = 0; i <= NUMLEDS; i++){
    brightness[NUMLEDS-i] = brightness[NUMLEDS-i-1];
  }
  
  for(int i = 0; i <= NUMLEDS; i++){
    hue[NUMLEDS-i] = hue[NUMLEDS-i-1];
  }
  
  brightness[0] = val;
  byte hue = HUE_INIT;
  for(int i = 0; i <= NUMLEDS; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, brightness[i]);
  }
  delay(DELAY);
}

void LinearReactive(){
  byte val = map(analogVal, 0, SENSITIVITY+1, 0, NUMLEDS);
  byte hue = HUE_INIT;
 
  for(int i = 0; i <= val; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHTNESS);
  }

  for(int i = val+1; i <= NUMLEDS; i++){
    leds[i].nscale8(10);
  }
}

void BrightnessReactive(){
  byte val = map(analogVal, 0, SENSITIVITY+1, 0, MAX_BRIGHTNESS);
  byte hue = HUE_INIT;
  for(int i = 0; i <= NUMLEDS; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, val);
  }
}

void CentreProgressive(){
  byte val = map(analogVal, 0, SENSITIVITY, 0, NUMLEDS/2);
  byte hue = HUE_INIT;
  for(int i = 0; i <= val; i++){
    leds[(NUMLEDS/2)+i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHTNESS);
    leds[(NUMLEDS/2)-i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHTNESS);
  }

  for(int i = val+1; i <= (NUMLEDS/2); i++){
    leds[(NUMLEDS/2)+i].nscale8(10);
    leds[(NUMLEDS/2)-i].nscale8(10);
  }
}

void EdgeProgressive(){
  byte val = map(analogVal, 0, SENSITIVITY, 0, NUMLEDS/2);
  byte hue = HUE_INIT;
  for(int i = 0; i <= val; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHTNESS);
    leds[NUMLEDS-i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHTNESS);
  }

  for(int i=val+1; i<=(NUMLEDS/2); i++){
    leds[i].nscale8(10);
    leds[NUMLEDS-i].nscale8(10);
  }
}
