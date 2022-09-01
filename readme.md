## Inspiration
This project is inspired by the Simonduino project in Andre LaMothe's course on Udemy (Crash Course Electronics and PCB Design). As with other projects in the course, I've decided to turn up the volume to the proverbial "11". I call it 
Yaduino - Yet Another Arduino (the missing r is deliberate!) :)

![Top PCB image](https://github.com/hishamk/yaduino/blob/main/top.png?raw=true)

 
### Design Goals
My aim with this project was to have fun, learn and push myself to figure out how to implement my design goals for the 8-bit ATmega 328 microcontroller.

   

 1. Should function as a regular Arduino (complete with ICSP for burning
    the bootloader)

 2. Should use as many SMD parts as possible from JLC's parts library so
    that I can utilize their assembly service to some extent
    
 3.  Should include easily accessible headers for pluggin in either an
    I2C VFD/OLED display or a serial 1602A display
    
 4. Should work with a LiPo as well as USB power and have a proper
    battery management circuit, boost circuit and load sharing circuit
    
 5. Should have a provision for plugging in an ESP-8266 ESP-01 module to add WiFi
    access
    
 6. Should have at least one optional IC, such as a demux, with some
    blinkenlights for good measure. This may be disabled using a solder
    jumper.
    
  7.  Should have some input facility, such as a rotary encoder/button.
    
  8.  Should look good without a case as well as with a case.
    
   9. Silkscreen should be informative and cool, because why not.

  
### Fixed Issues 

This board has been fully built, tested and debugged. All mistakes listed below have been fixed as of this version (but not built, as the bodge wire version with the fixes works fine).

  

The issues were:

 1. ESP stability. ESP-8266 ESP-01 requires more than 200mA at startup. This needs clean power. Fixed with 100uF bypass.

  2. Board was receiving serial data (RX) but was unable to transmit (TX). Culprit was too small resistor value on TX LED. It was 1K. Changed to 3K3 and that fixed it. It seems the LED was drawing too much current away from the actual 
TX signal distined to teh CH340C USB-UART IC.

3. Forgot to connect supply on the charge management IC! Bodge wire to the rescue!

4. Messed up volume POT connections. Had full resistance of POT (10K or 20K depending on POT used), instead of actual setting. Fixed with bodge wire and POT pin gymnastics and PCB trace cut.

 5. 1602A lines-to-pins silkscreen on bottom side was incorrect in a couple of places.
 
### Assembly

I ordered the PCBs from JLC and utilized their assembly service. The main components that I had to solder myself where: the ATmega328P-AU (drag solder and some solder wick does the trick), headers, POTS and the various bodge/fixes.


### Programming
  
So once the board is full assembled, you'll want to burn the Arduino bootloader onto it. Assuming you have a factory-fresh ATmega 328 with no set fuses, this process should not pose any issues.

*Tip*
You can use [AVRFuses](https://vonnieda.org/software/avrfuses) to check the fuse settings on the ATmega328 using a USBasp connected to the ICSP header.

  
To burn the bootloader and start uploading Arduino sketches:

1. Connect a USBasp to the ICSP header.

2. Install the [MiniCore](https://github.com/MCUdude/MiniCore) package

3. In the Arduino IDE, select MiniCore | Atmega328 for the Board, then select External 16MHz for clock, 2.7v for BOD, EEPROM retained for EEPROM, LTO Disabed for LTO Compiler, 328P/328PA for the Variant, YES (UART10) for the Bootloader

4. Set the programmer to "USBasp (MiniCore)"

5. Select Burn Bootloader

6. Once the bootloader is burned successfully, select AVRISP mkII (MiniCore) for the programmer.

7. Your board is now ready to run Arduino code uploaded via the micro USB connection

### ESP-8266 / WiFi

 To use an ESP-8266 ESP-01 with the Yaduino, you'll need to use the [WiFIESPAT](https://github.com/jandrassy/WiFiEspAT) library.

I opted to use [v2.2.0.0](https://github.com/jandrassy/UnoWiFiDevEdSerial1/wiki/files/ESP8266-1MB-tx1rx3-AT_V2.2.zip) of the firmware. You may need to update the firmware using a basic circuit and a USB-UART board (Google it, it's out there). Make sure you have a 100uF bypass cap if you're facing temperamental issues with either 
updating the firmware or testing it. A lot of these schematics on the net do not have this cap specified.

Once your firmware is updated, you'll have to change the baud rate to 9600. You may do this manually using AT commands after updating the fimrware, or you could:

1. Plug the ESP-8266 into the Yaduino

2. Load the ChangeATBaudRate sketch in the WiFiESPAT library's examples folder

3. Change the Software Serial ports to 11 and 10 instead of 6 and 7.

4. Upload sketch.

5. Your ESP-8266 should now work at 9600.

The reason we use Software Serial for the ESP-8266 connection is due to the fact that the hardware serial pins are also used for the 1602A display. Yes, I know, if you use an I2C display this wouldn't be an issue - but I wanted to 
design it with this constraint to see if I can get everything working fine.

### Example Code

 The example code does a few things:

1. Displays text on 1602A

2. Connects to WiFi (make sure you run the WiFiESPAT examples folder sketch "SetUp PersistentWiFiConnection" to set your WiFi details first.)

3. Displays IP address and some other fidgety things on clicking/rotating.

4. Runs a telnet server. Connect to port 2323 from another machine and type in a message. It'll get displayed on the Yaduino!

### Enclosures

I've designed and printed a few [enclosures](https://a360.co/3RtZ853) using Fusion 360 and printed them on a Prusa Mini+ using PLA filaments.

