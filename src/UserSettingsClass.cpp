/**
 * @file UserSettingsClass.cpp
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2019-09-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */
 
#include <Particle.h>
#include "UserSettingsClass.h"

/**
 * @brief Initialize the User Settings objects (fill from EEPROM)
 * 
 * @return true 
 * @return false 
 * 
 * Verifies storage integrity, if invalid then reinitialize with default values
 */
// bool UserSettingsClass::begin()
// {
//     // Verify Integrity
//     if (getObject(_mySettings))
//     {
//         Log.info("User Settings successfully loaded from EEPROM.");
//         return true;
//     }

//     else
//     {
//         Log.error("EEPROM User Settings Object corrupted, reinitializing...");
//         // Reinitialize Objects with defaults and load EEPROM
//         strcpy(_mySettings.userName, DEFAULT_USER_NAME);
//         strcpy(_mySettings.password, DEFAULT_USER_PW);
//         _mySettings.timeZone = DEFAULT_USER_TZ;
//         _mySettings.dstOffset = DEFAULT_USER_DSTOFFSET;
//         _mySettings.dstEnabled = DEFAULT_USER_DSTENABLE;
//         strcpy(_mySettings.hostName, DEFAULT_USER_HOSTNAME);
//         _mySettings.antennaType = DEFAULT_USER_ANTENNA;

//         // Reload EEPROM

//         writeObject(_mySettings);

//         // Return false so caller knows
//         return false;
//     }
// }
/******************************************************************************
 * Reinitialize Data Objects and EEPROM with defaults
 * This function must be updated as new objects are added
 ******************************************************************************/
void UserSettingsClass::reinitialize()
{
    Log.trace("Initializing EEPROM User Settings to defaults.");

    // Set local data to defaults

    strcpy(_mySettings.userName, DEFAULT_USER_NAME);
    strcpy(_mySettings.password, DEFAULT_USER_PW);
    _mySettings.timeZone = DEFAULT_USER_TZ;
    _mySettings.dstOffset = DEFAULT_USER_DSTOFFSET;
    _mySettings.dstEnabled = DEFAULT_USER_DSTENABLE;
    strcpy(_mySettings.hostName, DEFAULT_USER_HOSTNAME);
    _mySettings.antennaType = DEFAULT_USER_ANTENNA;

    // Reload EEPROM

    writeObject(_mySettings);

    Log.trace("User: Name %s, Password: %s", _mySettings.userName, _mySettings.password);
    Log.trace("Time Settings -- Timezone: %d, DST Offset: %f, DST Enabled: %s", _mySettings.timeZone, _mySettings.dstOffset, (_mySettings.dstEnabled) ? "Yes" : "No");
    Log.trace("Hostname: %s", _mySettings.hostName);
    switch (_mySettings.antennaType)
    {
    case ANT_INTERNAL:
        Log.trace("Antenna Type: Internal");
        break;

    case ANT_EXTERNAL:
        Log.trace("Antenna Type: External");
        break;

    case ANT_AUTO:
        Log.trace("Antenna Type: Auto");
        break;

    default:
        break;
    }
}

/******************************************************************************
 * For Debug: Print contents of the User Settings object
 * This function must be updated as new objects are added
 ******************************************************************************/
void UserSettingsClass::logUserData()
{
    Log.info("Stored User Data from EEPROM:");
    Log.info("User Name: %s", _mySettings.userName);
    Log.info("Password: %s", _mySettings.password);
    Log.info("Timezone: %d", _mySettings.timeZone);
    Log.info("DST Offset: %4.1f", _mySettings.dstOffset);
    Log.info("DST Enabled: %s", (_mySettings.dstEnabled) ? "Yes" : "No");
    Log.info("Hostname: %s", _mySettings.hostName);

    switch (_mySettings.antennaType)
    {
    case ANT_INTERNAL:
        Log.info("Antenna Type: Internal");
        break;

    case ANT_EXTERNAL:
        Log.info("Antenna Type: External");
        break;

    case ANT_AUTO:
        Log.info("Antenna Type: Auto");
        break;

    default:
        break;
    }

    Log.info("Checksum: 0x%04X\n", _checksum);
}

bool UserSettingsClass::setUserName(String userName)
{
    // Range check:
    if (userName.length() > (sizeof(_mySettings.userName) - 1))
    {
        strncpy(_mySettings.userName, userName, sizeof(_mySettings.userName) - 1);
        _mySettings.userName[sizeof(_mySettings.userName) - 1] = '\0';
        writeObject(_mySettings);
        Log.warn("User name too long, truncated.");
        return false;
    }
    else
    {
        strcpy(_mySettings.userName, userName);
        writeObject(_mySettings);
        return true;
    }

}

bool UserSettingsClass::setPassword(String password)
{
    // Range check:
    if (password.length() > (sizeof(_mySettings.password) - 1))
    {
        strncpy(_mySettings.password, password, sizeof(_mySettings.password) - 1);
        _mySettings.password[sizeof(_mySettings.password) - 1] = '\0';
        writeObject(_mySettings);
        Log.warn("Password too long, truncated.");
        return false;
    }
    else
    {
        strcpy(_mySettings.password, password);
        writeObject(_mySettings);
        return true;
    }
}

bool UserSettingsClass::setHostName(String hostName)
{
    strcpy(_mySettings.hostName, hostName);
    // Range check:
    if (hostName.length() > (sizeof(_mySettings.hostName) - 1))
    {
        strncpy(_mySettings.hostName, hostName, sizeof(_mySettings.hostName) - 1);
        _mySettings.hostName[sizeof(_mySettings.hostName) - 1] = '\0';
        writeObject(_mySettings);
        Log.warn("Hostname too long, truncated.");
        return false;
    }
    else
    {
        strcpy(_mySettings.hostName, hostName);
        writeObject(_mySettings);
        return true;
    }
}
