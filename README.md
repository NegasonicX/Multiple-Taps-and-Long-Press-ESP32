# Multiple Taps and Long Press on ESP32
A simple code to simulate Push Button to perform Single/Double/Triple Tap as well as Short/Long Press!

 ## GPIO Function:
 * GPIO34   : input ( externally pulled up )
 
 ## Connection:
 * Connect GPIO34 with Tactile Push Button and ground
 
 ## Understanding the Flow:
 * Taps have a time frame of 1 second
 * Short Press has a time frame of 1 - 3 seconds
 * Long Press has a time frame > 3 seconds
