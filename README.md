# WIZ820io Teensy3 Ethernet Test - Superceded
 - This code is pretty much superceded by the [W5500 Test repo](https://github.com/systronix/W5500_Test)
 - This code should still work but the WIZ820io is superceded by WIZ850io
 - See W5500 repo for details: new PJRC TD lib supports both 820io and 850io
 - No further work on this repo and its code is planned

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