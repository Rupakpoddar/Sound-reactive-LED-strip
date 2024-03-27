#include <FastLED.h>
#include <AudioAnalyzer.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    7
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    60
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120

#define VERBOSE 0

Analyzer Audio = Analyzer(4 /* Strobe pin ->4 */, 5 /* RST pin ->5 */, 0 /* Analog Pin ->0 */);

int freqVal[7];
int maxVal[3] = {2000, 1500, 500}; // int maxVal[7] = {920, 920, 640, 460, 530, 200, 150};

void setup()
{
  if (VERBOSE) {
    Serial.begin(115200);
  }

  Audio.Init();
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  //
  //  Clear LED Strip
  //
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void loop()
{
  Audio.ReadFreq(freqVal);  //  Return 7 value of 7 bands pass filiter 
                            //  Frequency(Hz):  63  160  400  1K  2.5K  6.25K  16K
                            //  freqVal[]:       0   1    2    3    4     5     6 

  int low = max((freqVal[0]+freqVal[1]-200), 0);
  int mid = max((freqVal[2]+freqVal[3]-200), 0);
  int high = max((freqVal[4]+freqVal[5]+freqVal[6]-300), 0);

  if (low > maxVal[0])
    low = maxVal[0];

  if (mid > maxVal[1])
    mid = maxVal[1];
  
  if (high > maxVal[2])
    high = maxVal[2];

  int red = map(low, 0, maxVal[0], 0, 255);
  int blue = map(mid, 0, maxVal[1], 0, 255);
  int green = map(high, 0, maxVal[2], 0, 255);

  if (VERBOSE) {
    Serial.print(low);
    Serial.print("\t");
    Serial.print(mid);
    Serial.print("\t");
    Serial.print(high);
    Serial.print("\n");
  }

  for (int i=0; i<NUM_LEDS-1; i++){
    leds[i] = leds[i+1];
  }

  leds[NUM_LEDS-1] = CRGB(red, green, blue);
  FastLED.show();

  int dynamicDelay = map(low, 0, maxVal[0], 20, 1);
  delay(dynamicDelay);
}
