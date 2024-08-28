// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//






#include <Adafruit_NeoPixel.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

//Feather Player
#define VS1053_CS 6    // VS1053 chip select pin (output)
#define VS1053_DCS 10  // VS1053 Data/command select pin (output)
#define CARDCS 5       // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ 9  // VS1053 Data request, ideally an Interrupt pin

// Button Default States
bool mainButton = ;
#define MAINBUTTONPIN 2
bool auxCenterB = ;
#define AUXCENTERBUTTONPIN
bool auxTopB = ;
#define AUXTOPBUTTONPIN
bool auxBottomB = ;
#define AUXBOTTOMBUTTONPIN

//Communication Pins
#define BLADEDATAPIN
int24_t currentColor = 0x000000;
Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(-1, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

Adafruit_NeoPixel strip(60, PIN_EXTERNAL_NEOPIXELS, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixel(1, 8, NEO_GRB + NEO_KHZ800);


// Accelerometer Declaration
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
int x = 0;
int y = 0;
int z = 0;
// Animation stuff
int frameNumber = 0;

void setup() {
  // core1 setup
  Serial.begin(115200);
  
  // VS1053 SD MP3 player
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
  // Play a file in the background, REQUIRES interrupts!
  Serial.println("Playing: ");
  Serial.print(songNames[3]);
  Serial.println();
  musicPlayer.startPlayingFile("/DEEPRO~1//FOLLOW~1.MP3"); 
  
  // Accelerometer LIS3DH
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start LIS3DH");
    while (1) yield();
  }
  lis.setRange(LIS3DH_RANGE_2_G);
  


  // External power pin setup 
  pinMode(PIN_EXTERNAL_POWER, OUTPUT);
  digitalWrite(PIN_EXTERNAL_POWER, HIGH);

  // Blade lib Setup
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  // Main Button pin Setup
  pinMode(PIN_EXTERNAL_BUTTON, INPUT_PULLUP);

}

void loop() {

  delay(10);
  frameNumber++;
  for(int32_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + frameNumber) & 255));
  }
  strip.show();
  // Print X Y & Z accelerometer data
  if (x % 10 == 0) {
    // every 100ms
    sensors_event_t event;
    lis.getEvent(&event);
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("Accel X: "); Serial.print(event.acceleration.x);
    Serial.print(" \tY: "); Serial.print(event.acceleration.y);
    Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
    Serial.println(" m/s^2 ");
    Serial.println(x);
  }  
  // external button press disable external power
  if (! digitalRead(PIN_EXTERNAL_BUTTON)) {
    Serial.println("External button pressed");
    digitalWrite(PIN_EXTERNAL_POWER, LOW);
  }
  else {
    digitalWrite(PIN_EXTERNAL_POWER, HIGH);
  }

  if (x < 128) {
    // forward
    servo_0.writeMicroseconds(map(x, 0, 127, 1000, 2000));
  } else {  
    // and back
    servo_0.writeMicroseconds(map(x-128, 0, 127, 2000, 1000));
  }
  return;
  
}

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

// audio runs on core 2!

#include <I2S.h>

#include "boot.h"
#include "hithere.h"

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
  i2s.setBCLK(PIN_I2S_BIT_CLOCK);
  i2s.setDATA(PIN_I2S_DATA);
  i2s.setBitsPerSample(16);
}

void loop1() {
  Serial.printf("Core #2 Playing audio clip #%d\n", sndIdx);
  play_i2s(sound[sndIdx].data, sound[sndIdx].len, sound[sndIdx].rate);
  delay(5000);
  if(++sndIdx >= N_SOUNDS) sndIdx = 0;
}

void play_i2s(const uint8_t *data, uint32_t len, uint32_t rate) {

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(rate)) {
    Serial.println("Failed to initialize I2S!");
    delay(500);
    i2s.end();
    return;
  }
  
  for(uint32_t i=0; i<len; i++) {
    uint16_t sample = (uint16_t)data[i] * 32;
    // write the same sample twice, once for left and once for the right channel
    i2s.write(sample);
    i2s.write(sample);
  }
  i2s.end();
}
