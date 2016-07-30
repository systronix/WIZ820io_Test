# WIZ820io Teensy3 Ethernet Test
These all use Teensy 3.X as the controller.

Hardware used
 - Teensy 3.X [Teensy 3.2](https://www.pjrc.com/store/teensy32.html)
 - PRJC [WIZ820io & Micro SD Card Adaptor](https://www.pjrc.com/store/wiz820_sd_adaptor.html)
 - WIZnet [WIZ820io](http://www.wiznet.co.kr/product-item/wiz820io/) 
 although the WIZ550io is what we are moving to since it is "better"

## DhcpStressTest
 - Shows what we had to do to get DHCP to work reliably. 
 - DHCP lease time (on my router) is set to 120 seconds, the minimum allowed by the Asus RT56NU router, and it appears to allow renewal at 1/2 that, or every 60 seconds. 
 - This enables 24 * 60, or 1440 DHCP renewals in 24 hrs vs a more typical one or two. This compresses one or two years of DHCP renewal into 24 hours.
 - Some issues are Arduino library issues I believe, but more on that later.
 - Totalphase Beagle SPI sniffer captures showing the effect of the ARP bug which affects DHCP 
## ILI9341_WIZ820_Test
 - use of Ethernet and the ILI9341 touchscreen at same time
 - moves the ILI9341 to alternate SPI pins
## EthernetKeypadLCD
 - the Arduino Ethernet Shield example adapted to the Teensy and WIZ820io hardware
## NtpTest
 - just as it sounds, to use Network Time Protocol, I have not done a lot with this yet
## DhcpAddressPrinter
 -- Example from Arduino, adapted to the Teensy and WIZ820io hardware