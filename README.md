# Making a prop Lightsaber from scratch with a custom chassis
- [Making a prop Lightsaber from scratch with a custom chassis](#making-a-prop-lightsaber-from-scratch-with-a-custom-chassis)
- [Libraries \& variables](#libraries--variables)
- [Pin diagram](#pin-diagram)
- [Setup](#setup)
  - [Status LED](#status-led)
  - [Accerometer](#accerometer)
  - [File System Check and Index](#file-system-check-and-index)
    - [Check Root for persistance](#check-root-for-persistance)
    - [Load Last State](#load-last-state)
- [Start the main loop](#start-the-main-loop)
  - [Button Behavior](#button-behavior)
  - [General Button Behaviors](#general-button-behaviors)
  - [Sensor Behaviors](#sensor-behaviors)
- [Possible add on features](#possible-add-on-features)




# Libraries & variables
FASTLED for neopixel control, the Adafruit_VS1053 and SD libraries for audio and file control
then Adafruit LSM6DS, Adafruit BusIO, Adafruit Unified Sensor for the accerometer/gyroscope
# Pin diagram

# Setup
## Status LED
initialize the neopixel on Feather M4 Express and use it as a status light. please also update the main button color based on the current status

Red being halted or any other of the following: 
   • SD card missing audio files or folder
   • missing SD card or feather

Yellow being start up successful but there is an error with any of the following:
   • Serial.begin(); failed
   • battery voltage low 
   • no blade detected 'on Blade detection pin'
   • SD card Checksum doesn't match or INDEX.txt is missing (after new index is made change to green if there are no issues)

Green being Start up succsesful and no issues 

## Accerometer 
check for the Adafruit LSM6DSOX 6 DoF Accelerometer and Gyroscope on the I²C interface print the initial position of the gyroscope

## File System Check and Index
### Check Root for persistance
check the presence of the Musicmakerfeather and its SD card and index.
        check index of the root folder and calculate a checksum and check if its the same as found in a file called INDEX.txt in the root directory.
   • if the checksum is different store the new index and checksum
   • if there is no prexisting checksum or INDEX.txt then store the new one, if it doesnt match replace it. 
   • print the status of the checksum and also print the index of the root folder.

### Load Last State
check for a file called lastState.csv where the first cell is the animation mode then and the rest are the color values of each pixel on the blade when it was last turned off. 
   • then if it exists load it into the FASTLED arraylist for the neopixel blade and print it to the serial console. 
   • 
        
Check for a directories that start with a number followed by a dash
   • run the Index chesksum check and lastState check proccess completed on the root directory
   • check if there are .mp3 files that match the master name list

Enable interupts on the main button and the 3 secondary buttons and write a interupt serice routine that updates a global constant for each button (main button reffered as M1 and secondary buttons are S1, S2, S3)

On a succsessful setup flash the Status LED and Main button Green twice then turn off the status light on the Feather M4

# Start the main loop
## Button Behavior
  1. if both (S1 && S3) are held for about a second enter __animation selection mode__
        1. once in *animation mode*, S1 and S2 with increment through a series of animations
        2. pressing M1 should use the current mode and Show it on the blade
   • flicker (modifier? | blade effect)
   • stutter (modifier? | pixel effect)
   • solid rainbow (contantly increase hue value | blade effect)
   • rainbow cascade () 
   • no animation

  2. if S1 is held for a second go into __color select mode__
        1. once in *color mode*, S1 and S3 with rotate through the **Hue** of the HSV color space
        2. pressing M1 should use the color currently displayed on M1 

  3. if S2 is held for a second go into __Saturation select mode__
        1. once in *Saturation mode*, S1 and S3 with increase/decrease through the **saturation** of the HSV color space
        2. pressing M1 should use the saturation value currently displayed on M1

  4. if S3 is held for a second go into __Vibrance select mode__
        1. once in *color mode*, S1 and S3 with increase/decrease through the **vibrance** of the HSV color space
        2. pressing M1 should use the vibrance value currently displayed on M1

## General Button Behaviors 
  if the blade is currently lit and M1 is pressed (not held) what ever status selected on M1 will smoothly morph transition from start value to end value in a finite and defined number of frames (undecided on the speed function as linear or not)

  if help it will wipe from the bottom to the tip of the blade


## Sensor Behaviors
  if the magnitude of ∆φ and ∆θ are above a threshold a 'swing' sound effect will be played 

  if the acceleration on the gyroscope is below a negitive threshold it will play a 'clash' sound effect and flash the blades brightness

  if no blade is detected it will still do the effects on the connector thats in the emitter
# Possible add on features
brightness control
volume control
low battery warning (visual or audible)
sound 'font' Switching

