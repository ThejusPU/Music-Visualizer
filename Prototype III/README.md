### Prototype III

Using the FastLED library as well as the FHT library on the same arduino was an issue. A lot of fine hardware manipulation was happening in the software in order to use the FHT library, such as turning off the timer and manipulating the adc, which seemed to have an effect on the FastLED library running. This meant that I needed an arduino for running the Fast Hartley Transformation(FHT) and another arduino for controlling the light strip.

Doing some research I came across the I2C bus. This is a circuit for connecting multiple devices to a master device, such that the master may call on information from any device through that devices' given address. For this problem, the LED control arduino was the master, and the FHT arduino was the slave device. This solved the problems I had, and some of the music visualization schemes can be seen under the folder [MASTER_LIGHTING](https://github.com/ThejusPU/Music-Visualizer/tree/master/Prototype%20III/MASTER_LIGHTING).

**POST NOTES**
7/3/2022
I did this project before learning about parallel computing, so I'm a little concerned that the I2C bus may violate some parallel computing rules.