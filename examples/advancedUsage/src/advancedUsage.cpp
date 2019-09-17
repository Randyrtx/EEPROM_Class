/**
 * @file advancedUsage.cpp
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief 
 * @version 1.0.1
 * @date 2019-09-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Particle.h>

/**
 * @details
 * 
 * This example expands on the basic usage example to include a user-define data object
 * to hold user credentials
 * 
 * Includes the UserSettings object from the basic example for the internal clock,
 * Wifi Antenna, and Wifi Hostname settings.
 * 
 * Adds a custom data object using the EEPROM_Class Base Class for user credentials.
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
#include "EEPROM_Class.h"
#include "UserSettingsClass.h" 


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

//! @brief User's Default user name
#define USER_NAME "NewUserName"
//! @brief User's Default password
#define USER_PASSWORD "NewPassword"

// Define the EEPROM addresses for the data object images.
// We will have two independant objects in EEPROM, each separately maintained.

//! Relative address of the mySettings object in EEPROM
#define mySettingsAddress 0

//! Relative address of the myCredentials object in EEPROM
#define myCredentialsAddress mySettingsAddress + sizeof(mySettings) + sizeof(uint16_t) // Make sure to account for the location and size of the previous object(s) and checksum


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

    // Create the data objects
	// Place here if only needed in setup.
	// Place globally if settings will be changed elsewhere in the application.

	// User Settings Class Object
	UserSettingsClass mySettings;

    // Create the object to hold user credentials
    struct UserCredentials
    {
        char userName[16];
        char password[16];
    };

    // Define the RAM copy
    UserCredentials myCredentials = {"", ""};

    // Create an EEPROM class instance to hold the object
    EEPROM_Class<UserCredentials> myEEPROM;

    // Set following if() statement to true to invalidate objects to simulate first run
    if(false)
    {
        EEPROM.write(mySettingsAddress, 0);
        EEPROM.write(myCredentialsAddress, 0);
    }
    
	Serial.println("\n***** Retrieving current contents. If it fails, reload defaults.\n");

	// Load Data from EEPROM (This will fail on first run so defaults will be loaded into EEPROM)
	if (!mySettings.begin(mySettingsAddress))
	{
		Serial.println("\n !!!!! mySettings Data Corrupted, reset to defaults.\n");
	}
    else
    {
        Serial.println("\n***** mySettings data retrieved successfully.");
    }

	// display the settings retrieved
	Serial.println();
	mySettings.logUserData();

	Serial.println("***** Hit any key to continue ***** \n");
	// wait for user to hit a key
	while (!Serial.available())
		;
	Serial.read();

    Serial.println("***** Now Initializing and fetching the credentials object...\n");

    // Now Initialize and fetch the credentials object
    
    // We have to do a bit more of the work to fix it if it's not valid
    // Serial.println("myEEPROM.begin(myCredentialsAddress, myCredentials)");
    if (!myEEPROM.begin(myCredentialsAddress, myCredentials))
    {
        // Save some default data to load
		Serial.println("\n!!!!! EEPROM Data Corrupted, resetting Credentials to defaults.\n");
        strcpy(myCredentials.userName, USER_NAME);
        strcpy(myCredentials.password, USER_PASSWORD);
        myEEPROM.writeObject(myCredentials);
        Serial.printlnf("\n***** Loaded EEPROM with User name: %s - Password %s", myCredentials.userName, myCredentials.password);
    }
    else
    {
        Serial.printlnf("\n***** Success! User name: %s - Password %s\n", myCredentials.userName, myCredentials.password);

    }
	Serial.println("***** Hit any key to continue ***** \n");
	// wait for user to hit a key
	while (!Serial.available())
		;
	Serial.read();
	

    // Now change something and save it
    Serial.println("***** Loading EEPROM with new data...\n");
    strcpy(myCredentials.userName, "abcde");
    strcpy(myCredentials.password, "12345");
    myEEPROM.writeObject(myCredentials);
    Serial.printlnf("\n***** Loaded EEPROM with User name: %s - Password: %s\n", myCredentials.userName, myCredentials.password);


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
		Serial.printlnf("Time Zone: %d", mySettings.getTimeZone());
        Serial.printlnf("DST Offset %0.2f", mySettings.getDstOffset());
        Serial.printlnf("DST Enabled: %s", (mySettings.isDSTEnabled()) ? "Yes" : "No");
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
        Serial.println();
		WiFi.selectAntenna(mySettings.getAntennaType());
	}

    Serial.printlnf("\n***** User name: %s", myCredentials.userName);
    Serial.printlnf("***** Password %s\n", myCredentials.password);

    Serial.println("***** Cycle power when done to verify that the objects remain valid.");

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