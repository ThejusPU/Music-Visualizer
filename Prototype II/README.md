## Prototype II (2020)

I intended to use a bluetooth chip to connect the music to multiple devices (speaker and LED device), but Apple has limited the scope of products that can be connected at the same time to an Apple device, to only Apple devices. Devices with either the H1 or W1 chip. So I acquired a microphone, because progress must continue.

For this prototype I wanted to implement the ArduinoFHT library I had found previously. Fast Hartley Transformation (FHT) is a method to implement Discrete Hartley Transoformation quickly, so that it may be run on low processing speed computer systems, like arduinos.

To better create the visualizer and analyze the data I have decided to learn Processing which can read and analyze the data through my serial ports and create a visualization for how the LED lights are going to behave. Processing has been very useful for prototyping different methods of music visualization.

This prototype never got physically manifested, because the managing the LED lights as well as running ArduinoFHT on a singular arduino would slow down the entire system. Prototype III tackles this problem.

# Resources
theory on sound analysis & arduinoFHT:
https://blog.yavilevich.com/2016/08/arduino-sound-level-meter-and-spectrum-analyzer/

Useful thread on arduinoFHT:
https://forum.arduino.cc/index.php?topic=521619.0

arduinoFHT example:
http://wiki.openmusiclabs.com/wiki/FHTExample

FHT functions (can also be found in the README of FHT library):
http://wiki.openmusiclabs.com/wiki/FHTFunctions

# Learning Processing!
Reading from serial port:
https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

Multiple windows:
https://forum.processing.org/two/discussion/12272/multiple-windows-with-processing-3
https://gist.github.com/atduskgreg/666e46c8408e2a33b09a

Tutorials:
https://processing.org/tutorials/

# Resources for bluetooth implementation in the future when iOS 13 bluetooth pairing represents American values again
combining left & right audio properly:
https://www.instructables.com/community/Connecting-left-and-right-audio-channels-together/