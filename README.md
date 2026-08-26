It's a simple code to check the controller connection with ESP32 and print inputs in Serial Monitor. I'm using Bluepad32 library by Ricardio Quesada for the connection purpose. The library allows connection coverage for pretty much all controlllers (I'm testing using an EvoFox S one and a Cosmic Byte controller). 
For connecting, first we gotta configure the board-
1. Download the Bluepad32 library by author Ricardio Quesada and install into the IDE.
2. From boards, select esp32_bluepad32 and select ESP32 Dev Module from it. This allows the board connection with the controller.
3. Compile and Upload the code.

Hardware segment-
1. Check your controller is not connected to any other devices, best is to turn off bluetooth on all other devices that connect to it.
2. Put your controller in Xbox mode (Bluetooth mode), usually by pressing A + Home button in EvoFox S. The light will start blinking and then come to a solid when it's connected.
3. Check the Serial Monitor for the inputs of the controller.

It's just a hobby project so I'm not tryna overcomplicate.