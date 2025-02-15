/*
  Still kinda trying to figure out how im gonna do sound so this is very experimental
  currently the 2 methods im experimenting with are:
  
  - Adafruit Music Maker FeatherWing
    • file sizes are a none issue with the SD card being multiple gigabytes or even a terrabyte 
      (tho i dont think or trust arduino can handle one that big)
    • Asynchronous audio playback is quite possibly the biggest draw to this solution meaning file 
      management can be offloaded to a audio co-processor but it increases powerdraw and requires 
      extraspace there may not be.
    • the output of the featherWing is stereo which isnt an issue but its overkill and just adds
      cost
    • requires lots of pins for a specialized interface and may interfere with timmings for neopixels

  - onboard I2S audio with external SD card 
    • the audio files are far too big to fit on the feather so they are gonna have to go on something
      like a SD card 
    • communicating from a SD card is almost ideal except the time it would take to fetch the data
      to fill the audio buffer back up after its all been played out would really add up especially
      without a seperate core to run the button interupts and handle the accelerometer, Neopixel 
      updates, & button interrupts all aside from audio 
    • potentially lower power and far cheaper than getting an additional external controller
    
  I'm really split between these to methods, I thought about getting a smaller microcontroller like a 
  Adafruit QT Py SAMD21 with an audio add-on but decided against the added complexity. currently im gonna
  try to implement the Music Maker FeatherWing but may switch.
*/

//(Adafruit_Feather_RP2040_Prop-Maker)
#define VS1053_CS 32    // VS1053 chip select pin (output)
#define VS1053_DCS 33   // VS1053 Data/command select pin (output)
#define CARDCS 14       // Card chip select pin
#define VS1053_DREQ 15  // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(-1, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);















String findFilePath(File dir, String name, String path)// hopefully there isn't too many folders RAM might be an issue if there is
{
  if(dir.isDirectory()) { // confirm the starting file is a directory
    File entry = dir.openNextFile(); // move to the next file
    while(entry.print(String) != name) 
    {
      if(!entry) //check if theres any files left in this directory
      {
        entry = dir.openNextFile(); 
        break; // welp not in this folder 
      }
      if(entry.isDirectory()) 
      {
        String nextPath = path + entry.print(String); // get the path to the next directory and save it
        String returnPath = findFilePath(entry, name, nextPath); // check the next directory
        (SD.open(returnPath).print(String) == name) ?  entry = SD.open(returnPath) : entry = dir.openNextFile();
      }
      if(entry.print(String).compareTo(name)) // WE FOUND IT BOIS WWOOOOOO! PASS THE PATH IT BACK THE LINE
      {
        path.concat("/" + name); //complete the path
        Serial.println("\n WE FOUND IT BOIS!!! \n File path found calling back. \n");
        return path; // File path found calling back.
      } 
      else 
      {
        entry = dir.openNextFile(); 
      }
    }
    return "";
  }
}

//(Adafruit_Feather_M4_Express)
// #define VS1053_CS 6    // VS1053 chip select pin (output)
// #define VS1053_DCS 10  // VS1053 Data/command select pin (output)
// #define CARDCS 5       // Card chip select pin
// #define VS1053_DREQ 9  // VS1053 Data request, ideally an Interrupt pin


