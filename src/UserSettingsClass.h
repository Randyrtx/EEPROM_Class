/**
 * @file UserSettingsClass.h
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2019-09-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <Particle.h>
#include "EEPROM_Class.h"

//! @brief Default user name
#define DEFAULT_USER_NAME "DefaultUser"
//! @brief Default Password
#define DEFAULT_USER_PW "DefaultPW"
//! @brief Default Timezone
#define DEFAULT_USER_TZ -6
//! @brief Default Daylight Savings Time Offset
#define DEFAULT_USER_DSTOFFSET 1.0
//! @brief Default DST Enable Flag
#define DEFAULT_USER_DSTENABLE true
//! @brief Default Wifi Hostname
#define DEFAULT_USER_HOSTNAME "DefaultHostName"
//! @brief Default Antenna Type
#define DEFAULT_USER_ANTENNA ANT_INTERNAL

/**************************************************
 * @brief Data Object Structure
 * 
 * This represents the RAM working copy of the data object and the object image that is written to EEPROM
 * using the Particle EEPROM.put() function.
 * 
 */

/**
 * @brief 
 * 
 */
struct SettingsObject
{
    /** User Name Field.
     * Null-terminated, 15-character limit
     */
    char userName[16];
    /** Password Field
     * Null-terminated, 15-character limit
     */
    char password[16];
    /** Local Timezone
     * Range: -12 to +14 hours from UTC
     */
    int8_t timeZone;
    /** Daylight Savings Time Offset
     * Offset in hours from standard time when daylight savings time is in effect
     */
    float dstOffset;
    /** Daylight Savings Time enable flag
     * True = enabled, False = disabled
     */
    bool dstEnabled;
    /** WiFi Hostname
     * Null-terminated, 31 Character limit
     */
    char hostName[32];
    /** Wifi Antenna Type Selection
     * For Particle Devices, may be Internal(ANT_INTERNAL), External(ANT_EXTERNAL), or Automatic(ANT_AUTO)
     */
    WLanSelectAntenna_TypeDef antennaType;
};

/*********************************************************************************************************
 * @brief The UserSettings Class
 * 
 * This encapsulates a common user settings object, containing such details as user name, time settings, etc.
 * Primary working RAM object image is loaded from EEPROM on startup. Change to any item results in an updated
 * object being rewritten to EEPROM.
 * 
 * A checksum is maintained to verify integrity of the EEPROM object image.
 * 
 * @note All access to the individual data items is made via getter/setter functions.
 * @todo Create a parent class to encapsulate all generic EEPROM block management functions. UserSettings would then become
 * a derived class. This would better support additional classes for storing additional sets of data in independant EEPROM blocks.

 */
class UserSettingsClass : public EEPROM_Class<SettingsObject>
{
private:
    /** Working copy of the Data Object that will reside in EEPROM
     */
    SettingsObject _mySettings = {"", "", 0, 0, false, "", ANT_INTERNAL};

    // Private functions for internal use

public:
    /** Constructor
     * 
     *
     */
    UserSettingsClass(): EEPROM_Class()
    {
        Log.trace("in UserSettingsClass Constructor.");
    }
    /** Destructor
     *
     */
    ~UserSettingsClass()
    {
   		Log.trace("in UserSettingsClass Destructor.");
	}


    /** Initializer: Loads working copy of object from EEPROM.
     * 
     * Checks integrity of EEPROM image before load, reinitializes to defaults if invalid.
     */
    bool begin(uint16_t address)
	{
		_adr_checksum = address;
		_adr_object = _adr_checksum + sizeof(_checksum);
		_eepromSize = sizeof(_mySettings) + sizeof(_checksum);
		EEPROM.get(_adr_checksum, _checksum);
		Log.trace("_adr_checksum: %d, _adr_object: %d, _eepromSize: %d, checksum: 0x%04X", _adr_checksum, _adr_object, _eepromSize, _checksum);

		return readObject(_mySettings);
	}

    /** Reinitializes data object and EEPROM image to defaults
     * 
     */
    void reinitialize();

    /** Prints contents of the data items to Serial
     */
    void logUserData();



    /******************************************
     * getters
     ******************************************/

    /** Get User Name.
     * @return char* to user name.
     */
    char *getUserName() { return _mySettings.userName; }
    /** Get Password.
     * @return char* to Password.
     */
    char *getPassword() { return _mySettings.password; }

    /** Get Time Zone
     * @return int8_t Time Zone value, UTC -12/+14 hours
     */
    int8_t getTimeZone() { return _mySettings.timeZone; }

    /** Get Daylight Savings Time offset
     * @return float Offset to standard time when DST is in effect
     */
    float getDstOffset() { return _mySettings.dstOffset; }

    /** Get Daylight Savings Time enable flag
     * @return bool true if enable, false if disabled
     */
    bool isDSTEnabled() { return _mySettings.dstEnabled; }

    /** Get Wifi Hostname.
     * @return char* to hostname.
     */
    char *getHostName() { return _mySettings.hostName; }
    /** get Antenna Type selection for Particle devices
     * @return ANT_INTERNAL, ANT_EXTERNAL, or ANT_AUTO
     */
    WLanSelectAntenna_TypeDef getAntennaType() { return _mySettings.antennaType; }

    /******************************************
     * setters
     ******************************************/

    /** set user name
     * @param[in] name string containing new user name (15-character limit)
     * @return bool false if input truncated, else true
     */
    bool setUserName(String name);

    /** set password
     * @param[in] password string containing new password (15-character limit)
     * @return bool false if input truncated, else true
     */
    bool setPassword(String password);

    /**
     * @brief Set the Time Zone object
     * 
     * @param tz 
     */
    void setTimeZone(int8_t tz)
    {
        _mySettings.timeZone = tz;
        writeObject(_mySettings);
    }
    /**
     * @brief Set the Dst Offset object
     * 
     * @param offset 
     */
    void setDstOffset(float offset)
    {
        _mySettings.dstOffset = offset;
        writeObject(_mySettings);
    }
    /**
     * @brief Set DST Enable Flag
     * 
     * @param flag 
     */
    void setDSTEnabled(bool flag)
    {
        _mySettings.dstEnabled = flag;
        writeObject(_mySettings);
    }

    /** set Wifi Hostname
     * @param[in] name string containing new Hostname (31-character limit)
     * @return bool false if input truncated, else true
     */
    bool setHostName(String name);

    /** set Antenna Type selection for Particle devices
     * @param[in] type ANT_INTERNAL, ANT_EXTERNAL, or ANT_AUTO
     * @return bool false if input invalid, else true
     */
    bool setAntennaType(WLanSelectAntenna_TypeDef type)
    {
        if ((type == ANT_INTERNAL) || (type == ANT_EXTERNAL) || (type == ANT_AUTO))
        {
            _mySettings.antennaType = type;
            writeObject(_mySettings);
            return true;
        }
        else
        {
            Log.warn("Invalid Antenna Type, setting not changed.");
            return false;
        }
        
            
    }
};
