/**
 * @file basicUsage.cpp
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2019-09-15
 * 
 * @copyright Copyright (c) 2019
 * 
 */
 
#include <Particle.h>

/**
 * @details
 * 
 * Basic Example to demonstrate use of the UserSettings Derived Class of the EEPROM_Class Library
 * 
 * This example reads the UserSettings object from EEPROM, then configures the internal clock,
 * Wifi Antenna, and Wifi Hostname from the retrieved data.
 * 
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
 * @brief Define DEBUG_RTX for more verbose log messages from the Class Functions
 * 
 */
#define DEBUG_RTX

#ifdef LOG_RTX
#ifdef DEBUG_RTX
//! @brief Log handler
SerialLogHandler logHandler(115200, LOG_LEVEL_ALL);
#else
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR);
#endif
#endif

/******************************************************************************
 * Class Instantiations
 ******************************************************************************/
#include "UserSettingsClass.h" 

/**
 * @brief Define an EEPROM address for the Settings Object.
 * 
 */
#define myAddress 0


//! @brief User's Default Time Zone
#define USER_TZ -8
//! @brief User's Default Daylight Savings Time Offset
#define USER_DSTOFFSET 1.0
//! @brief User's Default DST Enable Flag
#define USER_DSTENABLE true
//! @brief User's Default wifi Hostname
#define USER_HOSTNAME "NewHostName"
//! @brief User's Default Antenna Type
#define USER_ANTENNA_TYPE ANT_INTERNAL

/******************************************************************************
 * Hardware pin definitions
 ******************************************************************************/

/**
 * @brief Blue Led on the Photon Module
 * 
 */
#define ledMain D7

/******************************************************************************
 * Setup
 ******************************************************************************/
/**
 * @brief Setup Function
 * 
 * - Instantiate the settings data object
 * - Setup basic I/O
 * - Run basic demostration of the UserSetting Class Object
 * 
 */
void setup()
{
	// User Configuration Settings Object
	// Place this here if settings will only be used in setup.
	// Place globally if settings will be changed elsewhere in the application.

	UserSettingsClass mySettings;

	/******************************************************************************
	 * Setup basic environment for demonstration
	 ******************************************************************************/

	// Enable the onboard LED
	pinMode(ledMain, OUTPUT);

	// Setup Serial  and wait until the user acknowledges
	Serial.begin(115200);
	delay(5000);

	Serial.print("\n***** Hit any key to start *****\n\n");
	while (!Serial.available())
		;
	Serial.read();

	/******************************************************************************
	 * Start the demonstration
	 ******************************************************************************/
	Serial.print("***** Starting Usage Test\n\n");
	delay(1000);

	Serial.println("***** Retrieving current contents. If it fails, reload defaults.\n");

	// Load Data from EEPROM (This will fail on first run so defaults will be loaded into EEPROM)
	if (!mySettings.begin(myAddress))
	{
		Serial.println("!!!!! EEPROM Data Corrupted, User Settings reset to defaults.\n");
	}

	// display the settings retrieved
	Serial.println();
	mySettings.logUserData();

	Serial.println("***** Hit any key to continue ***** \n");
	// wait for user to hit a key
	while (!Serial.available())
		;
	Serial.read();
	
	// Force a "corrupt" of the data object to force a reload with defaults
	Serial.println("***** Corrupting the data block in EEPROM to force a reload...\n");
	
	/** Write to first byte after checksum, just to simulate a fault.
	 *  We normally would avoid manual access to any of the EEPROM space 
	 *  occupied by the data objects.
	 */
	EEPROM.write(myAddress + sizeof(uint16_t), 0); 
    delay(1000);
    
	if (!mySettings.verifyChecksum());
	{
		Serial.println("\n!!!!! EEPROM Data Corrupted, Resetting to defaults.\n");
		mySettings.reinitialize();
		Serial.println();
	}

	// display the settings retrieved
	mySettings.logUserData();

	Serial.println("***** Hit any key to continue *****\n ");
	// wait for user to hit a key
	while (!Serial.available())
		;
	Serial.read();
	
	// Now change settings to the defaults we defined earlier
	Serial.println("*****  Now changing to my defaults...\n");
	mySettings.setTimeZone(USER_TZ);
	mySettings.setDSTEnabled(USER_DSTOFFSET);
	mySettings.setAntennaType(USER_ANTENNA_TYPE);
	Serial.println();
	mySettings.logUserData(); 

	Serial.println("***** Hit any key to continue ***** \n");
	// wait for user to hit a key
	while (!Serial.available())
		;
	Serial.read();

	/******************************************************************************
	 * Now we can use the retrieved settings to configure the application
	 ******************************************************************************/
	if (true) // Set to true to run the following section
	{
		Serial.println("***** Configuring application user settings ***** \n");
		// This will set the internal clock settings according to the saved user data
		Serial.println("Settings for the clock...");
		Time.zone(mySettings.getTimeZone());		  // Set time zone
		Time.setDSTOffset(mySettings.getDstOffset()); // Set DST offset
		if (mySettings.isDSTEnabled())
		{
			Time.beginDST(); // Set Daylight Savings Time flag
		}

		// This sets the Wifi Hostname
		WiFi.setHostname(mySettings.getHostName());
		Serial.printlnf("Wifi Hostname: %s", mySettings.getHostName());

		// This sets the antenna type
		switch (mySettings.getAntennaType())
		{
		case ANT_INTERNAL:
			Serial.println("Setting Antenna Type to Internal");
			break;
		case ANT_EXTERNAL:
			Serial.println("Setting Antenna Type to External");
			break;
		case ANT_AUTO:
			Serial.println("Setting Antenna Type to Auto");
			break;
		default:
			break;
		}
		WiFi.selectAntenna(mySettings.getAntennaType());
	}

	/******************************************************************************
	 * End of demonstration
	 ******************************************************************************/
	Serial.println("\n***** Test Halted ***** \n");
    Serial.println("Since we're done with the settings, we can dispose of the object until needed again.\n");
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