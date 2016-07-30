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
/**
 * Modified by Bruce Boyes to stress test DHCP client with WIX820io and Teensy 3.X
 * Using the PJRC Wiznet and SD card adapter as well as custom board codenamed SALT.
 *
 * DHCP lease time is set to 120 seconds, the minimum allowed by the Asus RT56NU router,
 * and it appears to allow renewal at 1/2 that, or every 60 seconds. This enables 24 * 60,
 * or 1440 DHCP renewals in 24 hrs vs a more typical one or two. This compresses one
 * or two years of DHCP renewal into 24 hours.
 *
 * https://github.com/systronix   bboyes@systronix.com
 */

/**
 * Revisions
 *
 * 2016 Jan 15 print summary of renew and rebind fail/success every minute
 *
 */


#include <SPI.h>
#include <Ethernet.h>
#include <T3Mac.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
uint8_t ext_mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x33 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// for measuring how long a routine takes
uint32_t time_start, time_stop;

uint32_t new_millis = 0;
uint32_t old_millis = 0;	// millis saved every tiem through loop, if new > old
uint32_t old_sec_millis;	// millis saved every second tick

uint32_t new_elapsed_seconds = 0;
uint32_t total_elapsed_seconds = 0;
boolean seconds_tick = false;

uint8_t seconds_five=0;
boolean seconds_five_tick = false;
boolean minute_tick = false;

uint8_t flag=0;


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

	//  Serial1.begin(9600);  // TX1, RX1 on Teensy pins 3 and 2, not the USB serial output!

	// Wait here for up to 10 seconds to see if we will use Serial Monitor, so output is not lost
	while((!Serial) && (millis()<10000));    // wait until serial monitor is open or timeout,
	Serial.println(millis());

	Serial.println("DHCP Stress Test - 2016 Jan 15");

	//  while((!Serial1) && (millis()<10000));    // wait until serial1 is open or timeout,
	//  Serial1.print('d');
	//  Serial1.println(millis());

	//  pinMode(12, INPUT);   // help make sure MISO is input to Teensy

	delay(1000);


	Serial.println("Reading MAC from hardware...");
	read_mac();

	Serial.print("MAC: ");
	print_mac();
	Serial.println();

	uint8_t tries=1;

	  // start the Ethernet connection, try 8 times
	do
		{
		Serial.print("Try ");
		Serial.print(tries);
		Serial.print(":");
		flag = ethernet_start(mac);	// 0 if failed
		Serial.println(flag);
		}
	while ((tries++ < 5) && (flag == 0));

	if (0!=flag)
	{
		// Ethernet started OK
		Serial.print("Ethernet started after ");
	}
	else
	{
		Serial.print("Ethernet start failed even after ");
	}
	Serial.print(tries-1);	// loop already incremented it
	Serial.println(" attempts");

}	// end of setup

uint8_t renew;

uint32_t renew_success_count=0, renew_fail_count=0, rebind_success_count=0, rebind_fail_count=0;

void loop()
{

	/**
	 * Are we at a 1-second tick?
	 * This relates only to time count
	 */
	new_millis = millis();
	if (new_millis > (old_millis + 100UL))	// 100 msec or more has passed, so check for seconds tick
	{
		old_millis = new_millis;

		new_elapsed_seconds = new_millis/1000UL;
		if (new_elapsed_seconds > total_elapsed_seconds)
		{
			// seconds tick
			total_elapsed_seconds = new_elapsed_seconds;
//			Serial.print("@");
//			Serial.print(new_millis);
//			Serial.print(":");
//			Serial.print(new_millis-old_sec_millis);
//			Serial.print(F(" TES="));
//			Serial.print(total_elapsed_seconds);
//			Serial.print(" ");
			old_sec_millis = new_millis;
			seconds_tick = true;
		}
	}


	/**
	 * Do whatever needs to happen every second
	 * such as update other slower counters
	 */
	if (seconds_tick)
	{
		seconds_tick = false;	// we've used it up

		if (0 == (total_elapsed_seconds % 5))
		{
			seconds_five_tick = true;
		}

		if (0 == (total_elapsed_seconds % 60))
		{
			minute_tick = true;
		}
	}

	/**
	 * Do 5-second items
	 */
	if (seconds_five_tick)
	{
		seconds_five_tick = false;	// we've used it up
		Serial.print("@");
		Serial.print(total_elapsed_seconds);
		Serial.print(" ");

		/**
		* try to renew DHCP, maintain() returns
		* 0: nothing happened
		* 1: renew failed
		* 2: renew success
		* 3: rebind fail
		* 4: rebind success
		*
		* @see https://www.arduino.cc/en/Reference/EthernetMaintain
		*/
		renew = Ethernet.maintain();
		if (0 != renew)
		{
			Serial.print("maintain was ");
			Serial.print(renew);
			Serial.println();

			switch (renew)
			{
			case 1:		// renew failed
				Serial.print("Renew Failed=");
				Serial.print(renew_fail_count);
				break;

			case 2:		// renew success
				renew_success_count++;
				Serial.print("Renew Success=");
				Serial.print(renew_success_count);
				break;

			case 3:		// rebind failed
				Serial.print("Rebind Failed=");
				Serial.print(rebind_fail_count);
				flag = ethernet_start(mac);	// 0 if failed
				Serial.println(flag);
				break;

			case 4:		// rebind success
				Serial.print("Rebind Success=");
				Serial.print(rebind_success_count);
				break;

			default:
				Serial.print("Error! Defaulted with renew of  ");
				Serial.print(renew);
			}
			Serial.println();
		}

		// print local IP address:
		//  Serial.print("My IP address: ");
		//  Serial1.print('d'); // start of message to LCD must be 'd' char
		//  Serial1.print("IP:");
		for (byte thisByte = 0; thisByte < 4; thisByte++)
		{
			// print the value of each byte of the IP address:
			Serial.print(Ethernet.localIP()[thisByte], DEC);
			if (thisByte < 3) Serial.print(".");  // don't print trailing period
		}
		Serial.println();

	}		// end seconds five tick

	/**
	 * Do minute items
	 */
	if (minute_tick)
	{
		minute_tick = false;	// use it up
		Serial.print("Renew F/S, Bind F/S = ");
		Serial.print(renew_fail_count);
		Serial.print("/");
		Serial.print(renew_success_count);
		Serial.print(" ");
		Serial.print(rebind_fail_count);
		Serial.print("/");
		Serial.println(rebind_success_count);
	}

}	// end loop

/**
 * Try to begin ethernet with mac value, which means using DHCP.
 *
 * For non-DHCP different constructor with GW, etc is used.
 * @see https://www.arduino.cc/en/Reference/Ethernet
 * @return 0 if failed, 1 if success
 */
int8_t ethernet_start (uint8_t mac[])
{
	int8_t status = 0;	// 0 = failed
	time_start = millis();
	status = Ethernet.begin(mac);
	time_stop = millis();
	Serial.println();

	if (0==status)
	{
		Serial.print("Failed Ethernet DHCP after ");
	}
	else
	{
		Serial.print("OK Ethernet DHCP after ");
	}
	Serial.print(time_stop-time_start);
	Serial.println(" msec");
	return status;
}



