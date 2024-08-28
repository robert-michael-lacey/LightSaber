



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