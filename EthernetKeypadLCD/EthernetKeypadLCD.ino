/*
  DHCP-based IP printer
 
 This sketch uses the DHCP extensions to the Ethernet library
 to get an IP address via DHCP and print the address obtained.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 12 April 2011
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

byte key_received[10];  // format
byte key_bytes_read = 0;     // number of serial bytes ready or read in
byte byte_incoming = 0;     // one byte read from serial input

void setup() {


/*
 * When using with PJRC wiz820 sd adapter
 * https://www.pjrc.com/store/wiz820_sd_adaptor.html [sic]
 * Some SD cards can be sensitive to SPI activity while the Ethernet library is initilized before the SD library.
 * For best compatiblity with all SD cards, these 3 lines are recommended at the beginning of setup(). 
 * Pins 4 and 10 will be reconfigured as outputs by the SD and Ethernet libraries. Making them input 
 * pullup mode before initialization guarantees neither device can respond to unintentional signals 
 * while the other is initialized.
 */
  pinMode(4, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  delay(1);  // allow time for both pins to reach 3.3V

 // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial1.begin(9600);  // TX1, RX1 on Teensy pins 3 and 2, not the USB serial output!
  Serial1.setTimeout(100L); // timeout in msec. 9600 baud = 960 chars/sec, about 1.1 msec/char
  
  // Wait here for up to 10 seconds to see if we will use Serial Monitor, so output is not lost
  while((!Serial) && (millis()<10000));    // wait until serial monitor is open or timeout,
  Serial.println(millis());



//  while((!Serial1) && (millis()<10000));    // wait until serial1 is open or timeout,
  Serial1.print('d');
  Serial1.println(millis());

//  pinMode(12, INPUT);   // help make sure MISO is input to Teensy


  }

void loop() 
{

  Serial.print("@");
  Serial.print(millis());

  // (re)start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    Serial1.print("dDHCP Failed"); // start of message to LCD must be 'd' char
    for(;;)
    ;
  }
 
  // print your local IP address:
  Serial.print(" My IP: ");
  Serial1.print('d'); // start of message to LCD must be 'd' char
  Serial1.print("IP:");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    if (thisByte < 3) Serial.print(".");  // don't print trailing period

    Serial1.print(Ethernet.localIP()[thisByte], DEC);
    if (thisByte < 3) Serial1.print('.');
  }
  Serial.println();

  Serial1.print("@");   // this is on bottom line
  Serial1.print(millis());
  Serial1.println();  // end with CR LF

  // Any keys to read?
//  key_bytes_read = Serial.readBytes(key_received, 5);
  if (0 < Serial1.available())
  {
    // some chars are ready
//    byte_incoming = Serial1.read();
    key_bytes_read = Serial1.readBytesUntil(0x0A, key_received, 5);
    Serial.print("read ");
    Serial.print(key_bytes_read);
    Serial.print(" bytes: ");
    for (int i = 0; i < key_bytes_read; i++)
    {
      Serial.write(key_received[i]);  // print the ascii char
      Serial.print("/");
      Serial.print(key_received[i], HEX);
      Serial.write(0x20); //space      
    }
    Serial.println();
  }
}



