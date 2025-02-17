// Case check
bool solidColorCheck(CRGB *blade, int items) {
  for (int i = 0; i < items; i++) {
    if (blade[0] != blade[i]) return false;
  }
  return true;
}


// ##### transitional animations #####

// change the colors of each pixel individually castcading from base to tip
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(wait);
  }
}


// change the color of the whole blade at once slowly blending from the start color to end color
void colorMorphStepSize(CRGB start, CRGB end, int frame) {
  // there are 2 ways to do this blending the individual red, green, and blue values or converting to HSV and change the Hue
  //check if the start and end are solid colors
  if (start == end)
    return;

  //
  CRGB stepSize = 0;
  //return stepSize<>;
}


// ##### Repeating Animations #####

// change the color of each pixel individually so the blade shows a rainbow from base to tip
void rainbowflow(int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(x + i, 255, 255);  // i being pixel offset and x being animation frame
  }
  FastLED.show();
  x++;
}


// change the color of the whole blade gradually cycling through the rainbow
void rainbow(int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(x, 255, 255);
  }
  FastLED.show();
  x++;
}
