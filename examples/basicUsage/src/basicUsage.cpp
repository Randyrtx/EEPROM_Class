/**
 * @file basicUsage.cpp
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2019-09-14
 * 
 * @copyright Copyright (c) 2019
 * 
 * This example uses the EEPROM_Class Base Class only to demonstrate saving of basic
 * data to EEPROM.
 */

#include <Particle.h>

/**
 * Synopsis:
 */

/******************************************************************************
 * Serial log handler
 ******************************************************************************/

/**
 * @brief Define LOG_RTX to enable error logging to terminal
 * 
 */
#define LOG_RTX

/**
 * @brief Define DEBUG_RTX for more verbose messages from the Class Functions
 * 
 */
#define DEBUG_RTX

#ifdef LOG_RTX
#ifdef DEBUG_RTX
SerialLogHandler logHandler(115200, LOG_LEVEL_ALL);
#else
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR);
#endif
#endif

/******************************************************************************
 * Class Instatiations
 ******************************************************************************/

#include "EEPROM_Class.h"


/******************************************************************************
 * Hardware pin definitions
 ******************************************************************************/

/**
 * @brief Blue Led on the Photon Module
 * 
 */
#define ledMain D7

/**
 * @brief Create a simple object to hold user settings
 * 
 */
struct SettingsObject
{
    char userName[16];
    char password[16];
	char hostName[32];
};

SettingsObject mySettings = {"", "", ""};

#define myAddress 0

/**
 * @brief Create an EEPROM instance to hold the object
 * 
 */
// EEPROM_Class<SettingsObject> myEEPROM(myAddress, mySettings);
EEPROM_Class<SettingsObject> myEEPROM;

/******************************************************************************
 * Setup
 ******************************************************************************/
/**
 * @brief Setup Function
 * 
 */
void setup()
{
	/******************************************************************************
	 * Setup basic environment for demonstration
	 ******************************************************************************/

	// Enable the onboard LED
	pinMode(ledMain, OUTPUT);

	// Setup Serial  and wait until the user acknowledges
	Serial.begin(115200);
	delay(5000);

	Serial.println("***** Hit any key to start ***** ");
	while (!Serial.available())
		;
	Serial.read();

	/******************************************************************************
	 * Start the demonstration
	 ******************************************************************************/
	Serial.println("***** Starting Usage Test");
	delay(1000);

	// check the EEPROM 

	// Initialize the data object
	myEEPROM.begin(0, mySettings);

	// Fetch the EEPROM image (will fail on first run)
	Serial.println("***** Attempting load of object from EEPROM...");
	if (myEEPROM.readObject(mySettings))
	{
		Serial.printlnf("***** Success! User name: %s - Password %s - Hostname %s", mySettings.userName, mySettings.password, mySettings.hostName);

		// Now change something and save it
		Serial.println("***** Loading EEPROM with new data...");
		strcpy(mySettings.userName, "NewName");
		strcpy(mySettings.password, "NewPassword");
		strcpy(mySettings.hostName, "NewHostName");
		myEEPROM.writeObject(mySettings);
		Serial.printlnf("***** Loaded EEPROM with User name: %s - Password: %s - Hostname: %s", mySettings.userName, mySettings.password, mySettings.hostName);
		Serial.println("***** Cycle power now to verify that it got saved.");

	}
	else
	{
		// Save some data to load
		Serial.println("***** Loading EEPROM with some data...");
		strcpy(mySettings.userName, "abcde");
		strcpy(mySettings.password, "12345");
		strcpy(mySettings.hostName, "SomeHostName");
		myEEPROM.writeObject(mySettings);
		Serial.printlnf("***** Loaded EEPROM with User name: %s - Password %s - Hostname %s", mySettings.userName, mySettings.password, mySettings.hostName);
		Serial.println("***** Cycle power now to verify that it got saved.");
	}
	

	/******************************************************************************
	 * End of demonstration
	 ******************************************************************************/
	delay(1000);
	Serial.println("***** Test Halted ***** ");
}

/******************************************************************************
 * loop
 ******************************************************************************/
/**
 * @brief Main Loop
 * 
 */
void loop()
{
	// I'm just gonna blink 'cause I have nothing left to do...
	digitalWrite(ledMain, HIGH);
	delay(200);
	digitalWrite(ledMain, LOW);
	delay(800);
}