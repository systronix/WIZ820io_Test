# WIZ820io Teensy3 Ethernet Test
These all use Teensy 3.X as the controller.

Hardware used
 - Teensy 3.X [Teensy 3.2](https://www.pjrc.com/store/teensy32.html)
 - PRJC [WIZ820io & Micro SD Card Adaptor](https://www.pjrc.com/store/wiz820_sd_adaptor.html) V1 and now V2
 - WIZnet [WIZ820io](http://www.wiznet.co.kr/product-item/wiz820io/) 
 although the WIZ550io is what we are moving to since it is "better"
 - solderless breadboard (for optional sensors) such as this [Adafruit half-size](https://www.adafruit.com/products/64)
 - USB power supply to run the combination when not plugged into a PC
 - wire jumpers for connecting optional sensors and other devices, like [these](https://www.adafruit.com/products/1954)

## DhcpStressTest
 - Shows what we had to do to get DHCP to work reliably. 
 - This program includes several recovery techniques designed to work around the known bugs in the WIZ820io/W5200 chip.
 - DHCP lease time (on my router) is set to 120 seconds, the minimum allowed, and it appears to allow renewal at 1/2 that, or every 60 seconds. 
 - This enables 24 * 60, or 1440 DHCP renewals in 24 hrs vs a more typical one or two. This compresses one or two years of DHCP renewal into 24 hours.
 - Some issues are Arduino library issues I believe, but more on that later.
 - Totalphase Beagle SPI sniffer captures showing the effect of the ARP bug which affects DHCP. Not included in repo since they are too large (over 300MB Zipped). Ask if you'd like them.

## ILI9341_WIZ820_Test
 - use of Ethernet and the ILI9341 touchscreen at same time
 - moves the ILI9341 to alternate SPI pins

## EthernetKeypadLCD
 - the Arduino Ethernet Shield example adapted to the Teensy and WIZ820io hardware

## NtpTest
 - just as it sounds, to use Network Time Protocol, I have not done a lot with this yet

## DhcpAddressPrinter
 -- Example from Arduino, adapted to the Teensy and WIZ820io hardware