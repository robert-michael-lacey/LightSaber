#include <Adafruit_LIS3DH.h>
#include <FastLED.h>

#define EXTERNAL_NEOPIXELS 21
#define NUM_LEDS 128
CRGBArray<NUM_LEDS> leds;
int x = 0;  // Continuity Constant for Animation frames

// Adafruit_VS1053_FilePlayer musicPlayer =
//   Adafruit_VS1053_FilePlayer(-1, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);


#include "bladeAnimations.h"
#include "buttonStuff.h"
#include "AccelAudio.h"




void setup() {
  delay(200);
  Serial.begin(115200);

  FastLED.addLeds<WS2812, EXTERNAL_NEOPIXELS, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);

  

  pinMode(PIN_EXTERNAL_POWER, OUTPUT);
  digitalWrite(PIN_EXTERNAL_POWER, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
}



// audio runs on core 2!

#include <I2S.h>

// #include "boot.h"
// #include "hithere.h"

struct {
  const uint8_t *data;
  uint32_t       len;
  uint32_t       rate;
} sound[] = {
  hithereAudioData, sizeof(hithereAudioData), hithereSampleRate,
  bootAudioData   , sizeof(bootAudioData)   , bootSampleRate,
};
#define N_SOUNDS (sizeof(sound) / sizeof(sound[0]))

I2S i2s(OUTPUT);

uint8_t sndIdx = 0;


void setup1(void) {
    if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start LIS3DH");
    while (1) yield();
  }

  lis.setRange(LIS3DH_RANGE_2_G);

  if (!musicPlayer.begin()) {  // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    while (1)
      ;
  }
  musicPlayer.sineTest(0x44, 250);  // Make a tone to indicate VS1053 is working
  delay(125);
  musicPlayer.sineTest(0x44, 125);
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (!SD.begin(CARDCS)) {
      delay(2500);  // don't do anything more
      pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    }
  }
  Serial.println("SD OK!");

  // list files
  printDirectory(SD.open("/"), 0);
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10, 10);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void loop1() {
  
}
