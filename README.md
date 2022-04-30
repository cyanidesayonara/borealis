Borealis is a rehash of a project I made some years back, in the spring of 2016.

It's a fork of the Aurora project (https://github.com/pixelmatix/aurora) by
Pixelmatix, a dynamic LED display visualizer.

More specifically, it's a fork of the aurora64 branch, which adapts the software
to a display four times the size of the original 32x32 pixel display.

The LED display is built from two pieces of HUB75 64x32 Pixel LED panel screwed
onto a custom aluminum frame, together forming a single 64x64 pixel display. The
display is driven by a Teensy 3.2
microcontroller (https://www.pjrc.com/store/teensy32.html). The software is
developed and uploaded onto the Teensy using the Arduino IDE and the teensyduino
plugin.

Borealis in action  

![Borealis in action 1](demo1.gif)

![Borealis in action 2](demo2.gif)

![Borealis in action 3](demo3.gif)

Fun fact: I built this thing before I actually knew how to code at all. Like I
had basically never even seen code before. So all I really did was change some
values and comment out some unused features in order to save memory (using a
64x64 display instead of the original 32x32 means memory usage is also higher,
dangerously close to the Teensy's RAM limit of 64K). This is an attempt to
reapply my new-found coding skills on this project and to clean out and improve
the code I once left behind. What makes this interesting is that although I've
learned other languages since, I still don't know any C++ beyond the bare
basics :).

# Start developing

* Install Arduino IDE 1.6.5 (important: newer versions won't work)  
  https://www.arduino.cc/en/main/OldSoftwareReleases
* Install teensyduino 1.27 (run as administrator)  
  https://www.pjrc.com/teensy/td_download.html  
  Choose the follwing libraries to install: 
  * DS1307RTC
  * FastLED
  * Time
* Plug in your Teensy and Start Arduino IDE
* Set Teensy as target board  
  Tools -> Board -> Teensy 3.2 / 3.1
* Set target USB port  
  Tools -> Port
* Download aJson & SmartMatrix3  
  https://github.com/interactive-matter/aJson/tree/ae3d01b3115f7c1379f8f33e407aa93e3812a7f6  
  https://github.com/pixelmatix/SmartMatrix3  
  Go to Code -> Download Zip
* Unzip aJson & SmartMatrix3 into your /Arduino/libraries folder
* Open Borealis.ino in Arduino IDE and upload it to your Teensy  
  Sketch -> Upload
* Use Serial monitor for logging and debugging  
  Tools -> Serial Monitor

# Further reading

https://learn.adafruit.com/smartmatrix-remote-controlled-led-art-display/aurora-arduino-sketch?view=all
