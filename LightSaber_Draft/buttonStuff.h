#define REDPIN        29
#define GREENPIN      24
#define BLUEPIN       25
#define BUTTONPIN     19 // or EXTERNAL_BUTTON

#define TOPBUTTON     13
#define MIDDLEBUTTON  12
#define BOTTOMBUTTON  11


void showAnalogRGB(const CRGB& rgb) {
  analogWrite(REDPIN, rgb.r);
  analogWrite(GREENPIN, rgb.g);
  analogWrite(BLUEPIN, rgb.b);
  // Serial.println("Button Method Called");
}

//pull button updates
